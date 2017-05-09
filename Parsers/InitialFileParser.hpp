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

extern multimap<string,PowerPin> CurrnetMaps ;
extern map<string,string> VoltageMaps;
extern multimap<string,Constraint> ConstraintMaps ;
extern map<string,string> WeightsMaps;
class InitialFileParser : public Parsers
{
    
public:
    InitialFileParser(string FilePath);
    ~InitialFileParser();
    
    void run();
private:
    vector<string> Data ;
    void Load(string FilePath);
    
};

#endif /* InitialFileParser_hpp */
