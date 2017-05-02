//
//  TechTechlefParser.hpp
//  EDA_Contest2017(test)
//
//  Created by 吳峻陞 on 01/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef TechTechlefParser_hpp
#define TechTechlefParser_hpp



#include <regex.h>
#include <stdio.h>
#include "../lib/Components.h"
#include <string>
#include <vector>
#include <map>
#include "Parsers.hpp"
using namespace std ;



class TechlefParser : public Parsers {
    
public:
    TechlefParser(string FilePath);// the contructor of TechlefParser
    ~TechlefParser(); // the destructor of TechlefParser
    void run(); // acivate the parser
    map<string,Layer> LayerMaps ; // Layer Map , the key of layer map is Layer Name
    map<string,Via> ViaMaps ; // Via Map , the key of via  map is via name
private:
    vector<string> Data ; // load the cotent of File
    REGEX_TYPE getMatchType(string data);
    int Parsing(REGEX_TYPE , int ); // return current parsing index
    void Load(string FilePath);
    
    vector<string> Keywords = {"LAYER" , "VIA"} ;
    
};



#endif /* TechTechlefParser_hpp */
