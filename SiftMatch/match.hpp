/*
 * Speeded-Up Robust Features (SURF)
 * http://people.ee.ethz.ch/~surf
 *
 * Sample application for feature matching using nearest-neighbor
 * ratio method.
 *
 * BUILD USING "make match.ln".
 *
 * Author: Andreas Ess
 *
 * Copyright (2006): ETH Zurich, Switzerland
 * Katholieke Universiteit Leuven, Belgium
 * All rights reserved.
 *
 * For details, see the paper:
 * Herbert Bay,  Tinne Tuytelaars,  Luc Van Gool,
 *  "SURF: Speeded Up Robust Features"
 * Proceedings of the ninth European Conference on Computer Vision, May 2006
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for educational, research, and non-commercial
 * purposes, without fee and without a signed licensing agreement, is
 * hereby granted, provided that the above copyright notice and this
 * paragraph appear in all copies modifications, and distributions.
 *
 * Any commercial use or any redistribution of this software
 * requires a license from one of the above mentioned establishments.
 *
 * For further details, contact Andreas Ess (aess@vision.ee.ethz.ch).
 */

#ifndef SURFMATCH
#define SURFMATCH

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#include "ipoint.h"



using namespace std;
using namespace surf;

// Length of descriptor vector, ugly, global variable
int vlen = 128;




// Calculate square distance of two vectors
double distSquare(double *v1, double *v2, int n) {
	double dsq = 0.;
	while (n--) {
		dsq += (*v1 - *v2) * (*v1 - *v2);
		v1++;
		v2++;
	}
	return dsq;
}

// Find closest interest point in a list, given one interest point
int findMatch(const Ipoint& ip1, const vector< Ipoint >& ipts) {
	double mind = 1e100, second = 1e100;
	int match = -1;

	for (unsigned i = 0; i < ipts.size(); i++) {
		// Take advantage of Laplacian to speed up matching
		if (ipts[i].laplace != ip1.laplace)
			continue;

		double d = distSquare(ipts[i].ivec, ip1.ivec, vlen);

		if (d < mind) {
			second = mind;
			mind = d;
			match = i;
		} else if (d < second) {
			second = d;
		}
	}

	if (mind < 0.5 * second)
		return match;

	return -1;
}

// Find all possible matches between two images
vector< int > findMatches(const vector< Ipoint >& ipts1, const vector< Ipoint >& ipts2) {
	vector< int > matches(ipts1.size());
	int c = 0;
	for (unsigned i = 0; i < ipts1.size(); i++) {
		int match = findMatch(ipts1[i], ipts2);
		matches[i] = match;
		if (match != -1) {
//			cout << " Matched feature " << i << " in image 1 with feature "
//				<< match << " in image 2." << endl;
			c++;
		}
	}
	cout << " --> Matched " << c << " features of " << ipts1.size() << " in image 1." << endl;
	return matches;
}

// Load the interest points from a regular ASCII file
void loadIpoints(string sFileName, vector< Ipoint >& ipts) {
	ifstream ipfile(sFileName.c_str());
	if( !ipfile ) {
		cerr << "ERROR in loadIpoints(): "
			<< "Couldn't open file '" << sFileName.c_str() << "'!" << endl;
		return;
	}

	// Load the file header
	unsigned count;
	ipfile >> vlen >> count;

	// create a new interest point vector
	ipts.clear();
	ipts.resize(count);

	// Load the interest points in Mikolajczyk's format
	for (unsigned n = 0; n < count; n++) {
		// circular regions with diameter 5 x scale
		float x, y, a, b, c;

		// Read in region data, though not needed for actual matching
		ipfile >> x >> y >> a >> b >> c;

		float det = sqrt((a-c)*(a-c) + 4.0*b*b);
		float e1 = 0.5*(a+c + det);
		float e2 = 0.5*(a+c - det);
		float l1 = (1.0/sqrt(e1));
		float l2 = (1.0/sqrt(e2));
		float sc = sqrt( l1*l2 );

		ipts[n].x = x;
		ipts[n].y = y;
		ipts[n].scale = sc/2.5;

		// Read in Laplacian
		ipfile >> ipts[n].laplace;

		// SURF makes Laplacian part of descriptor, so skip it..
		ipts[n].ivec = new double[vlen - 1];
		for (unsigned j = 0; j < vlen - 1; j++)
			ipfile >> ipts[n].ivec[j];
	}
}

/*
void drawLine(Image *im, int x1, int y1, int x2, int y2) {
	if ((x1 < 0 && x2 < 0) || (y1 < 0 && y2 < 0) ||
	    (x1 >= im->getWidth() && x2 >= im->getWidth()) ||
            (y1 >= im->getHeight() && y2 >= im->getHeight()))
		return;

	bool steep = std::abs(y2 - y1) > std::abs(x2 - x1);
	if (steep) {
		int t;
		t = x1;
		x1 = y1;
		y1 = t;
		t = y2;
		y2 = x2;
		x2 = t;
	}

	if (x1 > x2) {
		// Swap points
		int t;
		t = x1;
		x1 = x2;
		x2 = t;
		t = y1;
		y1 = y2;
		y2 = t;
	}

	int deltax = x2 - x1;
	int deltay = std::abs(y2 - y1);

	int error = 0;
	int y = y1;
	int ystep = y1 < y2 ? 1 : -1;

	for (int x = x1; x < x2; x++) {
		if (steep) {
			if (x >= 0 && y >= 0 && y < im->getWidth() && x < im->getHeight())
				im->setPix(y, x, 1);
		} else {
			if (x >= 0 && y >= 0 && x < im->getWidth() && y < im->getHeight())
				im->setPix(x, y, 1);

		}
		error += deltay;

		if (2 * error > deltax) {
			y += ystep;
			error -= deltax;
		}
	}
}
*/

/*
void drawCross(Image *im, int x, int y, int s = 5) {
	for (int x1 = x - s; x1 <= x + s; x1++)
		im->setPix(x1, y, 1);
	for (int y1 = y - s; y1 <= y + s; y1++)
		im->setPix(x, y1, 1);
}
*/

int test() {
    //	Image *im1, *im2;
    vector< Ipoint > ipts1, ipts2;
    
	vector< int > matches = findMatches(ipts1, ipts2);


	return 0;
}

#endif