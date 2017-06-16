//
//  PDN.cpp
//  EDA_Contest2017(PDN)
//
//

#include "PDN.hpp"

#include "../Parsers/defrw.h"
#include "../Parsers/lefrw.h"


PDN::PDN()
{
    PDNHelper myHelper;
    for ( auto PinName : myHelper.PinNames)
    {
        vector < Line >  vec_special_net_line;
        for ( auto it = SpecialNetsMaps[PinName].NETSMULTIMAPS.begin() ; it != SpecialNetsMaps[PinName].NETSMULTIMAPS.end();it = SpecialNetsMaps[PinName].NETSMULTIMAPS.upper_bound(it->first))
        {
            auto first = SpecialNetsMaps[PinName].NETSMULTIMAPS.lower_bound( it->second.METALNAME);
            auto last = SpecialNetsMaps[PinName].NETSMULTIMAPS.upper_bound(it->second.METALNAME);

            while (first != last)
            {
                Line line(first->second.ABSOLUTE_POINT1,first->second.ABSOLUTE_POINT2) ;
                line.Width = first->second.ROUNTWIDTH;
                if(myHelper.isHorizontal(line.pt1,line.pt2))
                    line.isHorizontal = true ;
                else 
                    line.isHorizontal = false;
                if(line.Width==0) // if Via 
                    line.isPsedoLine =true;
                else 
                    line.isPsedoLine = false ;
                vec_special_net_line.push_back(line);
                first ++ ;
            }


        }
        vector < Line >OrderLine;
        //get Which is Start Line 
        pair < Point <int> ,Point<int>>getPowerPinRegion;
        getPowerPinRegion = myHelper.getPowerPinCoordinate(PinMaps[PinName].STARTPOINT.x,PinMaps[PinName].STARTPOINT.y , PinMaps[PinName].RELATIVE_POINT1, PinMaps[PinName].RELATIVE_POINT2 ,PinMaps[PinName].ORIENT);
        
        for (int i = 0 ; i < vec_special_net_line.size();i++)
        {
            if (myHelper.isCrossWithCoordinate( vec_special_net_line[i] ,getPowerPinRegion ) )  
                cout << vec_special_net_line[i]<<endl;
        }

        for ( int i = 0 ; i < vec_special_net_line.size();i++)
        {
            for( int  j = 0 ; j < vec_special_net_line.size();j++)
            {
                if(i==j)
                    continue;
                else 
                {
                    if (myHelper.isCross(vec_special_net_line[i],vec_special_net_line[j]))
                    {
                    }
                }
            }
        }
        cout<<endl;
    }
}


PDN::~PDN()
{

}

