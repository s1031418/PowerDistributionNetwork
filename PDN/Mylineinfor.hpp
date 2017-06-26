#ifndef Mylineinfor_hpp
#define Mylineinfor_hpp
#pragma once
#include "../Parsers/Components.h"
#include <vector>
#include <string>
using namespace std;
class Mylineinfor : public Line
{
    public:
    Mylineinfor(){};
    ~Mylineinfor(){};
    float LeftOrBotV = 999 ; 
    float RightOrUpV = 999 ;
    float LeftOrBotI = 999 ;
    float RightOrUpI = 999 ;
    int  LeftOrBotRoutingResource = -1 ;
    int  RightOrUpRoutingResource = -1 ;
    int    ViaOutsideWidth = -1 ;
    int    ViaInsideWidth  = -1 ;
    bool  isPowerPinPseudo = 0 ;
    bool  isBlockPinPseudo = 0;
    vector<Mylineinfor*> fan_in  ;
    vector<Mylineinfor*> fan_out ;
};
#endif
