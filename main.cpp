//
//  main.cpp
//  EDA_Contest2017(PDN)
//
//  Created by Jun-Sheng Wu on 26/04/2017.
//  Copyright © 2017 Jun-Sheng Wu. All rights reserved.
//

#include <iostream>
#include <string>
#include "lefrw.h"
#include <stdlib.h>
#include "defrw.h"
#include <unordered_map>
#include <map>
#include "InitialFileParser.hpp"
#include <cstdio>
#include "lib/Debugger.hpp"
#include "Converter.hpp"
#include <memory>
#include "PDN.hpp"
#include <stdexcept>

#include <array>
using namespace std ;

string exec(const char* cmd);
char ** getlefargv(char * argv[]);
char ** getdefargv(char * argv[]);
void freeargv(char * lefargv[] , char * defargv[]);
void runlef(char * argv[]);
void by(int num , std::multimap<std::string,Nets> & NETSMULTIMAPS);
void test(int num);
void rundef(char * argv[]);
// Ex. Team1.exe case1.v case1_input.def tech.lef blocks.lef initial_files caseX

int main(int argc,  char * argv[])
{
    string CaseName = argv[6];
    InitialFileParser initialfile(argv[5]) ;
    char ** lefargv = getlefargv(argv) ;
    char ** defargv = getdefargv(argv) ;
    defrw def(2, defargv) ;
    lefrw lef(3, lefargv);
    lef.run();
    def.run();
    initialfile.run();
    Converter converter(CaseName);
//    Debugger db ;
//    db.printAllSpecialNetMsg(SpecialNetsMaps);
    
    for(int i = 0 ; i < 3 ; i++)
    {
        converter.toLocationFile();
        converter.Visualize();
        test(2);
    }
    converter.toOutputDef();
    
    
    
    delete[] lefargv;
    delete[] defargv ;
    return 0;
}
void test(int num)
{
    vector<string> PinNames;
    for( auto it = PinMaps.begin(), end = PinMaps.end(); it != end;it = PinMaps.upper_bound(it->first))
    {
        PinNames.push_back(it->first);
    }
    
    for(auto PinName : PinNames)
    {
        by(num , SpecialNetsMaps[PinName].NETSMULTIMAPS);
    }
    
}
void by(int num , std::multimap<std::string,Nets> & NETSMULTIMAPS)
{
    for( auto it = NETSMULTIMAPS.begin(), end = NETSMULTIMAPS.end(); it != end;it = NETSMULTIMAPS.upper_bound(it->first))
    {
        auto first = NETSMULTIMAPS.lower_bound(it->first);
        auto last = NETSMULTIMAPS.upper_bound(it->first);
        while (first != last) {
            first->second.ROUNTWIDTH *= num ;
            first++;
        }
    }
}
string exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            result += buffer.data();
    }
    return result;
}
char ** getlefargv(char * argv[])
{
    // new memory allocation
    char ** LefInput ;
    LefInput = new char *[3];
    LefInput[0] = new char [1000];
    LefInput[1] = new char [1000];
    LefInput[2] = new char [1000];
    // initialize Lef argument value
    LefInput[0] = argv[0];
    LefInput[1] = argv[3];
    LefInput[2] = argv[4];
    return LefInput ;
}
char ** getdefargv(char * argv[])
{
    char ** DefInput ;
    DefInput = new char *[2];
    DefInput[0] = new char [1000];
    DefInput[1] = new char [1000];
    // initialize Lef argument value
    DefInput[0] = argv[0];
    DefInput[1] = argv[2];
    return DefInput ;
}

