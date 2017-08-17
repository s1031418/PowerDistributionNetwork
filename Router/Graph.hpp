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
#include <queue>
#include "lefrw.h"
#include "defrw.h"
#include "RouterUtil.hpp"
using namespace std;




struct Vertex
{
    // coordinate x , y is absolute coordinate , z is grid coordinate 
    Coordinate3D coordinate ;
    Vertex * fanIn ;
    vector<Vertex *> fanOut ;
    // 與fanin 得長度
    double length = 0 ;
    double current = 0 ;
    double width = 0 ; 
};
struct LeafInfo {
    string powerPin ;
    string block ;
    string blockPin ;
};

class Graph
{
    
public:
    Graph();
    ~Graph();
    string encode(Coordinate3D coordinate);
    Coordinate3D decode(string encodeString);
    void initialize(Coordinate3D source );
    void insert(Coordinate3D source , Coordinate3D target , double length );
    vector<Vertex *> getPath(Coordinate3D target);
    void addLeaf(Coordinate3D leaf , double constraint , double current , double voltage);
    void addLeafInfo(Coordinate3D leaf , string powerPin , string block , string blockPin);
    LeafInfo getLeafInfo();
    // 依照fanout 或 via
    void reduction();
//    void reduceVertex(Vertex * front , Vertex * next);
    void rectifyCurrent();
    void rectifyWidth();
    Vertex * getVertex(Coordinate3D coordinate);
    void destoryGraph();
    pair<double, double> getQuadraticEquation(double a , double b , double c );
    
    void printAllPath();
    
    vector<vector<Vertex *>> traverse();
private:
    Vertex * root ;
    map<string,Vertex *> LUT ; // Look Up Table (query ptr)
    map<string,double> COT ; // Constaint Table(query constraint)
    map<string,double> CUT ; // Current Table(query constraint)
    map<string,double> VT ; // Voltage Table (query voltage)
    queue<LeafInfo> leafQueue ;
    vector<Vertex *> leafs ;
    RouterUtil RouterHelper; 
};

#endif /* Graph_hpp */
