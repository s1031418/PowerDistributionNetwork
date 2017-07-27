//
//  DefGenerator.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 16/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "DefGenerator.hpp"



DefGenerator::DefGenerator()
{
    
}



DefGenerator::~DefGenerator()
{
    
}
void DefGenerator::setDefName(string def)
{
    outputfileName = def ;
}
void DefGenerator::toOutputDef()
{
    int StartIndex = 0 ;
    int EndIndex = 0 ;
    int Rowindex = 0 ;
    vector<string> temp = OriginDef ;
    
    for(auto line : temp)
    {
        if( line.find("SPECIALNETS") != string::npos )
        {
            if(StartIndex == 0)
            {
                StartIndex = Rowindex ;
                Rowindex++;
                continue;
            }
            if(EndIndex == 0)
                EndIndex = Rowindex ;
            if(EndIndex != 0)
                break;
        }
        Rowindex++;
    }
    // vector 用 erase 要注意 performace
    if( StartIndex != 0 && EndIndex != 0 ) temp.erase(temp.begin() + StartIndex, temp.begin()+EndIndex + 1 );
    if( StartIndex == 0 )
    {
        Rowindex = 0 ;
        for(auto line : temp)
        {
            if( line.find("PINS") != string::npos )
            {
                if(StartIndex == 0)
                {
                    StartIndex = Rowindex ;
                    Rowindex++;
                    continue;
                }
                if(EndIndex == 0)
                    EndIndex = Rowindex ;
                if(EndIndex != 0)
                    break;
            }
            Rowindex++;
        }
        StartIndex = EndIndex + 1 ; 
    }
    
    vector<string> NewDefSp;
    NewDefSp.push_back("SPECIALNETS " + to_string(SpecialNetsMaps.size()) + " ;");
    
    for( auto nets : SpecialNetsMaps )
    {
        string line = "- " + nets.first + " " ;
        line.append(myhelper.DestinationMapToString(nets.second.DESTINATIONMAPS));
        NewDefSp.push_back(line);
        NewDefSp.push_back(myhelper.NETSMULTIMAPSToString(nets.second.NETSMULTIMAPS));
        NewDefSp.push_back(" + USE " + nets.second.USE );
        NewDefSp.push_back(" ;");
    }
    for(auto x : NewDefSp)
    {
        temp.insert(temp.begin() + StartIndex, x);
        StartIndex++ ;
    }
    temp.insert(temp.begin() + StartIndex, "END SPECIALNETS");
    fstream fout(outputfileName , ios::out);
    for( auto x : temp )
        fout << x << endl;
}
