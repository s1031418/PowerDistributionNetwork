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
    
    // 對Ｘ軸做鏡射
    Point<int> FlipX(float y_axis , Point<int> pt , FlipOrient orientation);
    
    // 對Ｙ軸做鏡射
    Point<int> FlipY(float x_axis , Point<int> pt , FlipOrient orientation);
    void InitBlockMaps();
    void InitPowerMaps();
    
    // 判斷兩個點是否水平
    bool isHorizontal(Point<int> pt1 , Point<int> pt2);
    //return 左下角座標及右上角座標
    pair<Point<int>, Point<int>> getBlockCoordinate(int x , int y , int width , int length  , string orient ) ;
    
    // 給我兩條線判斷是否交叉
    bool isCross(Line line1 , Line line2);
    
    //給我兩條線，回傳兩條線的交叉點
    //如果是Pseudo Line，就回傳Psedo Line的中點(VIA中點)
    Point<int> getCrossPoint(Line line1 , Line line2);
    
    // 給我兩個點，判斷是不是起點（ PowerPin內 ）
    // 如果是就把那個點回傳，否則回傳空的Point
    Point<int> getStartPoint(Point<int> pt1 , Point<int> pt2);
    
    // 給我兩個點，判斷是不是終點（ BlockPin內 ）
    // 如果是就把那個點回傳，否則回傳空的Point
    Point<int> getEndPoint(Point<int> pt1 , Point<int> pt2);
    
    double calculateResistance(double rpsq , int width , double length );
    
    //給我一個點，判斷他是屬於哪個Block跟哪個BlockPin
    //第一個是BlockName , BlockPinName
    pair<string, string> getBlockMsg(Point<int> pt);
    
    //給我一個點，判斷他是屬於哪個PowerPin
    // 回傳 PowerPin Name
    string getPowerPinMsg(Point<int> pt);
    
    //給Block左下跟右上角，BlockPin左下跟右上角，還有方向
    //回傳旋轉完的左下跟右上
    pair<Point<int>, Point<int>> getRotatePoint(Point<int> BlcokLeftDown , Point<int> BlcokRightUp , Point<int> BlockPinLeftDown , Point<int> BlockPinRightUp , string orient);
    
    // 將Detination Map 全部轉為 string
    string DestinationMapToString(multimap<string,string>& DestinationMap);
    // 將NETSMULTI MAP 全部轉為 string
    string NETSMULTIMAPSToString(multimap<string,Nets> & NETSMULTIMAPS);
    
    
};

#endif /* PDNHelper_hpp */

