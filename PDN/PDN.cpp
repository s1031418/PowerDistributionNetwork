//
//  PDN.cpp
//  EDA_Contest2017(PDN)
//
//

#include "PDN.hpp"
#include <stack>
#include <deque>
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
        map < Line,map <string ,string > > TerimialLineToBlockPinName;
        map < Line,string > StartLineToPowerPinName;
        map < string , string > BlockNameAndBlockPinName;
        map < string , string > BlockNameAndPinName;
        for ( auto it = SpecialNetsMaps[PinName].DESTINATIONMAPS.begin(); it != SpecialNetsMaps[PinName].DESTINATIONMAPS.end();it = SpecialNetsMaps[PinName].DESTINATIONMAPS.upper_bound(it->first))
        {
            vector<Block>nowBlockPin =  myHelper.BlockMaps[it->first];
            BlockNameAndBlockPinName[ it->first ] = it->second;
            BlockNameAndPinName[ it->first ] = PinName ;
            for (int i = 0 ; i < nowBlockPin.size();i++ )
            {
                getBlockPinRegion.first = nowBlockPin[i].LeftDown;
                getBlockPinRegion.second = nowBlockPin[i].RightUp;
                if (it->second.compare(nowBlockPin[i].BlockPinName) == 0) //check same blockPin
                {
                    for(int j = 0 ; j < nowBlockPin[i].Metals.size();j++) //for each blockPin layer 
                    {
                        for(int k = 0 ; k < vec_special_net_line.size();k++) //for each now line 
                        {
                            if (nowBlockPin[i].Metals[j].compare(vec_special_net_line[k].MetalName)==0)//check same layer 
                            {
                                if(myHelper.isCrossWithCoordinate(vec_special_net_line[k],getBlockPinRegion))
                                {
                                    terminalLine.push_back(vec_special_net_line[k]);
                                    TerimialLineToBlockPinName[ vec_special_net_line[k] ] = BlockNameAndBlockPinName ; 
                                }
                            }
                        }
                    }
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
                    StartLineToPowerPinName[vec_special_net_line[i]] = PinName ;
                    break;
                }
            }
        }
        
        stack<Line> Stack;
        vector<Line> List ;
        startLine.isTraversal = true ;
        Stack.push(startLine);
        map < string ,vector<Line> > BlockPinToOrderLine;
        
        cout << "this is start    line : "<<startLine<<endl ;
        
        for (int i = 0 ; i < terminalLine.size();i++)
        {
            cout<<  "this is terminal line : "<<terminalLine[i]<<endl;
        }
        //while (Stack.size() != 0)
        //{
            //cout<<Stack.top()<<endl;
            //Stack.top().isTraversal = 1 ;
            //bool checkAllNoCross = 0 ;
            //for (int i = 0 ; i < vec_special_net_line.size(); i++ )
            //{
                //if (vec_special_net_line[i].isTraversal == true )
                    //continue;
                //if (myHelper.isCross(Stack.top(),vec_special_net_line[i]))
                //{
                    //checkAllNoCross = 1 ;
                    //Stack.push(vec_special_net_line[i]);
                    //break;
                //}
            //}
            //if (checkAllNoCross == 0 )
            //{
                //Stack.pop();
            //}
        //}
        
    }
}


PDN::~PDN()
{

}

