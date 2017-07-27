//
//  ngspice.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 22/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "ngspice.hpp"

ngspice::ngspice()
{
    LoadFile();
}
void ngspice::LoadFile()
{
    string content ;
    string FilePath;
    fstream fin("simulation" , ios::in);
    if(!fin)
    {
        cerr << "Can't open ngspice file.";
        exit(1);
    }
    while (getline(fin,content)) {
        Data.push_back(content);
    }
    fin.close();
}
ngspice::~ngspice()
{
    
}
void ngspice::initvoltage()
{
    voltages.clear();
    bool start = false ;
    for (int i = 0 ; i < Data.size(); i++)
    {
        if(Data[i].find("branch") != string::npos)
        {
            break;
        }
        if(start)
        {
            regex_t Regex ; // group 1、3
            if ( regcomp(&Regex, "(([[:alnum:]]|_|[.])+)[[:space:]]+(-?([[:alnum:]]|_|[.])+)", REG_EXTENDED) ) cerr << "Compile Error";
            vector<string> results = GetParsingString(Regex, Data[i], vector<int>{1,3});
            voltages.insert(make_pair(results[0], stod(results[1])));
            regfree(&Regex);
            continue;
        }
        if( Data[i].find("Node") != string::npos )
        {
            start = true ;
            i += 1 ;
        }
    }
}
