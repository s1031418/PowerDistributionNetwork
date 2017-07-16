//
//  SpiceGenerator.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 16/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef SpiceGenerator_hpp
#define SpiceGenerator_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std ;

struct ResistanceLine
{
    string node1 ;
    string node2 ;
    double resistance ;
};
struct CurrentLine {
    string node ;
    double current ;
};
struct VoltageLine {
    string node ;
    double voltage ;
};

struct SpiceModel
{
    VoltageLine voltageLine;
    vector<ResistanceLine> resistanceSet ;
    vector<CurrentLine> currentSet ;
};
class SpiceGenerator {
    
public:
    SpiceGenerator();
    ~SpiceGenerator();
    void addSpiceResistance(string vdd , string node1 , string node2 , double resistance);
    void initSpiceVdd(string vdd , string node ,  double voltage);
    void addSpiceCurrent(string vdd , string node , double current );
    void addSpiceCmd();
    void setSpiceName(string spiceName);
    void toSpice();
private:
    string outputfileName ;
    map<string,SpiceModel> SpiceContainer ;
};

#endif /* SpiceGenerator_hpp */
