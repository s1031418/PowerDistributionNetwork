//
//  RouterUtil.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 13/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "RouterUtil.hpp"




RouterUtil::RouterUtil()
{
    InitBlockMap();
}
void RouterUtil::InitBlockMap()
{
    for( auto component : ComponentMaps )
    {
        pair<Point<int>,Point<int>> coordinate = this->getBlockCoordinate(component.second.STARTPOINT.x, component.second.STARTPOINT.y, MacroMaps[ component.second.MACROTYPE ].WIDTH * UNITS_DISTANCE, MacroMaps[ component.second.MACROTYPE ].LENGTH * UNITS_DISTANCE, component.second.ORIENT);
        BlockMap.insert(make_pair(component.second.MACROTYPE, coordinate));
    }
}

RouterUtil::~RouterUtil()
{
    
}

bool RouterUtil::IsBlock(Grid grid)
{
    if( grid.lowermetal == 0 && grid.uppermetal == 0 )
        return false ;
    else
        return true ;
}
int RouterUtil::translateMetalNameToInt(string MetalName)
{
    if( MetalName == "METAL10" ) return 10;
    if( MetalName == "METAL9" ) return 9;
    if( MetalName == "METAL8" ) return 8;
    if( MetalName == "METAL7" ) return 7;
    if( MetalName == "METAL6" ) return 6;
    if( MetalName == "METAL5" ) return 5;
    if( MetalName == "METAL4" ) return 4;
    if( MetalName == "METAL3" ) return 3;
    if( MetalName == "METAL2" ) return 2;
    if( MetalName == "METAL1" ) return 1;
    assert(0);
}
pair<bool, string> RouterUtil::IsBlock(Point<int> LeftDown , Point<int> RightUp)
{
    pair<bool, string> result ;
    get<0>(result) = false;
    get<1>(result) = "";
    for(auto block : BlockMap)
    {
        if( LeftDown.x >= block.second.first.x
           && RightUp.x <= block.second.second.x
           && LeftDown.y >= block.second.first.y
           && RightUp.y <= block.second.second.y )
        {
            get<0>(result) = true;
            get<1>(result) = block.first;
        }
    }
    return result;
}
