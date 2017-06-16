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
                line.MetalName = first->second.METALNAME;
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
        //for(int i = 0 ; i < vec_special_net_line.size();i++)
            //cout<<"line : "<<vec_special_net_line[i]<<endl;
        vector < Line >OrderLine;
        //get Which is Start Line 
        pair < Point <int> ,Point<int> > getPowerPinRegion;
        getPowerPinRegion = myHelper.getPowerPinCoordinate(PinMaps[PinName].STARTPOINT.x,PinMaps[PinName].STARTPOINT.y , PinMaps[PinName].RELATIVE_POINT1, PinMaps[PinName].RELATIVE_POINT2 ,PinMaps[PinName].ORIENT);
        pair < Point <int> ,Point<int> > getBlockPinRegion;
        vector<Line> terminalLine;
        
        for ( auto it = SpecialNetsMaps[PinName].DESTINATIONMAPS.begin(); it != SpecialNetsMaps[PinName].DESTINATIONMAPS.end();it = SpecialNetsMaps[PinName].DESTINATIONMAPS.upper_bound(it->first))
        {
            vector<Block>nowBlockPin =  myHelper.BlockMaps[it->first];
            for (int i = 0 ; i < nowBlockPin.size();i++ )
            {
                getBlockPinRegion.first = nowBlockPin[i].LeftDown;
                getBlockPinRegion.second = nowBlockPin[i].RightUp;
                if (it->second.compare(nowBlockPin[i].BlockPinName) == 0) //check same blockPin
                {
                   // cout<<it->first<<" "<<it->second<<" "<<nowBlockPin[i].LeftDown<<" "<<nowBlockPin[i].RightUp<<endl;
                    for(int j = 0 ; j < nowBlockPin[i].Metals.size();j++) //for each blockPin layer 
                    {
                        for(int k = 0 ; k < vec_special_net_line.size();k++) //for each now line 
                        {
                            if (nowBlockPin[i].Metals[j].compare(vec_special_net_line[k].MetalName)==0)//check same layer 
                            {
                                if(myHelper.isCrossWithCoordinate(vec_special_net_line[k],getBlockPinRegion))
                                {
                                    terminalLine.push_back(vec_special_net_line[k]);
                                }
                            }
                        }
                    }
                     //cout<<it->first<<" "<<it->second<<" "<<nowBlockPin[i].LeftDown<<" "<<nowBlockPin[i].RightUp<<endl;
                }
            }
        }
        
        Line startLine; 
        for (int i = 0 ; i < vec_special_net_line.size();i++)
        {
            if (vec_special_net_line[i].MetalName.compare(PinMaps[PinName].METALNAME) == 0 ) //check same layer
            {
                if ( myHelper.isCrossWithCoordinate( vec_special_net_line[i] , getPowerPinRegion ) )  
                {
                    startLine = vec_special_net_line[i];
                    OrderLine.push_back(startLine);
                    break;
                }
            }
        }

            cout << "this is start    line : "<<startLine<<endl ;
        for (int i = 0 ; i < terminalLine.size();i++)
        {
            cout<<  "this is terminal line : "<<terminalLine[i]<<endl;
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
    }
}


PDN::~PDN()
{

}

