//
//  PDNHelper.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 20/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//
#include "PDNHelper.hpp"


PDNHelper::PDNHelper()
{
    for( auto it = PinMaps.begin(), end = PinMaps.end(); it != end;it = PinMaps.upper_bound(it->first))
    {
        PinNames.push_back(it->first);
    }
    InitBlockMaps();
    InitPowerMaps();
}
PDNHelper::~PDNHelper()
{
    
}
bool PDNHelper::isHorizontal(Point<int> pt1 , Point<int> pt2)
{
    if( pt1.y == pt2.y)
        return true ;
    else
        return false ;
}
string PDNHelper::NETSMULTIMAPSToString(multimap<string,Nets> & NETSMULTIMAPS)
{
    bool first = true ;
    string line;
    for( auto it = NETSMULTIMAPS.begin(), end = NETSMULTIMAPS.end(); it != end;it = NETSMULTIMAPS.upper_bound(it->first))
    {
        auto beginning = NETSMULTIMAPS.lower_bound(it->first);
        auto ending = NETSMULTIMAPS.upper_bound(it->first);
        while (beginning != ending)
        {
            if(first)
            {
                line.append("   + FIXED ");
                first = false ;
            }
            else
                line.append(" NEW ");
//            line.append(beginning->second.STATE).append(" ");
            line.append(" ").append(beginning->second.METALNAME).append(" ").append(to_string(beginning->second.ROUNTWIDTH)).append(" ");
            
            if( beginning->second.ROUNTWIDTH != 0 )
            {
                line.append(" + SHAPE ").append(beginning->second.SHAPE).append(" ( ");
                line.append(to_string(beginning->second.ABSOLUTE_POINT1.x)).append(" ");
                line.append(to_string(beginning->second.ABSOLUTE_POINT1.y)).append(" ) ");
                line.append("( ");
                line.append(to_string(beginning->second.ABSOLUTE_POINT2.x)).append(" ");
                line.append(to_string(beginning->second.ABSOLUTE_POINT2.y)).append(" )\n");
            }
            else
            {
                line.append(" ( ");
                line.append(to_string(beginning->second.ABSOLUTE_POINT1.x)).append(" ");
                line.append(to_string(beginning->second.ABSOLUTE_POINT1.y)).append(" ) ");
                line.append(beginning->second.VIANAME).append("\n");
            }
            beginning++;
        }
    }
    line.pop_back();
//    cout << line << endl;
    return line ;
}
string PDNHelper::DestinationMapToString(multimap<string,string> & DestinationMap)
{
    string line ;
    for( auto it = DestinationMap.begin(), end = DestinationMap.end(); it != end;it = DestinationMap.upper_bound(it->first))
    {
        auto beginning = DestinationMap.lower_bound(it->first);
        auto ending = DestinationMap.upper_bound(it->first);
        while (beginning != ending)
        {
            line.append(" ( ").append(beginning->first).append(" ").append(beginning->second).append(" ) ");
            beginning++;
        }
    }
    return line ; 
}

