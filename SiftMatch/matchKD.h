//
//  matchKD.h
//  SiftMatch
//
//  Created by Fan ZHAO on 11-3-16.
//  Copyright 2011年 Personne. All rights reserved.
//
#ifndef KDNODE
#define KDNODE

#include "ipoint.h"
#include <iostream>
#include <vector>

using namespace surf;
using namespace std;

typedef struct kd_node {
    int ki;
    double kv;
    int leaf;
    Ipoint ip;
    kd_node * left;
    kd_node * right;
      
}kdnode;

bool less_than_x(Ipoint ip1, Ipoint ip2);

bool less_than_y(Ipoint ip1, Ipoint ip2);



class KD_Tree {
    
public:
    int k;
    kdnode * root;
    
    KD_Tree(int kd){
        this->k = kd;
    }
    
    kdnode * create(vector<Ipoint> *ips, int depth);
    void printTree(kdnode * node);
};

#endif