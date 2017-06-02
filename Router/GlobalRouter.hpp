//
//  GlobalRouter.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 31/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef GlobalRouter_hpp
#define GlobalRouter_hpp

#include <stdio.h>
#include "lefrw.h"
#include "defrw.h"
#include "Graph_SP.hpp"
#include "PDNHelper.hpp"
class Grid
{
public:
    Point<int> StartPoint ;
    int Width;
    int Length;
    int LowerMetal = 0 ; // UpperMetalLayer
    int UpperMetal = 0 ; // LowerMetalLayer
    int Capacity ;
    bool OverFlow ;
};


class GlobalRouter {
    
    
public:
    GlobalRouter();
    ~GlobalRouter();
    void Route();
private:
    // variable:
    PDNHelper helper ;
    vector< vector< Grid > > Graph ;
    set<int> Vertical ;
    set<int> Horizontal ;
    // key:BlockName , value:那個block的左下及右上座標
    map<string, pair<Point<int>, Point<int>>> BlockMap;
    // -----------------------------------------------------
    // function:
    void initLineVec();
    void initGraph();
    void initBlockMap();
    // 第一個為是不是屬於這個Block，第二個這個Block Name
    pair<bool, string> isBlock(Point<int> LeftDown , Point<int> RightUp);
    
    // 轉換2d座標為1d
    unsigned long translate(int row , int column);
};


#endif /* GlobalRouter_hpp */
