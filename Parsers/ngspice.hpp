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
#include "../PDN/PDNHelper.hpp"
#include <map>
#include <regex.h>

using namespace std;

class ngspice : public Parsers
{
public: 
    ngspice();
    ~ngspice();
    void initvoltage();
    map<string,double> voltages;
private:
    vector<string> Data ;
    // key值是sp的座標,value是voltage
    void LoadFile();
};

#endif /* ngspice_hpp */
