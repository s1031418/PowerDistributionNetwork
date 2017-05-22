//
//  PDNHelper.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 20/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef PDNHelper_hpp
#define PDNHelper_hpp

#include <stdio.h>
#include "Components.h"
#include <cmath>
#include "lefrw.h"
#include "defrw.h"
#include <string>
using namespace std ;
enum FlipOrient
{
    TOP,
    DOWN,
    RIGHT,
    LEFT
    
};

class PDNHelper {
    
public:
    PDNHelper();
    ~PDNHelper();
    map<string , Block > PowerMaps ;
    map<string , vector<Block> > BlockMaps ;
    pair< Point<int>, Point<int> > getPowerPinCoordinate(int x , int y , Point<int> r_pt1, Point<int> r_pt2  , string orient);
    Point<int> FlipX(float y_axis , Point<int> pt , FlipOrient orientation);
    Point<int> FlipY(float x_axis , Point<int> pt , FlipOrient orientation);
    void InitBlockMaps();
    void InitPowerMaps();
    bool isHorizontal(Point<int> pt1 , Point<int> pt2);
    pair<Point<int>, Point<int>> getBlockCoordinate(int x , int y , int width , int length  , string orient ) ;
    bool isCross(Line line1 , Line line2);
    Point<int> getCrossPoint(Line line1 , Line line2);
    Point<int> getStartPoint(Point<int> pt1 , Point<int> pt2);
    Point<int> getEndPoint(Point<int> pt1 , Point<int> pt2);
    double calculateResistance(double rpsq , int width , double length );
    //第一個是BlockName , BlockPinName
    pair<string, string> getBlockMsg(Point<int> pt);
    string getPowerPinMsg(Point<int> pt);
    pair<Point<int>, Point<int>> getRotatePoint(Point<int> BlcokLeftDown , Point<int> BlcokRightUp , Point<int> BlockPinLeftDown , Point<int> BlockPinRightUp , string orient);
};

#endif /* PDNHelper_hpp */

