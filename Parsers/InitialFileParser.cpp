//
//  InitialFileParser.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 02/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "InitialFileParser.hpp"


multimap<string,PowerPin> CurrnetMaps ;
map<string,string> VoltageMaps;
multimap<string,Constraint> ConstraintMaps ;
map<string,string> WeightsMaps;
InitialFileParser::InitialFileParser(string FilePath)
{
    Load(FilePath);
}
InitialFileParser::~InitialFileParser()
{
    
}

void InitialFileParser::Load(string FilePath)
{
    string content ;
    fstream fin(FilePath , ios::in);
    if(!fin) cerr << "Can't open file.";
    while (getline(fin,content)) {
        Data.push_back(content);
    }
    fin.close();
}
void InitialFileParser::run()
{
    for (int i = 0 ; i < Data.size(); i++)
    {
        if (Data[i].find("# The current drawn by each power pin (mA)") != string::npos)
        {
            bool end = false ;
            while (!end)
            {
                
                i++ ; // getNextLine
                // peek nextline
                if( Data[i+1].find("#") != string::npos)
                {
                    end = true ;
                }
                
                // store in data structure
                regex_t Regex ; // group 1、2、3
                if ( regcomp(&Regex, "(.*) (.*) (.*)", REG_EXTENDED) ) cerr << "Compile Error";
                if(!isMatch(Regex, Data[i]))
                {
                    if(end == true)
                        break;
                    else
                        continue ;
                }
                PowerPin powerpin ;
                vector<string> results = GetParsingString(Regex, Data[i], vector<int>{1,2,3});
                
                powerpin.NAME = results[1];
                powerpin.CURRENTDRAWN = results[2];
                CurrnetMaps.insert(make_pair(results[0], powerpin));
            }
            
        }
        else if(Data[i].find("# The voltage of each power source (V)") != string::npos)
        {
            bool end = false ;
            while (!end)
            {
                i++ ; // getNextLine
                // peek nextline
                if( Data[i+1].find("#") != string::npos)
                {
                    end = true ;
                }
                
                // store in data structure
                regex_t Regex ; // group 1、2
                if ( regcomp(&Regex, "(.*) (.*)", REG_EXTENDED) ) cerr << "Compile Error";
                if(!isMatch(Regex, Data[i]))
                {
                    if(end == true)
                        break;
                    else
                        continue ;
                }
                vector<string> results = GetParsingString(Regex, Data[i], vector<int>{1,2});
                VoltageMaps.insert(make_pair(results[0],results[1]));
            }
        }
        else if(Data[i].find("# The IR-drop constraint of each power pin (%)") != string::npos)
        {
            bool end = false ;
            while (!end)
            {
                i++ ; // getNextLine
                // peek nextline
                if( Data[i+1].find("#") != string::npos)
                {
                    end = true ;
                }
                
                // store in data structure
                regex_t Regex ; // group 1、2、3
                if ( regcomp(&Regex, "(.*) (.*) (([[:digit:]]|[.])+)", REG_EXTENDED) ) cerr << "Compile Error";
                if(!isMatch(Regex, Data[i]))
                {
                    if(end == true)
                        break;
                    else
                        continue ;
                }
                Constraint constraint ;
                
                vector<string> results = GetParsingString(Regex, Data[i], vector<int>{1,2,3});
                constraint.NAME = results[1];
                constraint.CONSTRAINT = results[2];
                ConstraintMaps.insert(make_pair(results[0], constraint));
            }
        }
        else if (Data[i].find("# The weights on metal layers") != string::npos)
        {
            bool end = false ;
            while (!end)
            {
                i++ ; // getNextLine
                // peek nextline
                if( i == Data.size()-1 )
                {
                    end = true ;
                }
                
                // store in data structure
                regex_t Regex ; // group 1、2
                if ( regcomp(&Regex, "(.*) (.*)", REG_EXTENDED) ) cerr << "Compile Error";
                if(!isMatch(Regex, Data[i]))
                {
                    if(end == true)
                        break;
                    else
                        continue ;
                }
                vector<string> results = GetParsingString(Regex, Data[i], vector<int>{1,2});
                WeightsMaps.insert(make_pair(results[0], results[1]));
            }
        }
    }
}
