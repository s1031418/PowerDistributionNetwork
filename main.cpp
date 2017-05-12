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
#include <memory>
#include "Converter.hpp"
#include <stdexcept>
#include <array>
using namespace std ;

string exec(const char* cmd);
char ** getlefargv(char * argv[]);
char ** getdefargv(char * argv[]);
void freeargv(char * lefargv[] , char * defargv[]);
void runlef(char * argv[]);
void rundef(char * argv[]);
// Ex. Team1.exe case1.v case1_input.def tech.lef blocks.lef initial_files

int main(int argc,  char * argv[])
{
    // declare objects
    InitialFileParser initialfile(argv[5]) ;
    defrw def ;
    lefrw lef ;
    // get lef、def argument value
    char ** lefargv = getlefargv(argv) ;
    char ** defargv = getdefargv(argv) ;
    // acivate parser
    lef.run(3, lefargv);
    def.run(2, defargv);
    initialfile.run();
    
//    Debugger db ;
//    cout << "tech lef file" << endl;
//    db.printAllLayerMsg(LayerMaps);
//    db.printAllViaMsg(ViaMaps);
//    cout << endl;
//    cout << "Block lef file" << endl;
//    db.printAllMacroMsg(MacroMaps);
//    cout << endl ;
//    cout << "Def file" << endl;
//    db.printDieArea(DIEAREA);
//    db.printAllComponentMsg(ComponentMaps);
//    db.printAllPinMsg(PinMaps);
//    db.printAllSpecialNetMsg(SpecialNetsMaps);
    
//    cout << "initial File" << endl ;
//    db.printAllCurrentMsg(CurrnetMaps);
//    db.printAllVoltageMsg(VoltageMaps);
//    db.printAllConstraintMsg(ConstraintMaps);
//    db.printAllWeightMsg(WeightsMaps);
    
    Converter converter ;
    converter.toLocationFile();
    
//    converter.toSpice();
    delete[] lefargv;
    delete[] defargv ;
    return 0;
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

