//
//  DefParser.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 01/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "DefParser.hpp"
#include <fstream>
#include <iostream>

DefParser::DefParser(string FilePath)
{
    Load(FilePath);
}

DefParser::~DefParser()
{
    
}
int DefParser::Parsing(REGEX_TYPE , int )
{
    return 0;
}
void DefParser::Load(string FilePath)
{
    string content ;
    fstream fin(FilePath , ios::in);
    if(!fin) cerr << "Can't open file.";
    while (getline(fin,content)) {
        Data.push_back(content);
    }
    fin.close();
}
REGEX_TYPE DefParser::getMatchType(string data)
{
    for (int i = 0 ; i < Keywords.size() ; i++)
    {
        // if find the data
        if( data.find(Keywords[i]) != string::npos )
        {
            if( Keywords[i].compare("DIEAREA") == 0 )
                return  REGEX_TYPE::DIEAREA;
            else if( Keywords[i].compare("COMPONENTS") == 0 )
                return REGEX_TYPE::COMPONENTS ;
            else if( Keywords[i].compare("PINS") == 0 )
                return REGEX_TYPE::PINS ;
            else if( Keywords[i].compare("SPECIALNETS") == 0 )
                return REGEX_TYPE::SPECIALNETS ;
        }
    }
    return REGEX_TYPE::NONE;
}
