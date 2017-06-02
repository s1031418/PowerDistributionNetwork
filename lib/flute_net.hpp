//
//  flute_net.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 02/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef flute_net_hpp
#define flute_net_hpp

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "flute.h"
#include "Components.h"
#include <iostream>
#include <iomanip>
using namespace std;

class flute_net {
    
    
public:
    flute_net();
    ~flute_net();
    
    void getSteinerTree(vector<Point<int>> Points);
    void printSteinerTree();
    
    vector<node> SteinerTree ;
    int length ;
    int length_noRSMT ; 
};

#endif /* flute_net_hpp */
