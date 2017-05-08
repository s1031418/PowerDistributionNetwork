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
#include <unordered_map>
#include <map>
#include <cstdio>
#include "lib/Debugger.hpp"
#include <memory>
#include <stdexcept>
#include <array>
using namespace std ;

string exec(const char* cmd);
char ** getlefargv(char * argv[]);

// Ex. Team1.exe case1.v case1_input.def tech.lef blocks.lef initial_files

int main(int argc,  char * argv[])
{
//    for (int i = 0 ; i < argc ; i++) {
//        cout << argv[i] << endl;
//    }
//
//    lefrw lef ;
//    lef.run(3, getlefargv(argv));
    
    
    lefrw lef ;
    lef.run(3, getlefargv(argv));
//
//    cout << argc << endl;
    
    
    
    
//    lefrw lef;
//    a = new std::map<std::string,Via>;
//    lef.run(argc, argv );
//    Debugger db ;
//    db.printAllLayerMsg(LayerMaps);
//    db.printAllViaMsg(ViaMaps);
//    
//    db.printAllMacroMsg(MacroMaps);
//    db.printAllLayerMsg(LayerMaps);
//    db.printAllViaMsg(ViaMaps);
        
    
    
    
    
    
//	string filepath = argv[1];
//    Debugger db ;
//    
//    string FilePath = "/Users/Jeff/Desktop/tech.lef";
////    string FilePath = "/Users/Jeff/Desktop/blocks.lef";
//    TechlefParser Techlef(FilePath);
//    
//    Techlef.run();
//    db.printLayerMsg("METAL1", Techlef.LayerMaps);
    
    
//
    
//    BlocklefParser Blocklef("/Users/Jeff/Desktop/blocks.lef");
//    Blocklef.run();
//    db.printAllViaMsg(Techlef.ViaMaps);
//    db.printAllMacroMsg(Blocklef.MacroMaps);
    
    
    
    
    
//    TechlefParser lef(FilePath);
//    lef.run();
    
    
//    printAllViaMsg(Techlef.VM);
    
    
    //    unordered_multimap<string,string> mymm;
    //    mymm.insert(std::make_pair("B1_02", "VDD_C"));
    //    mymm.insert(std::make_pair("B1_02", "VDD_B"));
    //    mymm.insert(std::make_pair("B1_01", "VDD_B"));
    //    mymm.insert(std::make_pair("B1_01", "VDD_A"));
    //
    //
    //
    //    mymm.insert(std::make_pair("B1_01", "VDD_C"));
    //    pair<unordered_multimap<string,string>::iterator, unordered_multimap<string,string>::iterator> ret ;
    //    unordered_multimap<string,string>::iterator begin = mymm.begin() ;
    //    unordered_multimap<string,string>::iterator end = mymm.end() ;
    //    for (unordered_multimap<string,string>::iterator it = begin; it != end ; it++) {
    //        cout << it->first << " " << it->second << endl;
    //    }
    
    
    
    
    
    //    for (multimap<string,string>::iterator it = ret.first; it != ret.second; it++) {
    //        cout << it->second << endl;
    //    }
    
    
    //system("~/Desktop/test.sh");
    
//    delete a;
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
    LefInput[0] = new char [strlen(argv[0])];
    LefInput[1] = new char [strlen(argv[3])];
    LefInput[2] = new char [strlen(argv[4])];
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
