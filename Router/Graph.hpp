//
//  Tree.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 11/08/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp

#include <stdio.h>
#include <vector>
#include "RouterComponents.hpp"
#include "Graph_SP.hpp"
#include <stack>
using namespace std;




struct Vertex
{
    // coordinate x , y is absolute coordinate , z is grid coordinate 
    Coordinate3D coordinate ;
    Vertex * fanIn ;
    vector<Vertex *> fanOut ;
    double current ;
    // 與fanin 得長度
    double length ; 
};


class Graph
{
    
public:
    Graph();
    ~Graph();
    string encode(Coordinate3D coordinate);
    Coordinate3D decode(string encodeString);
    void initialize(Coordinate3D source , double current , double length);
    void insert(Coordinate3D source , Coordinate3D target , double current , double length );
    vector<Coordinate3D> getPath(Coordinate3D target);
    void reduction();
    void reduceVertex(Vertex * front , Vertex * next);
    void destoryGraph();
private:
    Vertex * root ;
    map<string,Vertex *> LUT ;
    map<string,int> IDT;
};

#endif /* Graph_hpp */
