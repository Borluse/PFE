#include <iostream>
#include <fstream>
#include "rapidxml.hpp"
#include <stdio.h>
//#include "ipoint.h"
//#include "match.hpp"
//#include "matchKD.h"
    #include "imgfeatures.h"

extern "C"{
    #include "kdtree.h"

}
using namespace rapidxml;
using namespace std;
//using namespace surf;


/* the maximum number of keypoint NN candidates to check during BBF search */
#define KDTREE_BBF_MAX_NN_CHKS 200

/* threshold on squared ratio of distances between NN and 2nd NN */
#define NN_SQ_DIST_RATIO_THR 0.49

char * readFromFile(const char* filename){
	FILE * fp;
	fp = fopen(filename, "r");
	long size;
	
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
		size_t results;
	
	
	char * text;
	text = (char *) malloc(size* sizeof(char));
	
	if (text == NULL) {fputs ("Memory error",stderr); exit (2);}
	
	// copy the file into the buffer:
	results = fread (text,1,size,fp);
	
	if (results != size){fputs ("Reading error",stderr); exit (3);}
	
	fclose(fp);
	return text;
}

/*vector<Ipoint> readXml2Ipoint(char * text){
    xml_document<> doc;
	doc.parse<0>(text);
    vector<Ipoint> pts;
    xml_node<> *node = doc.first_node();
    node = node->first_node("point");

    do {
        Ipoint pt;
        char * end;
        xml_node<> * temp = node -> first_node("x");
        
        pt.x = strtod(temp->value(), &end);
        temp = node -> first_node("y");
        pt.y = strtod(temp->value(), &end);
        temp = node -> first_node("d");
        pt.laplace = 0;
        
        int idx = 0;
        pt.allocIvec(128);
        do {
            pt.ivec[idx++]= strtod(temp->value(), &end);
        } while ((temp = temp -> next_sibling()) != NULL);

        pts.push_back(pt);        
        
    } while ((node = node -> next_sibling()) != NULL);
    
    return pts;

}
*/
struct feature * readXml2Feat(char * text , int &i){
    xml_document<> doc;
    doc.parse<0>(text);
    struct feature * feat;
    //feat = (struct feature *) malloc(1*sizeof(struct feature));
    xml_node<> *node = doc.first_node();
    node = node->first_node("point");
    int count = 0;
    do {
        feat = (struct feature *)realloc(feat, (count+1) * sizeof(struct feature));
        struct feature feat1;
        char * end;
        xml_node<> * temp = node->first_node("x");
        
        feat1.x = strtod(temp->value(), &end);
        temp = node->first_node("y");
        feat1.y = strtod(temp->value(), &end);
        temp = node -> first_node("d");
        
        feat1.d = 128;
        for (int i=0; i<128; i++){
            feat1.descr[i++] = strtod(temp->value(), &end);
            temp = temp->next_sibling();
        }
        feat1.feature_data = NULL;
        feat1.fwd_match = NULL;
        feat1.bck_match = NULL;
        feat1.mdl_match = NULL;
        memcpy(&feat[count], &feat1, sizeof(struct feature));
        count++;
        
    }while ((node = node -> next_sibling()) != NULL);
    i = count;
    return feat;
}



int main (int argc, char * const argv[]) {
	cout << "This is c++, welcome" << endl;
    // insert code here...
	
	char * text = readFromFile("test1.xml");
    char * text1 = readFromFile("test.xml");
	
    cout<<"I have read "<< strlen(text) << " characters" << endl;
    int size;
    struct feature *feat = readXml2Feat(text, size);
    
    struct kd_node * kd_root;
    int n1, n2;
    n1 = size;
    struct feature *feat1 = readXml2Feat(text1, n2);

    int i, k, m = 0;
    double d0, d1;
    struct feature ** nbrs;
    
    kd_root = kdtree_build( feat1, n2 );
    for( i = 0; i < n1; i++ )
    {
        feat = feat1 + i;
        k = kdtree_bbf_knn( kd_root, feat, 2, &nbrs, KDTREE_BBF_MAX_NN_CHKS );
        if( k == 2 )
        {
            d0 = descr_dist_sq( feat, nbrs[0] );
            d1 = descr_dist_sq( feat, nbrs[1] );
            if( d0 < d1 * NN_SQ_DIST_RATIO_THR )
            {
                //pt1 = cvPoint( cvRound( feat->x ), cvRound( feat->y ) );
                //pt2 = cvPoint( cvRound( nbrs[0]->x ), cvRound( nbrs[0]->y ) );
                //pt2.y += img1->height;
                //cvLine( stacked, pt1, pt2, CV_RGB(255,0,255), 1, 8, 0 );
                m++;
                feat1[i].fwd_match = nbrs[0];
            }
        }
        free( nbrs );
    }
    
    fprintf( stderr, "Found %d total matches\n", m );

    
        return 0;
}

