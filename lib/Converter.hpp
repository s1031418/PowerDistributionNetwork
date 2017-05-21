//
//  Converter.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 10/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Converter_hpp
#define Converter_hpp
#include <stdio.h>
#include "Components.h"
#include <unordered_map>
#include <vector>
#include "PDNHelper.hpp"
using namespace std;



enum FlipOrient
{
    TOP,
    DOWN,
    RIGHT,
    LEFT
    
};
struct MyComparator
{
    // 不確定
    bool operator()(const Line & left, const Line & right) const
    {
        if ( left.pt1.x == right.pt1.x )
        {
            return left.pt1.y < right.pt1.y;
        }
        
        return left.pt1.x < right.pt1.x;
    }
};


class Converter {
    
public:
    Converter();
    ~Converter();
    void toLocationFile();
//    void toSpice();
    void toSpice2();
private:
    
    vector<string> PinNames;
    
    
    // key:BlockName
    map<string , vector<Block> > BlockMaps ;
    
    PDNHelper myhelper ; 
    
    void initCrossPointMap(map<Line , vector<Point<int>>,MyComparator> & CrossPointMap ,map<string , vector<Line>> & lineMap , string PinName);
    void BuildBlockMaps();
    void BuildCrossPointMap();
    void initConverterState();
    Point<int> FlipX(float y_axis , Point<int> pt , FlipOrient orientation);
    Point<int> FlipY(float x_axis , Point<int> pt , FlipOrient orientation);
    void initLineMap(std::multimap<std::string,Nets> NetsMM , map<string , vector<Line>> & lineMap);
    
    
    void print(map<Line , vector<Point<int>>,MyComparator> & CrossPointMap);
    
    double calculateResistance(double rpsq , int width , double length );
    int getDistance(Point<int> pt1, Point<int> pt2);
    //假如是水平的就return 左右，垂直的return 上下
    pair<Point<int>, Point<int>> getOrder(Point<int> pt1, Point<int> pt2);
//  //return 左下角座標及右上角座標
    pair<Point<int>, Point<int>> getBlockCoordinate(int x , int y , int width , int length  , string orient ) ;
    
    pair<Point<int>, Point<int>> getRotatePoint(Point<int> BlcokLeftDown , Point<int> BlcokRightUp , Point<int> BlockPinLeftDown , Point<int> BlockPinRightUp , string orient);
    
};
#endif /* Converter_hpp */
