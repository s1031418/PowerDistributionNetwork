//
//  Parsers.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 01/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Parsers_hpp
#define Parsers_hpp

#include <stdio.h>
#include <regex.h>
#include <string>
#include <vector>
using namespace std;

enum class REGEX_TYPE
{
    DIEAREA, // DEF Parser
    COMPONENTS , // DEF Parser
    PINS ,// DEF Parser
    SPECIALNETS , // DEF Parser
    LAYER, // Tech lef Parser
    VIA , // Tech lef Parser
    MACRO , // Block lef Parser
    NONE
};

class Parsers {
    
public:
    Parsers();
    ~Parsers();
    bool isMatch(regex_t & regex , string data);
    vector<string> GetParsingString( regex_t & , string  data , vector<int> GroupIdx) ;
//  virtual void run() = 0 ; // pure virtual function
//    vector<string> GetParsingString( regex_t & , int  ParsingIdx , vector<int> GroupIdx) ;
private:
    
};
#endif /* Parsers_hpp */
