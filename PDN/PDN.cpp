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
        cout<<PinName<<endl;
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
                if (line.isPsedoLine)
                {
                    for ( auto x : ViaMaps[first->second.VIANAME].InnerMaps)
                    {
                        line.ViaMetal.push_back(x.first);
                    }
                }
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
        //map < Line,map <string ,string > > TerimialLineToBlockPinName;
        //map < Line,string > StartLineToPowerPinName;
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
                                    //TerimialLineToBlockPinName[ vec_special_net_line[k] ] = BlockNameAndBlockPinName ; 
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
                    //StartLineToPowerPinName[vec_special_net_line[i]] = PinName ;
                    break;
                }
            }
        }
        for (int i = 0  ; i < vec_special_net_line.size();i++)
        {
            if (vec_special_net_line[i].isPsedoLine)
            {
                Line Psedo(vec_special_net_line[i].pt1 , vec_special_net_line[i].pt2);
                Psedo.ViaName = vec_special_net_line[i].ViaName ;
                Psedo.Width = vec_special_net_line[i].Width;
                Psedo.ViaMetal = vec_special_net_line[i].ViaMetal;
                for ( int j = 0 ; j < vec_special_net_line[i].ViaMetal.size();j++)
                {
                    if (vec_special_net_line[i].ViaMetal[j].compare(vec_special_net_line[i].MetalName)!=0)
                    {
                        Psedo.MetalName = vec_special_net_line[i].ViaMetal[j];
                        vec_special_net_line.push_back(Psedo);
                    }
                }
            }
        }
        for (int i = 0 ; i < vec_special_net_line.size();i++)
        {
            if ( vec_special_net_line[i].Width == 0)
                vec_special_net_line[i].isPsedoLine = 1 ;
        }
        //for (int i = 0 ; i < vec_special_net_line.size();i++)
        //{
        //cout<< vec_special_net_line[i]<<" "<<vec_special_net_line[i].MetalName<<endl;
        //}
        vector<Line> List ;
        startLine.isTraversal = true ;
        map < string ,vector<Line> > BlockPinToOrderLine;
        //for(int i = 0 ; i < vec_special_net_line.size();i++)
        //{
        //cout<<vec_special_net_line[i]<<" "<<vec_special_net_line[i].MetalName<<endl;
        //}
        //cout << "this is start    line : "<<startLine<<endl ;

        //for (int i = 0 ; i < terminalLine.size();i++)
        //{
        //cout<<  "this is terminal line : "<<terminalLine[i]<<endl;
        //}
        vector< vector <Line> > Ans;
        map < Line , string > d;
        for (int i = 0 ; i < vec_special_net_line.size();i++)
        {
            d [ vec_special_net_line[i]  ] = vec_special_net_line[i].MetalName ;
        }
        for ( int i = 0 ; i < vec_special_net_line.size() ; i++)
        {
            cout<<vec_special_net_line[i]<<" "<<vec_special_net_line[i].MetalName<<endl;
        }
        for (int i = 0 ; i < vec_special_net_line.size();i++)
        {
            long long int sum = 0 ;
            for(int j = 0 ; j < vec_special_net_line[i].MetalName.size() ; j++)
            {
                sum += int(vec_special_net_line[i].MetalName[j]) * pow(26,j);
            }
            sum += vec_special_net_line[i].pt1.x;
            sum += vec_special_net_line[i].pt1.y;
            sum += vec_special_net_line[i].pt2.x;
            sum += vec_special_net_line[i].pt2.y;
            cout<<vec_special_net_line[i]<<" "<<d[vec_special_net_line[i]]<<" "<<sum<<endl;
        }
        //for ( auto x : d )
        //{
            //cout << x.first << " " << x.second<<endl;
        //}
        Ans = DFS(vec_special_net_line ,startLine,terminalLine); 
        //cout<<"---------------"<<endl;
        //for(int i = 0 ; i < Ans.size();i++ )
        //{
            //for(int j = 0 ; j < Ans[i].size();j++)
            //{
                //cout << Ans[i][j]<<endl;
            //}
            //cout<<"---------------"<<endl;
        //}
    }
}
vector < vector <Line> > PDN::DFS( vector<Line>&vec_special_net_line , Line & start , vector<Line> & terminals )
{
    PDNHelper myHelper ; 
    stack <Line> Stack;
    Stack.push(start);
    vector<Line>tmp_vec;
    tmp_vec.push_back(start);
    vector<vector < Line > >Return_vec;
    for (int i = 0 ; i < vec_special_net_line.size();i++)
    {
        if (start==vec_special_net_line[i])
            vec_special_net_line[i].isTraversal = 1;
        if (vec_special_net_line[i].MetalName[0]!='M')
            vec_special_net_line[i].isTraversal = 1; 
    }
    while (Stack.size() != 0)
    {
        for (int i = 0 ; i < terminals.size();i++)
        {
            if(Stack.top() == terminals[i] )
                Return_vec.push_back(tmp_vec);
        }
        //cout << Stack.top() <<" is top "<<Stack.top().MetalName<<endl;
        bool checkAllNoCross = 0 ;
        //via find via ( first first )
        bool flag = 0 ;
        if(Stack.top().isPsedoLine)
        {
            for (int i = 0 ; i < vec_special_net_line.size(); i++ )
            {
                if (vec_special_net_line[i] == Stack.top() && vec_special_net_line[i].MetalName.compare(Stack.top().MetalName)==0 )
                    continue;
                if (vec_special_net_line[i].isTraversal == true)
                    continue;
                if (( (Stack.top().pt1 == vec_special_net_line[i].pt1) && (Stack.top().pt2 == vec_special_net_line[i].pt2) )&& (vec_special_net_line[i].MetalName[0]=='M') ) // via find via (first first) 
                {
                    checkAllNoCross = 1 ;
                    vec_special_net_line[i].isTraversal = 1 ;
                    Stack.push(vec_special_net_line[i]);
                    tmp_vec.push_back(vec_special_net_line[i]);           
                    flag = 1 ;
                    break;
                }
            }
            if (flag)
                continue;
            for (int i = 0 ; i < vec_special_net_line.size(); i++ ) //via find wire (last)
            {
                if (vec_special_net_line[i] == Stack.top() && vec_special_net_line[i].MetalName.compare(Stack.top().MetalName)==0 )
                    continue;
                if (vec_special_net_line[i].isTraversal == true)
                    continue;
                if (myHelper.isCross(Stack.top(),vec_special_net_line[i])&& Stack.top().MetalName.compare(vec_special_net_line[i].MetalName)==0)//find line 
                {
                    if (vec_special_net_line[i].isTraversal == true )
                        continue;
                    checkAllNoCross = 1 ;
                    vec_special_net_line[i].isTraversal = 1 ;
                    Stack.push(vec_special_net_line[i]);
                    tmp_vec.push_back(vec_special_net_line[i]);           
                    break;
                }
            }
        }
        else 
        {
            for (int i = 0 ; i < vec_special_net_line.size(); i++ ) // wire find via
            {
                if (vec_special_net_line[i] == Stack.top() && vec_special_net_line[i].MetalName.compare(Stack.top().MetalName)==0 )
                    continue;
                if (vec_special_net_line[i].isTraversal == true)
                    continue;
                if(vec_special_net_line[i].isPsedoLine)
                {
                    if (myHelper.isCross(Stack.top(),vec_special_net_line[i])&& Stack.top().MetalName.compare(vec_special_net_line[i].MetalName)==0)//find line 
                    {
                        if (vec_special_net_line[i].isTraversal == true )
                            continue;
                        checkAllNoCross = 1 ;
                        vec_special_net_line[i].isTraversal = 1 ;
                        Stack.push(vec_special_net_line[i]);
                        tmp_vec.push_back(vec_special_net_line[i]);           
                        flag =1 ;
                        break;
                    }
                }
            }
            if(flag)
                continue;
            for (int i = 0 ; i < vec_special_net_line.size(); i++ ) // wire find wire (last)
            {
                if (vec_special_net_line[i] == Stack.top() && vec_special_net_line[i].MetalName.compare(Stack.top().MetalName)==0 )
                    continue;
                if (vec_special_net_line[i].isTraversal == true)
                    continue;
                if (myHelper.isCross(Stack.top(),vec_special_net_line[i])&& Stack.top().MetalName.compare(vec_special_net_line[i].MetalName)==0)//find line 
                {
                    if (vec_special_net_line[i].isTraversal == true )
                        continue;
                    checkAllNoCross = 1 ;
                    vec_special_net_line[i].isTraversal = 1 ;
                    Stack.push(vec_special_net_line[i]);
                    tmp_vec.push_back(vec_special_net_line[i]);           
                    break;
                }
            }
        }
        if (checkAllNoCross == 0 )
        {
            tmp_vec.pop_back();
            Stack.pop();
        }
    }    
    return Return_vec;
}


PDN::~PDN()
{

}

