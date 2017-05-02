//
//  BlocklefParser.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 01/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "BlocklefParser.hpp"
#include <fstream>
#include <iostream>
#include <assert.h>

BlocklefParser::BlocklefParser(string FilePath)
{
    Load(FilePath);
}
BlocklefParser::~BlocklefParser()
{
    
}

REGEX_TYPE BlocklefParser::getMatchType(string data)
{
    for (int i = 0 ; i < Keywords.size() ; i++)
    {
        // if find the data
        if( data.find(Keywords[i]) != string::npos )
        {
            if( Keywords[i].compare("MACRO") == 0 )
                return REGEX_TYPE::MACRO ;
        }
    }
    return REGEX_TYPE::NONE;
}
void BlocklefParser::Load(string FilePath)
{
    string content ;
    fstream fin(FilePath , ios::in);
    if(!fin) cerr << "Can't open file.";
    while (getline(fin,content)) {
        Data.push_back(content);
    }
    fin.close();
}
int BlocklefParser::Parsing(REGEX_TYPE RT , int ParsingIdx )
{
    if(RT == REGEX_TYPE::MACRO)
    {
        Macro macro ;
        regex_t MacroNameRegex ;
        if ( regcomp(&MacroNameRegex, "[[:space:]]*MACRO[[:space:]]+([[:alnum:]]+)", REG_EXTENDED) ) cerr << "Compile Error";
        if( isMatch(MacroNameRegex,Data[ParsingIdx]) )
            macro.Name = GetParsingString(MacroNameRegex, Data[ParsingIdx], vector<int>{1})[0];
        else
            assert(0);
        ParsingIdx++ ;
        string MacroEndPattern = "[[:space:]]*END[[:space:]]+" + macro.Name  ;
        regex_t MacroEndRegex ; // Macro end condition
        regex_t SizeRegex ; // group 1、2
        regex_t LayerNameRegex ;
        regex_t PinNameRegex ;
        regex_t RectRegex ; // group 1、 3、5、7
        regex_t OBSRegex ;
        // compile regex
        // compile success return 0 , otherwise 1
        if ( regcomp(&MacroEndRegex, MacroEndPattern.c_str() , REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&SizeRegex, "[[:space:]]*SIZE[[:space:]]+([[:digit:]]+)[[:space:]]+BY[[:space:]]+([[:digit:]]+)", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&PinNameRegex, "[[:space:]]*PIN[[:space:]]+(([[:alnum:]]|_)+)", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&LayerNameRegex, "[[:space:]]*LAYER[[:space:]]+([[:alnum:]]+)", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&RectRegex, "[[:space:]]*RECT[[:space:]]+([+-]?[[:digit:]]+([.][[:digit:]]+)?)[[:space:]]+([+-]?[[:digit:]]+([.][[:digit:]]+)?)[[:space:]]+([+-]?[[:digit:]]+([.][[:digit:]]+)?)[[:space:]]+([+-]?[[:digit:]]+([.][[:digit:]]+)?)[[:space:]]*", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&OBSRegex, "[[:space:]]*OBS[[:space:]]*", REG_EXTENDED) ) cerr << "Compile Error";
        
        while (true)
        {
            // loop end condition
            if( isMatch(MacroEndRegex,Data[ParsingIdx]) )
            {
                break;
            }
            if( isMatch(SizeRegex, Data[ParsingIdx]))
            {
                vector<string> results = GetParsingString(SizeRegex, Data[ParsingIdx] , vector<int>{1,2});
                macro.Width = results[0];
                macro.Length = results[1];
            }
            else if(isMatch(PinNameRegex, Data[ParsingIdx]))
            {
                // sub block
                
                Pin pin ;
                InnerLayer innerlayer ;
                
                
                if( isMatch(PinNameRegex,Data[ParsingIdx]) )
                    pin.Name = GetParsingString(PinNameRegex, Data[ParsingIdx], vector<int>{1})[0];
                ParsingIdx++ ;
                
                regex_t PinEndRegex ;
                string PinEndPattern = "[[:space:]]*END[[:space:]]*" + pin.Name ;
                if ( regcomp(&PinEndRegex, PinEndPattern.c_str(), REG_EXTENDED) ) cerr << "Compile Error";
                while (true)
                {
                    if( isMatch(PinEndRegex,Data[ParsingIdx]) )
                    {
                        break;
                    }
                    else if( isMatch(LayerNameRegex,Data[ParsingIdx]) )
                    {
                         innerlayer.NAME = GetParsingString(LayerNameRegex, Data[ParsingIdx] , vector<int>{1})[0];
                    }
                    else if( isMatch(RectRegex,Data[ParsingIdx]))
                    {
                        vector<string> results = GetParsingString(RectRegex, Data[ParsingIdx] , vector<int>{1,3,5,7});
                        innerlayer.pt1.x = results[0];
                        innerlayer.pt1.y = results[1];
                        innerlayer.pt2.x = results[2];
                        innerlayer.pt2.y = results[3];
                        pin.InnerMaps.insert(make_pair(innerlayer.NAME, innerlayer));
                    }
                    ParsingIdx++;
                }
                macro.PinMaps.insert(make_pair(pin.Name, pin));
            }
            else if(isMatch(OBSRegex, Data[ParsingIdx]))
            {
                regex_t OBSEndRegex ;
                
                string OBSEndPattern = "[[:space:]]*END[[:space:]]*" ;
                if ( regcomp(&OBSEndRegex, OBSEndPattern.c_str(), REG_EXTENDED) ) cerr << "Compile Error";
                OBS obs ;
                InnerLayer innerlayer ;
                while (true)
                {
                    if (isMatch(OBSEndRegex,Data[ParsingIdx]))
                    {
                        break; 
                    }
                    else if( isMatch(LayerNameRegex,Data[ParsingIdx]) )
                    {
                        innerlayer.NAME = GetParsingString(LayerNameRegex, Data[ParsingIdx] , vector<int>{1})[0];
                    }
                    else if (isMatch(RectRegex,Data[ParsingIdx]))
                    {
                        vector<string> results = GetParsingString(RectRegex, Data[ParsingIdx] , vector<int>{1,3,5,7});
                        innerlayer.pt1.x = results[0];
                        innerlayer.pt1.y = results[1];
                        innerlayer.pt2.x = results[2];
                        innerlayer.pt2.y = results[3];
                        obs.InnerMaps.insert(make_pair(innerlayer.NAME, innerlayer));
                    }
                    ParsingIdx++ ;
                }
                macro.obs = obs ;
            }
            
            ParsingIdx++;
        }
        // insert into Macro Maps
        MacroMaps.insert(make_pair(macro.Name, macro));
        // free memory for regex
        regfree(&MacroNameRegex);
        regfree(&MacroEndRegex);
        regfree(&SizeRegex);
        regfree(&PinNameRegex);
        regfree(&LayerNameRegex);
        regfree(&RectRegex);
        regfree(&OBSRegex);
        
        return ParsingIdx;
        
    }
    else if (RT == REGEX_TYPE::NONE)
    {
        
        return ParsingIdx;
    }
    // it will not reach here
    assert(0);
    return -1 ;

}

void BlocklefParser::run()
{
    for (int i = 0 ; i < Data.size(); i++)
    {
        REGEX_TYPE RT = getMatchType(Data[i]);
        i = Parsing(RT , i );
    }
}
