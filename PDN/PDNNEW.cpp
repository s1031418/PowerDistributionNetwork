#include "PDNNEW.hpp"
#include "../Parsers/defrw.h"
#include "../Parsers/lefrw.h"
void PDNNEW::pushNetsinMyLineVector( vector <Mylineinfor*> &AllLineFormPin , Nets NET)
{
    Mylineinfor * ptr = new Mylineinfor ;
    if( NET.ROUNTWIDTH == 0 )
    {
        ptr->pt1 = NET.ABSOLUTE_POINT1;
        ptr->isPsedoLine = 1 ;
        ptr->ViaName = NET.VIANAME;
        ptr->MetalName = NET.METALNAME;
        ptr->Width = 0 ;
        int tmp = 0 ;
        int MAX = 0 ;
        for (auto it = ViaMaps[NET.VIANAME].InnerMaps.begin();it!=ViaMaps[NET.VIANAME].InnerMaps.end();++it)
        {
            tmp = it->second.pt2.y ;
            if (tmp > MAX)
                MAX = tmp;
        }
        tmp = 0;
        int MIN = 999;
        for (auto it = ViaMaps[NET.VIANAME].InnerMaps.begin();it!=ViaMaps[NET.VIANAME].InnerMaps.end();++it)
        {
            tmp = it->second.pt2.y ;
            if (tmp < MIN)
                MIN = tmp;
            if (it->second.NAME.compare(NET.METALNAME)==0 )
                continue;
            if (it->second.NAME[0]!='M'&&it->second.NAME[1]!='E'&&it->second.NAME[2]!='T')
                continue;
            ptr->ViaMetal.push_back(it->second.NAME);
        }
        ptr->ViaMetal.push_back(ptr->MetalName);
        ptr->ViaOutsideWidth = MAX * UNITS_DISTANCE ;
        ptr->ViaInsideWidth = MIN * UNITS_DISTANCE ; 
        AllLineFormPin.push_back(ptr);
        return ;
    }
    if(HelpMe.isHorizontal(NET.ABSOLUTE_POINT1 , NET.ABSOLUTE_POINT2))
    {
        Point<int> right  = ( NET.ABSOLUTE_POINT1.x > NET.ABSOLUTE_POINT2.x  ) ? NET.ABSOLUTE_POINT1 : NET.ABSOLUTE_POINT2 ;
        Point<int> left   = ( NET.ABSOLUTE_POINT1.x < NET.ABSOLUTE_POINT2.x  ) ? NET.ABSOLUTE_POINT1 : NET.ABSOLUTE_POINT2 ;
        ptr->pt1 = left ;
        ptr->pt2 = right ;
        ptr->isHorizontal = 1 ;
        ptr->MetalName = NET.METALNAME;
        ptr->Width = NET.ROUNTWIDTH;
        AllLineFormPin.push_back(ptr);
        return ;
    }
    else
    {
        Point<int> top   = ( NET.ABSOLUTE_POINT1.y > NET.ABSOLUTE_POINT2.y  ) ? NET.ABSOLUTE_POINT1 : NET.ABSOLUTE_POINT2 ;
        Point<int> bot   = ( NET.ABSOLUTE_POINT1.y < NET.ABSOLUTE_POINT2.y  ) ? NET.ABSOLUTE_POINT1 : NET.ABSOLUTE_POINT2 ;
        ptr->pt1 = bot;
        ptr->pt2 = top;
        ptr->isHorizontal = 0 ;
        ptr->MetalName = NET.METALNAME;
        ptr->Width = NET.ROUNTWIDTH;
        AllLineFormPin.push_back(ptr);
        return;
    }
    return;
}
void PDNNEW::getRoutingResource(   )
{
}

void PDNNEW::initGraphRootMap()
{
    //add PowerPin
    for(auto PinName : HelpMe.PinNames)
    {

    }
    //add BlockPin

}
void PDNNEW::initLineInLayer()
{
    for(auto PinName : HelpMe.PinNames)
    {
        for ( auto it = SpecialNetsMaps[PinName].NETSMULTIMAPS.begin(); it != SpecialNetsMaps[PinName].NETSMULTIMAPS.end(); it = SpecialNetsMaps[PinName].NETSMULTIMAPS.upper_bound(it->first)  )
        {
            auto first = SpecialNetsMaps[PinName].NETSMULTIMAPS.lower_bound(it->second.METALNAME);
            auto last = SpecialNetsMaps[PinName].NETSMULTIMAPS.upper_bound(it->second.METALNAME);
            while(first != last )
            {
                pushNetsinMyLineVector( AllLine , first->second );
                first++;
            }
        }
    }
    for ( int i = 0 ; i < AllLine.size();i++ )
        LinesOfLayer[AllLine[i]->MetalName].push_back(AllLine[i]);
}
void PDNNEW::initDataByBFS()
{
}
