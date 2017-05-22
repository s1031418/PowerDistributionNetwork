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
#include <algorithm>
#include "PDNHelper.hpp"
#include <set>
#include "ngspice.hpp"
using namespace std;




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
    Converter(string Casename);
    ~Converter();
    void toLocationFile();
    void toSpice();
    void toNgspice();
    void Visualize();
private:
    string CaseName ;
    vector<string> PinNames;
    // key是起點座標 , value:終點座標 一個起點可能有多個終點
    multimap<string, string> DestinationMap;
    // key:BlockName
//    map<string , vector<Block> > BlockMaps ;
//    map<string , Block > PowerMaps ;
    vector<Line> ViaTable ;
    PDNHelper myhelper ;
    
    void initCrossPointMap(map<Line , vector<Point<int>>,MyComparator> & CrossPointMap ,map<string , vector<Line>> & lineMap , string PinName , pair<string, Point<int>> & Voltage_Msg , vector<pair<string, Point<int>>> & Current_Msg );
//    void BuildBlockMaps();
//    void BuildPowerMaps();
    void BuildCrossPointMap();
    void initConverterState();
    
//    Point<int> FlipX(float y_axis , Point<int> pt , FlipOrient orientation);
//    Point<int> FlipY(float x_axis , Point<int> pt , FlipOrient orientation);
    void initLineMap(std::multimap<std::string,Nets> NetsMM , map<string , vector<Line>> & lineMap , vector<Line> & ViaTable);
    void printVoltage(string MetalName , Point<int> StartPoint , string PinName , FILE * pfile );
    void printCurrent(vector<pair<string, Point<int>>> & Current_Msg , string PinName , FILE * pFile);
    void printResistance(map<Line , vector<Point<int>>,MyComparator> & CrossPointMap , string PinName, vector<Line> & ViaTable , FILE * pFile);
    
//    pair< Point<int>, Point<int> > getPowerPinCoordinate(int x , int y , Point<int> r_pt1, Point<int> r_pt2  , string orient);
    //return 左下角座標及右上角座標
    pair<Point<int>, Point<int>> getBlockCoordinate(int x , int y , int width , int length  , string orient ) ;
    
//    pair<Point<int>, Point<int>> getRotatePoint(Point<int> BlcokLeftDown , Point<int> BlcokRightUp , Point<int> BlockPinLeftDown , Point<int> BlockPinRightUp , string orient);
    
};
#endif /* Converter_hpp */
