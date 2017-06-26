#include "PDNNEW.hpp"
#include <stdlib.h>
#include "../Parsers/defrw.h"
#include "../Parsers/lefrw.h"
#include <queue>
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
    //add PowerPin to Line (H wire)
    for ( auto PinName : HelpMe.PinNames)
    {
        if( PinMaps[PinName].PortMaps.size()!=0)
        {
        }
        else 
        {
            Mylineinfor * ptr = new Mylineinfor;
            ptr->MetalName = PinMaps[PinName].METALNAME ;
            ptr->isPowerPinPseudo = 1 ;
            pair<Point<int>,Point<int>> getTmpPair;
            getTmpPair = HelpMe.getPowerPinCoordinate(PinMaps[PinName].STARTPOINT.x ,PinMaps[PinName].STARTPOINT.y , PinMaps[PinName].RELATIVE_POINT1 , PinMaps[PinName].RELATIVE_POINT2 , PinMaps[PinName].ORIENT );
            ptr->Width = getTmpPair.second.y - getTmpPair.first.y ;
            ptr->pt1.x = getTmpPair.first.x ;
            ptr->pt1.y = (getTmpPair.first.y + getTmpPair.second.y) / 2 ;
            ptr->pt2.x = getTmpPair.second.x;
            ptr->pt2.y = (getTmpPair.first.y + getTmpPair.second.y) / 2 ;
            ptr->isHorizontal = 1 ;
            LinesOfLayer[ptr->MetalName].push_back(ptr);
            //cout<<ptr->pt1 <<" "<<ptr->pt2 <<" "<<ptr->MetalName <<endl;
            AllLine.push_back(ptr);
        }
    }
    //add blockPin to Line ( H wire )
    for (auto it = ComponentMaps.begin(); it != ComponentMaps.end();++it )
    {
        for ( auto MacroType_it = MacroMaps[it->second.MACROTYPE].obs.InnerMaps.begin(); MacroType_it != MacroMaps[it->second.MACROTYPE].obs.InnerMaps.end() ; ++ MacroType_it )
        {
            pair<Point<int>,Point<int>>getTmpPair;
            double getLength = MacroMaps[it->second.MACROTYPE].LENGTH;
            double getWidth  = MacroMaps[it->second.MACROTYPE].WIDTH;
            getLength *= UNITS_DISTANCE;
            getWidth  *= UNITS_DISTANCE;
            getTmpPair = HelpMe.getBlockCoordinate( it->second.STARTPOINT.x ,it->second.STARTPOINT.y , int(getWidth) , int(getLength) , it->second.ORIENT );
            Mylineinfor * ptr = new Mylineinfor;
            ptr->isBlockPseudo = 1;
            ptr->MetalName = MacroType_it->second.NAME;
            ptr->Width = getTmpPair.second.y - getTmpPair.first.y ;
            ptr->pt1.x = getTmpPair.first.x ;
            ptr->pt1.y = (getTmpPair.first.y + getTmpPair.second.y) / 2 ;
            ptr->pt2.x = getTmpPair.second.x;
            ptr->pt2.y = (getTmpPair.first.y + getTmpPair.second.y) / 2 ;
            ptr->isHorizontal = 1 ;
            LinesOfLayer[ptr->MetalName].push_back(ptr);
            AllLine.push_back(ptr);
        }
    }
    for (auto it = HelpMe.BlockMaps.begin() ; it != HelpMe.BlockMaps.end();++it)
    {
        for(int i = 0 ; i < it->second.size();i++)
        {
            for(int j = 0 ; j < it->second[i].Metals.size();j++)
            {
                Mylineinfor * ptr = new Mylineinfor;
                ptr->MetalName = it->second[i].Metals[j];
                ptr->isBlockPinPseudo = 1 ;
                ptr->isHorizontal = 1 ;
                ptr->Width = it->second[i].RightUp.y - it->second[i].LeftDown.y;
                ptr->pt1.x = it->second[i].LeftDown.x ;
                ptr->pt1.y = (it->second[i].RightUp.y + it->second[i].LeftDown.y )/2;
                ptr->pt2.x = it->second[i].RightUp.x;
                ptr->pt2.y = (it->second[i].RightUp.y + it->second[i].LeftDown.y )/2;
                LinesOfLayer[ptr->MetalName].push_back(ptr);
                AllLine.push_back(ptr);
            }
        }
    }
    //for( int  i = 0 ; i < AllLine.size() ; i++)
    //{
        //cout<<AllLine[i]->pt1<<" "<<AllLine[i]->pt2<<" "<<AllLine[i]->MetalName<<" "<<AllLine[i]->Width<<endl;
    //}
    //for(auto it = LinesOfLayer.begin();it != LinesOfLayer.end();++it)
    //{
        //for(int i = 0 ; i < it->second.size();i++)
            //cout<<it->second[i]->pt1<<" "<<it->second[i]->pt2<<" "<<it->second[i]->MetalName<<endl;
    //}
}
void PDNNEW::freeMem()
{
    for(int i = AllLine.size()-1 ; i >=0 ; i--)
        delete AllLine[i];
}
void PDNNEW::initDataByBFS()
{
    //initial is Traversal
    //Time Complex is O^2
    queue <Mylineinfor*> Queue;
    for(int i = 0 ; i < AllLine.size();i++)
    {
        if(AllLine[i]->isBlockPseudo)
            AllLine[i]->isTraversal = 1 ;
        else 
            AllLine[i] = 0 ;
        if(AllLine[i]->isPowerPinPseudo)
            Queue.push(AllLine[i]);
    }
    while(Queue.size() != 0)
    {
        if(Queue.front()->isTraversal)
            Queue.pop();
        for ( int i = 0 ; i < AllLine.size();i++ )
        {
            //if( AllLine[i]-> )
        }
        Queue.front()->isTraversal = 1 ;
        Queue.pop();
    }
}
bool PDNNEW::isCross(Mylineinfor* line_1 , Mylineinfor* line_2)
{
    Point<int>l_1_mid;
    Point<int>l_2_mid;
    if(!line_1->isPsedoLine)
    {
        l_1_mid.x = (line_1->pt1.x + line_1->pt2.x ) / 2 ;
        l_1_mid.y = (line_1->pt1.y + line_1->pt2.y ) / 2 ;
    }
    if(!line_2->isPsedoLine)
    {
        l_2_mid.x = (line_2->pt1.x + line_2->pt2.x ) / 2 ;
        l_2_mid.y = (line_2->pt1.y + line_2->pt2.y ) / 2 ;
    }
    int l1_x_r;
    int l1_y_r;
    int l2_x_r;
    int l2_y_r;
    if( line_1->isHorizontal&&!line_1->isPsedoLine ) // line_1 is H wire && != via
    {
        l1_x_r = ( line_1->pt2.x - line_1->pt1.x ) / 2 ;
        l1_y_r = ( line_1->Width ) / 2 ;
    }
    else if( !line_1->isHorizontal&&!line_1->isPsedoLine )  //line_1 is V wire && != via
    {
        l1_x_r = ( line_1->Width / 2 ) ;
        l1_y_r = ( line_1->pt2.y - line_1->pt1.y ) / 2 ;
    }
    if( line_2->isHorizontal&&!line_2->isPsedoLine ) // line_2 is H wire && != via
    {
        l2_x_r = ( line_2->pt2.x - line_2->pt1.x  ) / 2 ;
        l2_y_r = ( line_2->Width ) / 2 ;
    }
    else if( !line_2->isHorizontal&&!line_2->isPsedoLine) //line_2 is V wire
    {
        l2_x_r = ( line_2->Width / 2 ) ;
        l2_y_r = ( line_2->pt2.y - line_2->pt1.y ) / 2 ;
    }
    if(line_1->isPsedoLine)
    {
        l_1_mid = line_1->pt1 ;
        l1_x_r = line_1->ViaOutsideWidth;
        l1_y_r = line_1->ViaOutsideWidth;
    }
    if(line_2->isPsedoLine)
    {
        l_2_mid = line_2->pt1 ;
        l2_x_r = line_2->ViaOutsideWidth;
        l2_y_r = line_2->ViaOutsideWidth;
    }
    int V_Distance  = abs(l_2_mid.x - l_1_mid.x);
    int H_Distance  = abs(l_2_mid.y - l_1_mid.y);
    int V_Threshold = l1_x_r + l2_x_r;
    int H_Threshold = l1_y_r + l2_y_r;
    
    if (V_Distance > V_Threshold || H_Distance > H_Threshold )
        return false ;
    return true ;
}
