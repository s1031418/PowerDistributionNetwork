//
//  DefParser.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 01/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef DefParser_hpp
#define DefParser_hpp

#include <stdio.h>
#include <vector>
#include "Parsers.hpp"
#include <string>
#include <map>


using namespace std;
class DefParser : public Parsers{
    
    
public:
    DefParser(string );
    ~DefParser();
private:
    vector<string> Data ;
    void Load(string FilePath);
    int Parsing(REGEX_TYPE , int ); // return current parsing index
    REGEX_TYPE getMatchType(string data);
    vector<string> Keywords = {"DIEAREA" , "COMPONENTS" , "PINS" , "SPECIALNETS"} ;
};
#endif /* DefParser_hpp */
