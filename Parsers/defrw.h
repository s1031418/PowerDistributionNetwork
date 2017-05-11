//
//  defrw.h
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 08/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef defrw_h
#define defrw_h


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifndef WIN32
#   include <unistd.h>
#endif /* not WIN32 */
#include "defrReader.hpp"
#include "defiAlias.hpp"
#include "Components.h"
extern double UNITS_DISTANCE ;
extern Diearea DIEAREA ;
// key:BlockName , value: Components
extern std::map<std::string,Components> ComponentMaps ;
// key:PinName , value:Pin
extern std::map<std::string,Pin> PinMaps;
// key:PinName , value SpectialNet
extern std::map<std::string,SpecialNets> SpecialNetsMaps ;


class defrw {
    
    
public:
    defrw();
    ~defrw();
    int run(int argc , char ** argv);
};

#endif /* defrw_h */
