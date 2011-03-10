#include <iostream>
#include <fstream>
#include "rapidxml.hpp"
#include <stdio.h>


using namespace rapidxml;
using namespace std;

char * readFromFile(char* filename){
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
int main (int argc, char * const argv[]) {
	cout << "This is c++, welcome";
    // insert code here...
	
	char * text = readFromFile("temp.xml");
	
	cout << strlen(text);

	//strlen(xmltext);
	xml_document<> doc;
	doc.parse<0>(text);
	
	
	
    return 0;
}

