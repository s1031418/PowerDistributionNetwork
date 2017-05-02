//
//  TechlefParser.cpp
//  EDA_Contest2017(test)
//
//  Created by 吳峻陞 on 01/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "TechlefParser.hpp"
#include <fstream>
#include <iostream>
#include <regex.h>
#include <assert.h>

TechlefParser::TechlefParser(string FilePath)
{
    Load(FilePath);
    //    for(int i = 0 ; i < Data.size() ; i++)
    //        cout << Data[i] << endl;
}
TechlefParser::~TechlefParser()
{
    
}

void TechlefParser::Load(string FilePath)
{
    string content ;
    fstream fin(FilePath , ios::in);
    if(!fin) cerr << "Can't open file.";
    while (getline(fin,content)) {
        Data.push_back(content);
    }
    fin.close();
}
REGEX_TYPE TechlefParser::getMatchType(string data)
{
    
    for (int i = 0 ; i < Keywords.size() ; i++)
    {
        // if find the data
        if( data.find(Keywords[i]) != string::npos )
        {
            if( Keywords[i].compare("LAYER") == 0 )
                return REGEX_TYPE::LAYER ;
            else if( Keywords[i].compare("VIA") == 0 )
                return REGEX_TYPE::VIA ;
        }
    }
    return REGEX_TYPE::NONE;
}
//逐行讀取，不支援不換行的。
int TechlefParser::Parsing(REGEX_TYPE RT , int ParsingIdx)
{
    
    if(RT == REGEX_TYPE::LAYER)
    {
        Layer layer ;
        regex_t LayerRegex ;
        regex_t TypeRegex ;
        regex_t WidthRegex;
        regex_t MaxwidthRegex ;
        regex_t SpacingRegex ;
        regex_t PitchRegex  ;
        regex_t OffsetRegex  ;
        regex_t DirectionRegex ;
        regex_t Resistance_RPersqRegex ;
        // compile regex
        // compile success return 0 , otherwise 1
        if ( regcomp(&LayerRegex, "^[[:space:]]*LAYER[[:space:]]+([[:alnum:]]+)", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&TypeRegex, "^[[:space:]]*TYPE[[:space:]]+([[:alnum:]]+)[[:space:]]*", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&WidthRegex, "^[[:space:]]*WIDTH[[:space:]]+([[:alnum:]]+)[[:space:]]*", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&MaxwidthRegex, "^[[:space:]]*MAXWIDTH[[:space:]]+([[:alnum:]]+)[[:space:]]*", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&SpacingRegex, "^[[:space:]]*SPACING[[:space:]]+([[:alnum:]]+)[[:space:]]*", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&PitchRegex, "^[[:space:]]*PITCH[[:space:]]+([[:alnum:]]+)[[:space:]]*", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&OffsetRegex, "^[[:space:]]*OFFSET[[:space:]]+([[:alnum:]]+)[[:space:]]*", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&DirectionRegex, "^[[:space:]]*DIRECTION[[:space:]]+([[:alnum:]]+)[[:space:]]*", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&Resistance_RPersqRegex, "^[[:space:]]*RESISTANCE RPERSQ[[:space:]]+([+-]?[[:digit:]]+(\\.[[:digit:]]+[eE]?[-+]?([[:digit:]]+)?)?)[[:space:]]*", REG_EXTENDED) ) cerr << "Compile Error";
        while (true)
        {
            // loop end condition
            if( Data[ParsingIdx].find("END") != string::npos )
            {
                break;
            }
            
            if( isMatch(LayerRegex, Data[ParsingIdx]))
            {
                layer.NAME = GetParsingString(LayerRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else if(isMatch(TypeRegex, Data[ParsingIdx]))
            {
                layer.TYPE = GetParsingString(TypeRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else if(isMatch(MaxwidthRegex, Data[ParsingIdx]))
            {
                layer.MAXWIDTH = GetParsingString(MaxwidthRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else if(isMatch(WidthRegex, Data[ParsingIdx]))
            {
                layer.WIDTH = GetParsingString(WidthRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else if(isMatch(SpacingRegex, Data[ParsingIdx]))
            {
                layer.SPACING = GetParsingString(SpacingRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else if(isMatch(PitchRegex, Data[ParsingIdx]))
            {
                layer.PITCH_DISTANCE = GetParsingString(PitchRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else if(isMatch(OffsetRegex, Data[ParsingIdx]))
            {
                layer.OFFSET_DISTANCE = GetParsingString(OffsetRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else if(isMatch(DirectionRegex, Data[ParsingIdx]))
            {
                layer.DIRECTION = GetParsingString(DirectionRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else if(isMatch(Resistance_RPersqRegex, Data[ParsingIdx]))
            {
                layer.RESISTANCE_RPERSQ = GetParsingString(Resistance_RPersqRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else
            {
                assert(0);
            }
            ParsingIdx++;
        }
        // insert into Layer Map
        LayerMaps.insert(make_pair(layer.NAME, layer));
        // free memory for regex
        regfree(&LayerRegex);
        regfree(&TypeRegex);
        regfree(&WidthRegex);
        regfree(&MaxwidthRegex);
        regfree(&SpacingRegex);
        regfree(&PitchRegex);
        regfree(&OffsetRegex);
        regfree(&DirectionRegex);
        regfree(&Resistance_RPersqRegex);
        return ParsingIdx;
        
    }
    else if (RT == REGEX_TYPE::VIA)
    {
        Via via ;
        InnerLayer innerlayer;
        regex_t ViaRegex ;
        regex_t ResistanceRegex ;
        regex_t LayerOfViaNameRegex ;
        regex_t LayerOfViaPointRegex ;
        // compile regex
        // compile success return 0 , otherwise 1
        if ( regcomp(&ViaRegex, "[[:space:]]*VIA[[:space:]]+(([[:alnum:]]|_)+)", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&ResistanceRegex, "[[:space:]]*RESISTANCE[[:space:]]+(([+-]?[[:digit:]]+(\\.[[:digit:]]+[eE]?[-+]?([[:digit:]]+)?)?))", REG_EXTENDED) ) cerr << "Compile Error";
        if ( regcomp(&LayerOfViaNameRegex, "[[:space:]]*LAYER[[:space:]]+([[:alnum:]]+)", REG_EXTENDED) ) cerr << "Compile Error";
        // group 1、3、5、7
        if ( regcomp(&LayerOfViaPointRegex, "[[:space:]]*RECT[[:space:]]+([+-]?[[:digit:]]+([.][[:digit:]]+)?)[[:space:]]+([+-]?[[:digit:]]+([.][[:digit:]]+)?)[[:space:]]+([+-]?[[:digit:]]+([.][[:digit:]]+)?)[[:space:]]+([+-]?[[:digit:]]+([.][[:digit:]]+)?)[[:space:]]*", REG_EXTENDED) ) cerr << "Compile Error";
        while (true)
        {
            if( Data[ParsingIdx].find("END") != string::npos )
            {
                
                break;
            }
            if( isMatch(ViaRegex, Data[ParsingIdx]) )
            {
                via.NAME = GetParsingString(ViaRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else if(isMatch(ResistanceRegex, Data[ParsingIdx]) )
            {
                via.RESISTANCE = GetParsingString(ResistanceRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else if(isMatch(LayerOfViaNameRegex, Data[ParsingIdx]))
            {
                innerlayer.NAME = GetParsingString(LayerOfViaNameRegex, Data[ParsingIdx] , vector<int>{1})[0] ;
            }
            else if(isMatch(LayerOfViaPointRegex, Data[ParsingIdx]))
            {
                vector<string> results = GetParsingString(LayerOfViaPointRegex, Data[ParsingIdx] , vector<int>{1,3,5,7});
                innerlayer.pt1.x = results[0];
                innerlayer.pt1.y = results[1];
                innerlayer.pt2.x = results[2];
                innerlayer.pt2.y = results[3];
                via.InnerMaps.insert(make_pair(innerlayer.NAME , innerlayer));
            }
            else
            {
                assert(0);
            }
            ParsingIdx++;
        }
        // insert into Layer Map
        ViaMaps.insert(make_pair(via.NAME, via));
        
        // free memory for regex
        regfree(&ViaRegex);
        regfree(&ResistanceRegex);
        regfree(&LayerOfViaNameRegex);
        regfree(&LayerOfViaPointRegex);
        
        
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
void TechlefParser::run()
{
    for (int i = 0 ; i < Data.size(); i++)
    {
        REGEX_TYPE RT = getMatchType(Data[i]);
        i = Parsing(RT , i );
    }
}

