//
//  DefGenerator.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 16/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef DefGenerator_hpp
#define DefGenerator_hpp

#include <stdio.h>
#include <string> 
#include "lefrw.h"
#include "defrw.h"
#include "PDNHelper.hpp"
using namespace std ; 
class DefGenerator {
    
public:
    DefGenerator();
    ~DefGenerator();
    void setDefName(string def);
    void toOutputDef();
private:
    string outputfileName ;
    PDNHelper myhelper ;
};

#endif /* DefGenerator_hpp */
