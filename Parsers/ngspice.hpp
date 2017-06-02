//
//  ngspice.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 22/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef ngspice_hpp
#define ngspice_hpp
#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "InitialFileParser.hpp"
#include <cstring>
#include "PDNHelper.hpp"
#include <map>
#include <regex.h>
using namespace std;
class ngspice : public Parsers{
    
    
public:
    ngspice(string casename);
    ~ngspice();
    void initvoltage();
    void ConcatIR_Drop();
    double getIR_DropCons(string StartPowerPin , string EndBlockPin );
    void printStats(multimap<string, string> & DetinationMap);
    
private:
    PDNHelper helper ; 
    vector<string> Data ;
    // key值是sp的座標,value是voltage
    map<string,double> voltages;
    string CaseName;
    void LoadFile();
    // 將sp的座標轉為Point
    Point<int> translateToPoint(string key);
};

#endif /* ngspice_hpp */
