//
//  PDN.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 17/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef PDN_hpp
#define PDN_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "Components.h"
using namespace std ;


//class Line
//{
//public:
//    Point<int> pt1;
//    Point<int> pt2;
//    string MetalName ;
//    int Width ; 
//};
//
//class WireNode
//{
//public:
//    Point<int> pt;
//    WireNode * top = nullptr;
//    WireNode * down = nullptr ;
//    WireNode * left = nullptr;
//    WireNode * right = nullptr;
//    string MetalName ;
//    int V_Width = -1; // Vertical Width
//    int H_Width = -1; // Horizontal Width
//};
//class ColumnNode
//{
//public:
//    WireNode * ulink = nullptr; // down link
//    ColumnNode * rlink = nullptr;
//    unsigned int index = 0;
//};
//class RowNode {
//    
//public:
//    WireNode * rlink = nullptr;
//    RowNode * ulink = nullptr;
//    unsigned int index  = 0 ;
//};
//
//class Root
//{
//public:
//    int TotalColumn = 0 ;
//    int TotalRow = 0;
//    ColumnNode * rlink = nullptr; // right link
//    RowNode * ulink = nullptr ; // up link
//    ColumnNode * rmostlink = nullptr; // right most link
//    RowNode * umostlink = nullptr; // up most link
//};
//
//typedef RowNode * RowNodePointer;
//typedef ColumnNode * ColumnNodePointer;
//typedef WireNode * WireNodePointer;

class PDN
{
public:
    PDN();
    ~PDN();
    void optimize();
    void toSpice();
    void BuildDataStructure();
private:
//    Root * root ;
//    void getLineVec(vector<Line> & VertialLine_Vec , vector<Line> & HorizontalLine_Vec );
//    void createColumnNode();
//    bool isHorizontal(Point<int> pt1 , Point<int> pt2);
//    void FreeMemory(Root * root);
};

#endif /* PDN_hpp */