void PDNHelper::InitBlockMaps()
{
    for( auto component : ComponentMaps )
    {
        Point<int> BlockLeftDown = component.second.STARTPOINT ;
        Point<int> BlockRightUp ;
        BlockRightUp.x = BlockLeftDown.x + (MacroMaps[component.second.MACROTYPE].WIDTH * UNITS_DISTANCE);
        BlockRightUp.y = BlockLeftDown.y + (MacroMaps[component.second.MACROTYPE].LENGTH * UNITS_DISTANCE);
        string orient = component.second.ORIENT ;
        vector<Block> temp ;
        // init BlockMap ， 把每個key都塞一個空的vector
        BlockMaps.insert(make_pair(component.first,temp));
        for( auto blockpin : MacroMaps[component.second.MACROTYPE].BlockPinMaps)
        {
            
            Block block ;
            for(auto innerlayer : blockpin.second.InnerMaps)
            {
                block.Metals.push_back(innerlayer.first);
            }
            Point<float> LeftDownScaling = (blockpin.second.InnerMaps[block.Metals[0]].pt1) * UNITS_DISTANCE ;
            Point<float> RightUpScaling = (blockpin.second.InnerMaps[block.Metals[0]].pt2) * UNITS_DISTANCE ;
            Point<int> BlockPinLeftDown , BlockPinRightUp;
            BlockPinLeftDown.x = BlockLeftDown.x + LeftDownScaling.x ;
            BlockPinLeftDown.y = BlockLeftDown.y + LeftDownScaling.y ;
            BlockPinRightUp.x = BlockLeftDown.x + RightUpScaling.x ;
            BlockPinRightUp.y = BlockLeftDown.y + RightUpScaling.y ;
            pair<Point<int>, Point<int>> RotatePoint = getRotatePoint(BlockLeftDown , BlockRightUp , BlockPinLeftDown, BlockPinRightUp, orient);
            block.LeftDown = get<0>(RotatePoint) ;
            block.RightUp = get<1>(RotatePoint);
            block.BlockPinName = blockpin.second.Name;
            // 不考慮BlockPin有在端點
            // 如果需要考慮判斷對哪邊開口比較大，DIRECTION就為哪個方向
            if( block.LeftDown.x == BlockLeftDown.x ) block.Direction = LEFT ;
            if( block.RightUp.x == BlockRightUp.x ) block.Direction = RIGHT ;
            if( block.LeftDown.y == BlockLeftDown.y ) block.Direction = DOWN ;
            if( block.RightUp.y == BlockRightUp.y ) block.Direction = TOP ;
            BlockMaps[component.first].push_back(block);
        }
        
    }
    
    // Print BlockMap
//    for( auto x : BlockMaps )
//    {
//        for( auto y : x.second )
//        {
//            cout << x.first << " " ;
//            cout << y.BlockPinName << endl;
//        }
//    }
}
pair< Point<int>, Point<int> > PDNHelper::getPowerPinCoordinate(int x , int y , Point<int> r_pt1, Point<int> r_pt2  , string orient)
{
    Point<int> pt1 ;
    Point<int> pt2 ;
    Point<int> LeftDown;
    Point<int> RightUp;
    if (orient == "E")
    {
        pt1.x = x + r_pt1.y ;
        pt1.y = y - r_pt1.x ;
        pt2.x = x + r_pt2.y ;
        pt2.y = y - r_pt2.x ;
    }
    else if( orient == "N")
    {
        pt1.x = x + r_pt1.x ;
        pt2.x = x + r_pt2.x ;
        pt1.y = y + r_pt1.y ;
        pt2.y = y + r_pt2.y ;
    }
    else if (orient == "S")
    {
        pt1.x = x - r_pt1.x ;
        pt2.x = x - r_pt2.x ;
        pt1.y = y - r_pt1.y ;
        pt2.y = y - r_pt2.y ;
    }
    else if (orient == "W")
    {
        pt1.x = x - r_pt1.y ;
        pt2.x = x - r_pt2.y ;
        pt1.y = y + r_pt1.x ;
        pt2.y = y + r_pt2.x ;
    }
    else if (orient == "FN")
    {
        pt1.x = x - r_pt1.x ;
        pt2.x = x - r_pt2.x ;
        pt1.y = y + r_pt1.y ;
        pt2.y = y + r_pt2.y ;
    }
    else if(orient == "FS")
    {
        pt1.x = x + r_pt1.x ;
        pt2.x = x + r_pt2.x ;
        pt1.y = y - r_pt1.y ;
        pt2.y = y - r_pt2.y ;
    }
    else if(orient == "FE")
    {
        pt1.x = x - r_pt1.y;
        pt2.x = x - r_pt2.y;
        pt1.y = y - r_pt1.x ;
        pt2.y = y - r_pt2.x ;
    }
    else if(orient == "FW")
    {
        pt1.x = x + r_pt1.y ;
        pt2.x = x + r_pt2.y ;
        pt1.y = y + r_pt1.x;
        pt2.y = y + r_pt2.x;
    }
    LeftDown.x = (pt1.x < pt2.x ) ? pt1.x : pt2.x ;
    LeftDown.y = (pt1.y < pt2.y ) ? pt1.y : pt2.y ;
    RightUp.x = (pt1.x > pt2.x ) ? pt1.x : pt2.x ;
    RightUp.y = (pt1.y > pt2.y ) ? pt1.y : pt2.y ;
    return make_pair(LeftDown, RightUp);
}
pair<Point<int>, Point<int>> PDNHelper::getRotatePoint(Point<int> BlcokLeftDown , Point<int> BlcokRightUp , Point<int> BlockPinLeftDown , Point<int> BlockPinRightUp , string orient)
{
    int BlockWidth = BlcokRightUp.x - BlcokLeftDown.x ;
    int BlockLength = BlcokRightUp.y - BlcokLeftDown.y ;
    int BlockPinWidth = BlockPinRightUp.x - BlockPinLeftDown.x ;
    int BlockPinLength = BlockPinRightUp.y - BlockPinLeftDown.y ;
    float x_axis = BlcokLeftDown.x + (BlockWidth / 2);
    float y_axis = BlcokLeftDown.y + (BlockLength / 2);
    if(orient == "N")
    {
        return make_pair(BlockPinLeftDown, BlockPinRightUp);
    }
    else if (orient == "FN")
    {
        Point<int> RightDown= FlipY(x_axis, BlockPinLeftDown , RIGHT); //變成右下角
        Point<int> LeftUp = FlipY(x_axis, BlockPinRightUp , RIGHT); //變成左上角
        return make_pair(Point<int>( LeftUp.x, RightDown.y), Point<int>( RightDown.x , LeftUp.y ));
    }
    else if (orient == "W")
    {
        int difference = BlockPinLeftDown.y - BlcokLeftDown.y ;
        pair<Point<int>, Point<int>> BlockCoordinate = getBlockCoordinate(BlcokLeftDown.x, BlcokLeftDown.y,BlockWidth , BlockLength, "W");
        Point<int> LeftDown( get<1>(BlockCoordinate).x - difference - BlockPinLength, get<0>(BlockCoordinate).y);
        Point<int> RightUp( get<1>(BlockCoordinate).x - difference, get<0>(BlockCoordinate).y + BlockPinWidth);
        return make_pair(LeftDown, RightUp);
    }
    else if (orient == "FW")
    {
        pair<Point<int>, Point<int>> W = getRotatePoint(BlcokLeftDown, BlcokRightUp, BlockPinLeftDown, BlockPinRightUp, "W");
        pair<Point<int>, Point<int>> BlockCoordinate = getBlockCoordinate(BlcokLeftDown.x, BlcokLeftDown.y,BlockWidth , BlockLength, "W");
        x_axis = get<0>(BlockCoordinate).x + BlockLength / 2 ;
        Point<int> RightDown = FlipY(x_axis, get<0>(W) , LEFT);
        Point<int> LeftUp = FlipY(x_axis, get<1>(W), LEFT);
        return make_pair(Point<int>( LeftUp.x, RightDown.y), Point<int>( RightDown.x , LeftUp.y ));
    }
    else if (orient == "S")
    {
        
        return make_pair( FlipY(x_axis, FlipX(y_axis, BlockPinRightUp, TOP), RIGHT), FlipY(x_axis, FlipX(y_axis, BlockPinLeftDown, TOP), RIGHT));
    }
    else if (orient == "FS")
    {
        pair<Point<int>, Point<int>> S = getRotatePoint(BlcokLeftDown, BlcokRightUp, BlockPinLeftDown, BlockPinRightUp, "S");
        Point<int> RightDown = FlipY(x_axis, get<0>(S), LEFT);
        Point<int> LeftUp = FlipY(x_axis, get<1>(S), LEFT);
        return make_pair(Point<int>( LeftUp.x, RightDown.y), Point<int>( RightDown.x , LeftUp.y ));
    }
    else if (orient == "E")
    {
        int difference = BlockPinLeftDown.y - BlcokLeftDown.y ;
        pair<Point<int>, Point<int>> BlockCoordinate = getBlockCoordinate(BlcokLeftDown.x, BlcokLeftDown.y,BlockWidth , BlockLength, "E");
        return make_pair(Point<int>( get<0>(BlockCoordinate).x + difference, get<1>(BlockCoordinate).y - BlockPinWidth) , Point<int>(get<0>(BlockCoordinate).x + difference + BlockPinLength , get<1>(BlockCoordinate).y));
    }
    else if (orient == "FE")
    {
        pair<Point<int>, Point<int>> E = getRotatePoint(BlcokLeftDown, BlcokRightUp, BlockPinLeftDown, BlockPinRightUp, "E");
        x_axis = BlcokLeftDown.x + BlockLength / 2 ;
        Point<int> LeftUp = FlipY(x_axis, get<1>(E), RIGHT);
        Point<int> RightDown = FlipY(x_axis, get<0>(E), RIGHT);
        return make_pair(Point<int>( LeftUp.x, RightDown.y), Point<int>( RightDown.x , LeftUp.y ));
    }
    assert(0);
    return make_pair(Point<int>(0,0), Point<int>(0,0));

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
        if( psedo_line.pt1.x >= leftX && psedo_line.pt1.x <= RightX && psedo_line.pt1.y >= BottomY && psedo_line.pt1.y <= TopY )
            return true ;
        else
            return false ;
    }
    if(line1.isHorizontal && !(line2.isHorizontal))
    {
        
        if( line1.pt2.y >= line2.pt1.y && line1.pt2.y <= line2.pt2.y && line2.pt2.x >= line1.pt1.x && line2.pt2.x <= line1.pt2.x ) // isIntersaction
            return true ;
        else
            return false ;
        
    }
    else if( !(line1.isHorizontal) && line2.isHorizontal)
    {
        if( line2.pt2.y >= line1.pt1.y && line2.pt2.y <= line1.pt2.y && line1.pt2.x >= line2.pt1.x && line1.pt2.x <= line2.pt2.x ) // isIntersaction
            return true ;
        else
            return false ;
    }
    else
        return false;
}
bool PDNHelper::isCrossWithCoordinate ( Line line , pair < Point<int> , Point<int> > pairPoint)
{
    if ( line.pt1.x >= pairPoint.first.x && line.pt1.x <= pairPoint.second.x && line.pt1.y >= pairPoint.first.y && line.pt1.y <= pairPoint.second.y )
        return true ;
    if ( line.pt2.x >= pairPoint.first.x && line.pt2.x <= pairPoint.second.x && line.pt2.y >= pairPoint.first.y && line.pt2.y <= pairPoint.second.y )
        return true ;
    return false ; 
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
Point<int> PDNHelper::getEndPoint(Point<int> pt1  , Point<int> pt2 )
{
    Point<int> temp ;
    for(auto Blocks : BlockMaps)
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
pair<string, string> PDNHelper::getBlockMsg(Point<int> pt)
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
    return make_pair(string(), string());
}
Point<int> PDNHelper::FlipY(float x_axis , Point<int> pt , DIRECTION orientation)
{
    int x = 0 , y = 0;
    if( orientation == RIGHT)
    {
        float distance = x_axis - pt.x ;
        x = pt.x + 2 * distance;
    }
    else
    {
        float distance = pt.x - x_axis  ;
        x = pt.x - 2 * distance;
    }
    y = pt.y;
    return Point<int>(x,y);
}
Point<int> PDNHelper::FlipX(float y_axis , Point<int> pt , DIRECTION orientation)
{
    
    int x = 0 , y = 0;
    if( orientation == TOP)
    {
        float distance = y_axis - pt.y ;
        y = pt.y + 2 * distance ;
    }
    else
    {
        float distance =  pt.y - y_axis;
        y = pt.y - 2 * distance ;
    }
    x = pt.x ;
    return Point<int>(x,y);
    
}

void PDNHelper::InitPowerMaps()
{
    // key pinName , value block
    for(auto pin : PinMaps)
    {
        string orient = pin.second.ORIENT ;
        Block block ;
        pair< Point<int>, Point<int> > PowerPinCoordinate = getPowerPinCoordinate(pin.second.STARTPOINT.x, pin.second.STARTPOINT.y, pin.second.RELATIVE_POINT1 , pin.second.RELATIVE_POINT2, orient);
        block.LeftDown = get<0>(PowerPinCoordinate);
        block.RightUp = get<1>(PowerPinCoordinate);
        PowerMaps.insert(make_pair(pin.first, block));
    }
}
string PDNHelper::getPowerPinMsg(Point<int> pt)
{
    for(auto pin : PowerMaps)
    {
        if( pt.x >= pin.second.LeftDown.x && pt.x <= pin.second.RightUp.x && pt.y >= pin.second.LeftDown.y && pt.y <= pin.second.RightUp.y )
        {
            return pin.first ;
        }
    }
    return string();
}
Point<int> PDNHelper::getStartPoint(Point<int> pt1 , Point<int> pt2 )
{
    
    for(auto pin : PowerMaps)
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
Block PDNHelper::getBlock( string BlockName , string BlockPinName )
{
    for(auto x : BlockMaps[BlockName])
    {
        if( x.BlockPinName == BlockPinName )
            return x ;
    }
    return Block();
}