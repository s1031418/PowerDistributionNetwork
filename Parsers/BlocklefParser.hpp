//
//  BlocklefParser.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 01/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef BlocklefParser_hpp
#define BlocklefParser_hpp

#include <stdio.h>
#include <vector>
#include "Parsers.hpp"
#include <string>
#include <map>
#include "../lib/Components.h"


class BlocklefParser : public Parsers
{
public:
    BlocklefParser(string );
    ~BlocklefParser();
    void run();
    map<std::string , Macro> MacroMaps ;// Macro Maps
private:
    vector<string> Data ;
    void Load(string FilePath);
    int Parsing(REGEX_TYPE , int ); // return current parsing index
    REGEX_TYPE getMatchType(string data);
    vector<string> Keywords = {"MACRO"} ;


};

#endif /* BlocklefParser_hpp */
