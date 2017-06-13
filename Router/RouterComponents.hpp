//
//  RouterComponents.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 13/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef RouterComponents_hpp
#define RouterComponents_hpp

#include <stdio.h>
#include "Components.h"
#include <map>
class Border
{
    Point<unsigned> leftdown ;
    Point<unsigned> rightdown ;
    Point<unsigned> leftup ;
    Point<unsigned> rightup ;
};

class GridComponent
{
    unsigned int capacity;
    bool overflow;
    Border border ;
};

class Grid
{
public:
    Point<int> startpoint ;
    unsigned int width;
    unsigned int length;
    unsigned int lowermetal = 0 ;
    unsigned int uppermetal = 0 ;
    std::map<std::string,GridComponent> layers ;
};


#endif /* RouterComponents_hpp */
