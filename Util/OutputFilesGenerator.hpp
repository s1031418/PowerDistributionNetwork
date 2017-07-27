//
//  OutputFilesGenerator.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 26/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef OutputFilesGenerator_hpp
#define OutputFilesGenerator_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "lefrw.h"
#include "defrw.h"
#include "PDNHelper.hpp"
#include "InitialFileParser.hpp"
#include <fstream>
using namespace std ;

struct IRDropLine {
    string blockName ;
    string blockPinName ;
    double dropPercentage ;
};
struct MetalUsageLine {
    string MetalName ; // M4 、 M5 ... etc 
    double metalUsage ;
};


class OutputFilesGenerator
{
public:
    OutputFilesGenerator();
    ~OutputFilesGenerator();
    
    
    void setIRDrop(string BlockName , string BlockPinName , double DropPercentage);
    void setOutputFilesName(string outputName);
    void toOutputFiles();
    
private:
    string outputfileName ;
    vector<IRDropLine> irDropLines ;
    vector<MetalUsageLine> metalUsageLines ;
    PDNHelper myhelper ;
    void calculateMetalUsage();
    Rectangle getRectangle(multimap<string,Nets>::iterator it );
};

#endif /* OutputFilesGenerator_hpp */
