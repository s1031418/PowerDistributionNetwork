//
//  verilog.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 05/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef verilog_hpp
#define verilog_hpp

#include <stdio.h>
#include "Parsers.hpp"
#include <map>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct BlockInfo {
    string BlockName ;
    string BlockPinName;
};

extern multimap< string, BlockInfo > Connection ;

class verilog : public Parsers
{
    
public:
    verilog(string FilePath);
    ~verilog();
    void run();
private:
    vector<string> Data ;
    void Load(string FilePath);
    
    void ParseSubBlock(string BlockName , string BlockMsg);
};

#endif /* verilog_hpp */
