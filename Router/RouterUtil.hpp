//
//  RouterUtil.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 13/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef RouterUtil_hpp
#define RouterUtil_hpp

#include <stdio.h>
#include "InitialFileParser.hpp"
#include "RouterComponents.hpp"
#include <string>
#include "lefrw.h"
#include "defrw.h"
#include "verilog.hpp"
#include "PDNHelper.hpp"
#include <algorithm>
using namespace std;


struct BlockCoordinate {
    Point<int> LeftDown ;
    Point<int> RightUp ;
    int lowerMetal = 0 ;
    int upperMetal = 0 ;
};

struct CrossInfo {
    bool isCross = false ;
    int lowerMetal = -1;
    int upperMetal = -1;
    bool isUpEdgeBlock = false;
    bool isDownEdgeBlock = false;
    bool isLeftEdgeBlock = false;
    bool isRightEdgeBlock = false;
    bool viaIsCross = false ;
};

class RouterUtil : public PDNHelper 
{
    
public:
    RouterUtil();
    ~RouterUtil();
    
    
    // 判斷這個grid是否為block
    bool IsBlock(Grid grid);
    // 第一個為是不是屬於這個Block，第二個這個Block Name
    pair<bool, string> IsBlock(Point<int> LeftDown , Point<int> RightUp);
    
    CrossInfo isCrossWithBlock(Rectangle rect1 , Rectangle via , BlockCoordinate & block , int width , int spacing);
    
    // first : whether cross with block
    // second: if cross , the lower metal of this block
    // thrid: if cross , the upper metal of this block 
    CrossInfo isCrossWithBlock(Rectangle rect1 );
    
    CrossInfo isCrossWithObstacle( Rectangle rect1  , string source , map<string,vector<BlockCoordinate>> & obstacles);
    
    
    // 如果完全沒有相交 return -1 , -1
    // 如果相交在一個點就return rectangle 左下右上都是那個點
    // 回傳 相交矩形的左下右上
    Rectangle getCrossRect(Rectangle rect1 , Rectangle rect2);
    
    int getRectArea(Rectangle rect1);
    
    vector<int> getPowerPinGridCandidate(Rectangle rect);
    
    string getLCS(string s1,string s2);
    
    
    int translateMetalNameToInt(string MetalName);
    
    string translateIntToMetalName(int layer);
    
    tuple<bool,bool,bool,bool> getCrossEdge();
    
    int getWeights(int layer);
    
    vector<Rectangle> getBlockRectangle();

    map<string, BlockCoordinate > BlockMap;
    
    int getViaWeight(int area , int layer);
    
private:
    
    
    void InitBlockMap();
    // key:BlockName , value:那個block的左下及右上座標
    
};


#endif /* RouterUtil_hpp */
