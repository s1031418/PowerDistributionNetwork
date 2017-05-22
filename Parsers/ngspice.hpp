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
#include <map>
using namespace std;
class ngspice {
    
    
public:
    ngspice();
    ngspice(string casename);
    ~ngspice();
    void ConcatIR_Drop();
    
//    bool analyze( string StartPowerPin , string EndBlockPin );
//    double getIR_DropCons(string StartPowerPin , string EndBlockPin );
private:
    map<string,double> VoltageMap;
    string CaseName;
};

#endif /* ngspice_hpp */
