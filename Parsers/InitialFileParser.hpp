//
//  InitialFileParser.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 02/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef InitialFileParser_hpp
#define InitialFileParser_hpp

#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include "Parsers.hpp"
class InitialFileParser : public Parsers
{
    
public:
    
private:
    vector<string> Data ;
    void Load(string FilePath);
    int Parsing(REGEX_TYPE , int ); // return current parsing index
    REGEX_TYPE getMatchType(string data);
    vector<string> Keywords = {"MACRO"} ;
};

#endif /* InitialFileParser_hpp */
