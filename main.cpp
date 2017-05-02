//
//  main.cpp
//  EDA_Contest2017(PDN)
//
//  Created by Jun-Sheng Wu on 26/04/2017.
//  Copyright © 2017 Jun-Sheng Wu. All rights reserved.
//

#include <iostream>
#include <string>
#include "Parsers/TechlefParser.hpp"
#include "Parsers/BlocklefParser.hpp"
#include <stdlib.h>
#include <unordered_map>
#include <map>
#include <cstdio>
#include "lib/Debugger.hpp"
#include <memory>
#include <stdexcept>
#include <array>
using namespace std ;

string exec(const char* cmd) {
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

// Ex. Team1.exe case1.v case1_input.def tech.lef blocks.lef initial_files
int main(int argc, const char * argv[])
{
    Debugger db ;
    
//    string FilePath = "/Users/Jeff/Desktop/tech.lef";
//    string FilePath = "/Users/Jeff/Desktop/blocks.lef";
    TechlefParser Techlef("/Users/Jeff/Desktop/tech.lef");
    Techlef.run();
    
    
//
    
    BlocklefParser Blocklef("/Users/Jeff/Desktop/blocks.lef");
    Blocklef.run();
    db.printAllViaMsg(Techlef.ViaMaps);
    db.printAllMacroMsg(Blocklef.MacroMaps);
    
    
    
    
    
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
    
    
    return 0;
}
