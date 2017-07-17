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


const unsigned DEFAULT_WIDTH = 10 ;
const unsigned DEFAULT_PITCH = 12 ;


class Border
{
public:
    // edge avaiable
    bool leftEdge = true;
    bool rightEdge = true ;
    bool upEdge = true  ;
    bool downEdge = true ;
};

class VerticalBorder
{
public:
    bool topEdge = true ;
    bool bottomEdge = true ;
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
    std::vector<Border> Edges ;
    std::vector<VerticalBorder> verticalEdges ; 
    std::vector<Rectangle> obstacles;
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
    bool operator==(const Coordinate3D & right)
    {
        return (this->x == right.x && this->y == right.y && this->z == right.z);
    }
    const Coordinate3D & operator=(const Coordinate3D & right  )
    {
        if( this != &right )
        {
            this->x = right.x ;
            this->y = right.y ;
            this->z = right.z ;
        }
        return *this ;
    }
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
enum Direction3D
{
    upOrient ,
    downOrient ,
    leftOrient ,
    rightOrient ,
    topOrient ,
    bottomOrient
};



#endif /* RouterComponents_hpp */
