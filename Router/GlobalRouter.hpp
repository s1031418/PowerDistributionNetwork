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
#include "verilog.hpp"
#include "lefrw.h"
#include "defrw.h"
#include "flute_net.hpp"
#include "RouterComponents.hpp"

// User Define Parameters
// Define Global Routing Grid Size
// Grid Size = Scaling * minimal space
const unsigned Scaling = 500 ;


class GlobalRouter {
    
    
public:
    GlobalRouter();
    ~GlobalRouter();
    void Route();
    void toGridGraph();
private:
    // variable:
    PDNHelper helper ;
    vector< vector< Grid > > Grids ;
    set<int> Vertical ;
    set<int> Horizontal ;
    Graph_SP graph;
    
    // key:BlockName , value:那個block的左下及右上座標
    map<string, pair<Point<int>, Point<int>>> BlockMap;
    // -----------------------------------------------------
    // function:
    
    // Cut By BlockBoundary ( non-uniform )
    void CutByBlockBoundary();
    
    // Cut By Minimal Space ( uniform )
    void CutByUserDefine();
    
    void initGrids();
    void initBlockMap();
    void initGraph_SP();
    // 第一個為是不是屬於這個Block，第二個這個Block Name
    pair<bool, string> isBlock(Point<int> LeftDown , Point<int> RightUp);
    
    // 判斷這個grid是否為block
    bool isBlock(Grid grid);
    // 轉換2d座標為1d
    int translate2D_1D(int x , int y);
    // 轉換1d座標為2d
    pair<int, int> translate1D_2D(int index);
    
    // 給我一個方塊的左下右上，算出他在哪些Grid裡面
    // 如果這個方塊介於兩個的Grid之間
    // return 包比較多的那個GridY
    // 第一個值為X座標，第二個為Y座標
    pair<int, int> getGridCoordinate( Point<int> LeftDown , Point<int> RightUp );
    
    // 給我一個點，算出它屬於哪一個Grid
    // 如果點在交界上，如果是Ｘ就return左邊的，如果是Ｙ就return下面的
    // 第一個值為X座標，第二個為Y座標
    pair<int, int> getGridCoordinate( Point<int> pt );
    
    
    pair<int, int> ripple(int X , int Y);
    
    // print shortest path of Dijkstra algorithm
    void printPath(vector<int> & path);
    
    // get x y coordinate of flute results 
    Point<int> getTerminalCoordinate(int x , int y , int target , vector<node> & SteierTree) ;
    
};


#endif /* GlobalRouter_hpp */
