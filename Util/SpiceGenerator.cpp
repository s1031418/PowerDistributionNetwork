//
//  SpiceGenerator.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 16/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "SpiceGenerator.hpp"





SpiceGenerator::SpiceGenerator()
{
    
}
SpiceGenerator::~SpiceGenerator()
{
    
}

void SpiceGenerator::setSpiceName(string spiceName)
{
    outputfileName = spiceName ; 
}


void SpiceGenerator::addSpiceResistance(string vdd , string node1 , string node2 , double resistance)
{
    if( SpiceContainer.find(vdd) == SpiceContainer.end() )
    {
        cout << "You must be initialize vdd first" << endl; 
        return ;
    }
    ResistanceLine resistanceLine ;
    resistanceLine.node1 = node1 ;
    resistanceLine.node2 = node2 ;
    resistanceLine.resistance = resistance ;
    SpiceContainer[vdd].resistanceSet.push_back(resistanceLine);
}
void SpiceGenerator::initSpiceVdd(string vdd , string node , double voltage)
{
    if( SpiceContainer.find(vdd) != SpiceContainer.end() )
    {
        cout << "You have already own vdd" << endl; 
        return ;
    }
    SpiceModel spiceModel ;
    VoltageLine voltageLine ;
    voltageLine.node = node ;
    voltageLine.voltage = voltage ;
    spiceModel.voltageLine.push_back(voltageLine);
    SpiceContainer.insert(make_pair(vdd, spiceModel));
}
void SpiceGenerator::addMultiVdd(string vdd , string node , double voltage)
{
    auto iterator = SpiceContainer.find(vdd);
    VoltageLine voltageLine ;
    voltageLine.node = node ;
    voltageLine.voltage = voltage ;
    iterator->second.voltageLine.push_back(voltageLine);
}
void SpiceGenerator::addSpiceCurrent(string vdd , string node , double current )
{
    if( SpiceContainer.find(vdd) == SpiceContainer.end() )
    {
        cout << "You must be initialize vdd first" << endl;
        return ;
    }
    CurrentLine currentLine ;
    currentLine.node = node ;
    currentLine.current = current ;
    bool insert = true ;
    for(auto cl : SpiceContainer[vdd].currentSet)
    {
        if( cl.node == node )
            insert = false ;
    }
    if( insert ) SpiceContainer[vdd].currentSet.push_back(currentLine);
}
void SpiceGenerator::addSpiceCmd()
{
    FILE * pFile ;
    pFile = fopen(outputfileName.c_str(), "a");
    if( NULL == pFile ) printf("Failed to open file\n");
    
    fprintf(pFile, ".control\n");
    fprintf(pFile, "set noaskquit\n");
    fprintf(pFile, "run\n");
    fprintf(pFile, "quit\n");
    fprintf(pFile, ".enddc\n");
    fclose(pFile);
}
const SpiceGenerator & SpiceGenerator::operator=(const SpiceGenerator & right  )
{
    if( this != &right )
    {
        this->outputfileName = right.outputfileName ;
        this->SpiceContainer = right.SpiceContainer ;
    }
    return *this ;
}
void SpiceGenerator::toSpice()
{
    FILE * pFile ;
    pFile = fopen(outputfileName.c_str(), "w");
    if( NULL == pFile ) printf("Failed to open file\n");
    fprintf(pFile, "#Comments\n");
    int resistanceCount = 1 ;
    int currentCount = 1;
    int vddCount = 1 ;
    for( auto Spice : SpiceContainer )
    {
        // vdd
        for(auto vdd : Spice.second.voltageLine)
        {
            fprintf(pFile, "V_%s_%d %s gnd %g\n" , Spice.first.c_str() ,vddCount ,  vdd.node.c_str() , vdd.voltage);
            vddCount++;
//            fprintf(pFile, "V_%s_%d %s gnd %g\n" , Spice.first.c_str() , Spice.second.voltageLine.node.c_str() , Spice.second.voltageLine.voltage);
        }
        
        // resistance
        for( auto resistance : Spice.second.resistanceSet )
        {
            fprintf(pFile, "R_%s_%d %s %s %g\n" , Spice.first.c_str() , resistanceCount , resistance.node1.c_str() , resistance.node2.c_str() , resistance.resistance );
            resistanceCount++;
        }
        
        // current
        for( auto current : Spice.second.currentSet )
        {
            fprintf(pFile, "I_%s_%d %s gnd %g\n" , Spice.first.c_str() , currentCount , current.node.c_str() , current.current);
            currentCount++;
        }
        fprintf(pFile, "\n" );
    }
    fprintf(pFile, ".tran 1ns 1ns\n");
    fprintf(pFile, ".end\n");
    fclose(pFile);
}
