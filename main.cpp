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
#include <stdexcept>
#include <array>
using namespace std ;

string exec(const char* cmd);
char ** getlefargv(char * argv[]);
char ** getdefargv(char * argv[]);
void freeargv(char * lefargv[] , char * defargv[]);

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
    // free memory
    freeargv(lefargv,defargv);
    
    Debugger db ;
    
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
    for (int j = 0; j < strlen(argv[0]); j++) {
        LefInput[0][j] = argv[0][j];
    }
    for (int j = 0; j < strlen(argv[3]); j++) {
        LefInput[1][j] = argv[3][j];
    }
    for (int j = 0; j < strlen(argv[4]); j++) {
        LefInput[2][j] = argv[4][j];
    }
    return LefInput ;
}
char ** getdefargv(char * argv[])
{
    char ** DefInput ;
    DefInput = new char *[2];
    DefInput[0] = new char [1000];
    DefInput[1] = new char [1000];
    // initialize Lef argument value
    for (int j = 0; j < strlen(argv[0]); j++) {
        DefInput[0][j] = argv[0][j];
    }
    for (int j = 0; j < strlen(argv[2]); j++) {
        DefInput[1][j] = argv[2][j];
    }
    return DefInput ;
}
void freeargv(char * lefargv[] , char * defargv[])
{
    //deletes an inner array
    for(int i = 0; i < 3; ++i){
        delete[] lefargv[i];
    }
    for(int i = 0; i < 2; ++i){
        delete[] defargv[i];
    }
    
    //delete pointer holding array of pointers;
    delete[] lefargv;
    delete [] defargv ;
    
    
}

