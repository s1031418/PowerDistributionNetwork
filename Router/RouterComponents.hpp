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
#include <vector>
class Border
{
    std::pair<Point<unsigned>, Point<unsigned>> LeftEdge ;
    std::pair<Point<unsigned>, Point<unsigned>> RightEdge ;
    std::pair<Point<unsigned>, Point<unsigned>> TopEdge ;
    std::pair<Point<unsigned>, Point<unsigned>> BottomEdge ;
//    Point<unsigned> leftdown ;
//    Point<unsigned> rightdown ;
//    Point<unsigned> leftup ;
//    Point<unsigned> rightup ;
};



class GridComponent
{
    // 上下左右
    unsigned int capacity = 0 ;
    
    //Border border ;
};

class Grid
{
public:
    Point<int> startpoint ;
    unsigned int width;
    unsigned int length;
    //  非block 的 Grid 兩個metal都為0 
    unsigned int lowermetal = 0 ;
    unsigned int uppermetal = 0 ;
    bool overflow = false ;
    std::vector<unsigned> capacities ;
    
    // key為metal層
    //std::map<std::string,GridComponent> layers ;
};


class Coordinate3D {
    
public:
    Coordinate3D(){};
    Coordinate3D(unsigned X , unsigned Y , unsigned Z) :x(X) , y(Y) , z(Z){};
    unsigned x ;
    unsigned y ;
    unsigned z ; 
};

class Path {
public:
    Path(){};
    std::string source ; // PowerPinName
    std::pair<std::string, std::string> target ;  // ( BlockName , BlockPinName  )
    Coordinate3D sourceGrid ;
    Coordinate3D targetGrid ;
};


class GlobalSolution
{
public:
    std::string PowerSourceName ;
    std::pair<std::string, std::string> TargetName ; 
    Coordinate3D source ;
    std::vector<Coordinate3D> targets ;
    std::vector<Coordinate3D> paths ;
};

class segment {
    
public:
    std::string direction ;
    int progress ; 
};


#endif /* RouterComponents_hpp */
