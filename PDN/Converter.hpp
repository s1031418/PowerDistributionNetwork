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
#include "../Parsers/Components.h"
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "PDNHelper.hpp"
#include <set>
#include "../Parsers/ngspice.hpp"
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
    void toOutputDef();
    void toOutputFile();
    void Visualize();
    multimap<string, string> DestinationMap;
private:
    string CaseName ;
    vector<string> PinNames;
    // key是起點座標 , value:終點座標 一個起點可能有多個終點
    ngspice ng ;
    vector<Line> ViaTable ;
    // 用來儲存 inputdef 
    vector<string> Data ;
    // key值為metal層，value為這層用的金屬面積
    map<string,long double> MetalUsage ;
    
    PDNHelper myhelper ;
    void caluMetalUse(map<string , vector<Line>> & lineMap );
    void initCrossPointMap(map<Line , vector<Point<int>>,MyComparator> & CrossPointMap ,map<string , vector<Line>> & lineMap , string PinName , pair<string, Point<int>> & Voltage_Msg , vector<pair<string, Point<int>>> & Current_Msg );
    void BuildCrossPointMap();
    void initConverterState();
    void initLineMap(std::multimap<std::string,Nets> NetsMM , map<string , vector<Line>> & lineMap , vector<Line> & ViaTable);
    void printVoltage(string MetalName , Point<int> StartPoint , string PinName , FILE * pfile );
    void printCurrent(vector<pair<string, Point<int>>> & Current_Msg , string PinName , FILE * pFile);
    void printResistance(map<Line , vector<Point<int>>,MyComparator> & CrossPointMap , string PinName, vector<Line> & ViaTable , FILE * pFile);
    void printOutputFile();
    string getAlias(string name);
};
#endif /* Converter_hpp */
