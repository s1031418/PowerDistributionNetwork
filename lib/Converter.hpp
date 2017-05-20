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

enum class orient
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

struct Block {
    Point<int> LeftDown ;
    Point<int> RightUp ;
    vector<string> Metals ;
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
    
    map<Line , vector<Point<int>>,MyComparator> CrossPointMap ;
    // key:BlockName
    map<string , vector<Block> > BlockMaps ;
    map<string , Line> ViaMap; 
    PDNHelper myhelper ; 
    
    void BuildBlockMaps();
    void BuildCrossPointMap();
    void initConverterState();
    Point<int> FlipX(float y_axis , Point<int> pt , orient orientation);
    Point<int> FlipY(float x_axis , Point<int> pt , orient orientation);
    void initLineMap(std::multimap<std::string,Nets> NetsMM , map<string , vector<Line>> & lineMap);
    bool isCross(Line line1 , Line line2);
    Point<int> getCrossPoint(Line line1 , Line line2);
    void print(vector<Point<int>> CrossPoints, Line myline);
    
    double calculateResistance(double rpsq , int width , double length );
    int getDistance(Point<int> pt1, Point<int> pt2);
    //假如是水平的就return 左右，垂直的return 上下
    pair<Point<int>, Point<int>> getOrder(Point<int> pt1, Point<int> pt2);
//  //return 左下角座標及右上角座標
    pair<Point<int>, Point<int>> getBlockCoordinate(int x , int y , int width , int length  , string orient ) ;
    
    pair<Point<int>, Point<int>> getRotatePoint(Point<int> BlcokLeftDown , Point<int> BlcokRightUp , Point<int> BlockPinLeftDown , Point<int> BlockPinRightUp , string orient);
    
};
#endif /* Converter_hpp */
