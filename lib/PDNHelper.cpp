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
bool PDNHelper::isCross(Line line1 , Line line2)
{
    // 不會同時有兩個psedo line 進來
    if( line1.isPsedoLine || line2.isPsedoLine )
    {
        Line psedo_line = (line1.isPsedoLine) ? line1 : line2;
        Line non_psedo_line = ( psedo_line == line1 ) ? line2 : line1 ;
        int leftX = isHorizontal(non_psedo_line.pt1, non_psedo_line.pt2) ? non_psedo_line.pt1.x : non_psedo_line.pt1.x - (non_psedo_line.Width / 2) ;
        int RightX = isHorizontal(non_psedo_line.pt1, non_psedo_line.pt2) ? non_psedo_line.pt2.x : non_psedo_line.pt2.x + (non_psedo_line.Width / 2) ;
        int TopY = isHorizontal(non_psedo_line.pt1, non_psedo_line.pt2) ? non_psedo_line.pt1.y + ( non_psedo_line.Width / 2 ) : non_psedo_line.pt2.y ;
        int BottomY = isHorizontal(non_psedo_line.pt1, non_psedo_line.pt2) ? non_psedo_line.pt2.y - ( non_psedo_line.Width / 2 ) : non_psedo_line.pt1.y ;
        if( psedo_line.pt1.x > leftX && psedo_line.pt1.x < RightX && psedo_line.pt1.y > BottomY && psedo_line.pt1.y < TopY )
            return true ;
        else
            return false ;
    }
    if(line1.isHorizontal && !(line2.isHorizontal))
    {
        
        if( line1.pt2.y > line2.pt1.y && line1.pt2.y < line2.pt2.y && line2.pt2.x > line1.pt1.x && line2.pt2.x < line1.pt2.x ) // isIntersaction
            return true ;
        else
            return false ;
        
    }
    else if( !(line1.isHorizontal) && line2.isHorizontal)
    {
        if( line2.pt2.y > line1.pt1.y && line2.pt2.y < line1.pt2.y && line1.pt2.x > line2.pt1.x && line1.pt2.x < line2.pt2.x ) // isIntersaction
            return true ;
        else
            return false ;
    }
    else
        return false;
}
Point<int> PDNHelper::getCrossPoint(Line line1 , Line line2)
{
    Point<int> CrossPoint ;
    if(isCross(line1, line2))
    {
        if(line1.isPsedoLine || line2.isPsedoLine)
        {
            if(line1.isPsedoLine)
                return line1.pt1 ;
            else
                return line2.pt1 ;
        }
        else
        {
            CrossPoint.x = ( !line1.isHorizontal ) ? line1.pt1.x : line2.pt1.x ;
            CrossPoint.y = ( line1.isHorizontal ) ? line1.pt1.y : line2.pt1.y ;
        }
    }
    return CrossPoint ;
}
Point<int> PDNHelper::getEndPoint(map<string , vector<Block> > & BlockMap ,Point<int> pt1  , Point<int> pt2 )
{
    Point<int> temp ;
    for(auto Blocks : BlockMap)
    {
        for(auto block : Blocks.second)
        {
            if( pt1.x >= block.LeftDown.x && pt1.x <= block.RightUp.x && pt1.y >= block.LeftDown.y && pt1.y <= block.RightUp.y )
                return block.RightUp ;
            if( pt2.x >= block.LeftDown.x && pt2.x <= block.RightUp.x && pt2.y >= block.LeftDown.y && pt2.y <= block.RightUp.y )
                return block.RightUp ;
        }
    }
    return temp;
}
double PDNHelper::calculateResistance(double rpsq , int width , double length )
{
    return rpsq * length / width ;
}
pair<string, string> PDNHelper::getBlockMsg(map<string , vector<Block> > & BlockMaps , Point<int> pt)
{
    for(auto block : BlockMaps)
    {
        for(auto blockpin : block.second)
        {
            if( pt.x >= blockpin.LeftDown.x && pt.x <= blockpin.RightUp.x && pt.y >= blockpin.LeftDown.y && pt.y <= blockpin.RightUp.y )
            {
                return make_pair(block.first, blockpin.BlockPinName);
            }
        }
    }
    assert(0);
}
Point<int> PDNHelper::getStartPoint(map<string ,Block > & PowerMap ,Point<int> pt1 , Point<int> pt2 )
{
    
    for(auto pin : PowerMap)
    {
        // 假如pt1在Powerpin得範圍內
        if( pt1.x >= pin.second.LeftDown.x && pt1.x <= pin.second.RightUp.x && pt1.y >= pin.second.LeftDown.y && pt1.y <= pin.second.RightUp.y )
        {
            return pt1 ;
        }
        // 假如pt2在Powerpin得範圍內
        if( pt2.x >= pin.second.LeftDown.x && pt2.x <= pin.second.RightUp.x && pt2.y >= pin.second.LeftDown.y && pt2.y <= pin.second.RightUp.y )
        {
            return pt2 ;
        }
    }
    return Point<int>();
}
