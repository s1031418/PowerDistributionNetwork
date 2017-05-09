//
//  InitialFileParser.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 02/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef InitialFileParser_hpp
#define InitialFileParser_hpp
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <map>
#include <vector>
#include "Components.h"
#include <string>
#include "Parsers.hpp"
class InitialFileParser : public Parsers
{
    
public:
    InitialFileParser(string FilePath);
    ~InitialFileParser();
    multimap<string,PowerPin> CurrnetMaps ;
    map<string,string> VoltageMaps;
    multimap<string,Constraint> ConstraintMaps ;
    map<string,string> WeightsMaps;
    void run();
private:
    vector<string> Data ;
    void Load(string FilePath);
    
};

#endif /* InitialFileParser_hpp */
