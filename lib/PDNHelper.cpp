//
//  PDNHelper.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 20/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "PDNHelper.hpp"



bool PDNHelper::isHorizontal(Point<int> pt1 , Point<int> pt2)
{
    if( pt1.y == pt2.y)
        return true ;
    else
        return false ;
}
double PDNHelper::calculateResistance(double rpsq , int width , double length )
{
    return rpsq * length / width ;
}
pair<Point<int>, Point<int>> PDNHelper::getBlockCoordinate(int x , int y , int width , int length  , string orient  )
{
    Point<int> pt1 ;
    Point<int> pt2 ;
    pt1.x = x ;
    pt1.y = y ;
    if( orient == "N" || orient == "S" || orient == "FN" || orient == "FS" )
    {
        pt2.x = x + width ;
        pt2.y = y + length ;
    }
    else if( orient == "W" || orient == "E" || orient == "FW" || orient == "FE"  )
    {
        pt2.x = x + length ;
        pt2.y = y + width ;
    }
    return make_pair(pt1, pt2);
}
