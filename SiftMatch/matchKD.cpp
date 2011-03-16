//
//  matchKD.cpp
//  SiftMatch
//
//  Created by Fan ZHAO on 11-3-16.
//  Copyright 2011年 Personne. All rights reserved.
//

#include "matchKD.h"


kdnode * KD_Tree::create(vector<Ipoint> *ips, int depth){
    if (ips -> size() != 0){
        
        printf("%d\n",ips->size());
        
        int axis = depth % k;
                
        int median = ips->size() / 2;
        median --;

        
//        for(int i=0; i< 128; i++){
//            printf("%f \n ",ips->at(0).ivec[i]);
//        }
        
        
        kdnode * node = new kdnode();
        node -> ip = ips->at(median);
        ips->erase(ips->begin()+median-1);
        
        
        node -> left = this->create(ips, depth+1);
        node -> right = this->create(ips, depth+1);
        return node;
    }
    return NULL;
}
bool less_than_x(Ipoint ip1, Ipoint ip2){
    if (ip1.x < ip2.x){
        return true;
    }else{
        return false;
    }
}
bool less_than_y(Ipoint ip1, Ipoint ip2){
    if (ip1.y < ip2.y){
        return true;
    }
    else {
        return false;
    }
}

void KD_Tree::printTree(kdnode * node = NULL){
    kdnode * p ;
    if (node == NULL){
        printf("null");
        p = this->root;
    }
    else{
        p = node;
    }
    
    
}
