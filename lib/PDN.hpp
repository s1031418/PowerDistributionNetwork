//
//  PDN.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 17/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef PDN_hpp
#define PDN_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <vector>
#include "Components.h"
#include "PDNHelper.hpp"
#include <set>
using namespace std ;

class Grid
{
public:
    Point<int> StartPoint ;
    int Width;
    int Length;
    int UML ; // UpperMetalLayer
    int LML ; // LowerMetalLayer
};
class PDN
{
public:
    PDN();
    ~PDN();
    void optimize();
    void toSpice();
    void Init();
private:
    // variable:
    PDNHelper myhelper ;
    vector< vector< Grid > > Graph ;
    set<int> Vertical ;
    set<int> Horizontal ;
    vector<string> PinNames;
    
    // -----------------------------------------------------
    // function:
    void initLineVec();
    void initGraph();
};

#endif /* PDN_hpp */
