#include <iostream>
#include <fstream>
#include "rapidxml.hpp"
#include <stdio.h>
#include "ipoint.h"
#include "match.hpp"
#include "matchKD.h"

using namespace rapidxml;
using namespace std;
using namespace surf;

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

vector<Ipoint> readXml2Ipoint(char * text){
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




int main (int argc, char * const argv[]) {
	cout << "This is c++, welcome" << endl;
    // insert code here...
	
	char * text = readFromFile("test.xml");
	
    cout<<"I have read "<< strlen(text) << " characters" << endl;

    
    vector<Ipoint> ipt1 = readXml2Ipoint(text);
    
    //vector< int > matches = findMatches(ipt1, ipt1);
    
    KD_Tree *tree = new KD_Tree(2);
    
    tree->create(&ipt1, 0);
    
    tree->printTree(NULL);
    
    return 0;
}

