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



    PDN::PDN(string str)
:WhichCase(str)
{
    //getRoutingResource();
}
vector <vector <string >> PDN::getNoPassInfo ()
{
    //    Converter con(WhichCase);
    //    con.toSpice();
    //    ngspice ng ;
    //    ng.init(WhichCase);
    //    cout<<"this is nopass: ";
    //    ng.printStats(con.DestinationMap);
    //    cout<<"End.\n"; 
    //    return ng.NoPassInfo;
    return vector<vector<string>>();
}
vector <vector <string >> PDN::getPassInfo ()
{
    //    Converter con(WhichCase);
    //    con.toSpice();
    //    ngspice ng ;
    //    ng.init(WhichCase);
    //    cout<<"this is pass: ";
    //    ng.printStats(con.DestinationMap);
    //    cout<<"End.\n";
    //    return ng.PassInfo;
    return vector<vector<string>>();
}
bool PDN::isHorizontalExist( Nets* NET1,Nets* NET2 )
{
    if ( NET1->ABSOLUTE_POINT1.x >= NET2->ABSOLUTE_POINT1.x && NET1->ABSOLUTE_POINT1.x <= NET2->ABSOLUTE_POINT2.x )//NET1的前面x存在在NET2裡
        return true;
    if ( NET1->ABSOLUTE_POINT2.x >= NET2->ABSOLUTE_POINT1.x && NET1->ABSOLUTE_POINT2.x <= NET2->ABSOLUTE_POINT2.x )//NET1的後面的X存在在NET2裡
        return true;
    if ( NET2->ABSOLUTE_POINT1.x >= NET1->ABSOLUTE_POINT1.x && NET2->ABSOLUTE_POINT1.x <= NET1->ABSOLUTE_POINT2.x )//NET2的前面的x存在在NET1裡
        return true;
    if ( NET2->ABSOLUTE_POINT2.x >= NET1->ABSOLUTE_POINT1.x && NET2->ABSOLUTE_POINT2.x <= NET1->ABSOLUTE_POINT2.x )//NET2的後面的X存在在NET1裡
        return true;
    return false;
}
void PDN::getRoutingResource()
{
    vector < Nets* > temp;
    //add all wire to vector 
    for (auto PinName : myHelper.PinNames)
    {
        for ( auto it = SpecialNetsMaps[PinName].NETSMULTIMAPS.begin() ; it != SpecialNetsMaps[PinName].NETSMULTIMAPS.end();it = SpecialNetsMaps[PinName].NETSMULTIMAPS.upper_bound(it->first))
        {
            auto first = SpecialNetsMaps[PinName].NETSMULTIMAPS.lower_bound(it->second.METALNAME);
            auto last = SpecialNetsMaps[PinName].NETSMULTIMAPS.upper_bound(it->second.METALNAME);
            while (first != last)
            {
                Nets *ptr = new Nets ;
                ptr = & first -> second;
                if (first->second.ROUNTWIDTH!=0)
                    temp.push_back(ptr);
                first++;
            }
        }
    }
    Nets *ptr = new Nets ;
    ptr->ABSOLUTE_POINT1.x = 0 ;
    ptr->ABSOLUTE_POINT1.y = 3000000;
    ptr->ABSOLUTE_POINT2.x = 3000000;
    ptr->ABSOLUTE_POINT2.y = 3000000;
    ptr->ROUNTWIDTH = 0 ; 
    temp.push_back(ptr);
    for ( int i = 0  ; i < temp.size();i++)
    {
        if (temp[i]->ABSOLUTE_POINT1.y==3000000)
            continue;
        int TopResource = 2147483647 ; // Only H line have  
        int BottomResource = 2147483647; //Only H line have 
        int LeftResource = 2147483647 ; //Only V line have
        int RightResource = 2147483647  ; //Only V line have
        if(myHelper.isHorizontal(temp[i]->ABSOLUTE_POINT1,temp[i]->ABSOLUTE_POINT2))
        {
            for(int j = 0 ; j < temp.size();j++)
            {
                if(i==j || !myHelper.isHorizontal(temp[j]->ABSOLUTE_POINT1,temp[j]->ABSOLUTE_POINT2) )
                    continue;
                if(!isHorizontalExist(temp[i],temp[j]))
                    continue;
                if( temp[j]->ABSOLUTE_POINT1.y < temp[i]->ABSOLUTE_POINT1.y )
                    continue;
                //cout<<"this is : "<<temp[i]->ABSOLUTE_POINT1<<" "<<temp[i]->ABSOLUTE_POINT2<<endl;
                //cout<<"that is : "<<temp[j]->ABSOLUTE_POINT1<<" "<<temp[j]->ABSOLUTE_POINT2<<endl;
                //cout<<"this top is : "<<temp[i]->ABSOLUTE_POINT1.y+ temp[i]->ROUNTWIDTH/2<<endl;
                //cout<<"that bot is : "<<temp[j]->ABSOLUTE_POINT2.y - temp[j]->ROUNTWIDTH/2<<endl;
                int topDistance = abs( (temp[i]->ABSOLUTE_POINT1.y + temp[i]->ROUNTWIDTH/2) - (temp[j]->ABSOLUTE_POINT1.y - temp[j]->ROUNTWIDTH/2)  );
                if(TopResource > topDistance)
                    TopResource = topDistance;
            }
            cout<<temp[i]->ABSOLUTE_POINT1<<" "<<temp[i]->ABSOLUTE_POINT2<<" top resource : "<<TopResource<<endl;
        }
    }

}
//for all no pass * 2 and 不重複*到
void PDN::Optimize()
{
    vector< vector <string> > NoPassInfo = getNoPassInfo() ;
    vector< vector <string> > PassInfo   = getPassInfo() ;
    //use vector be careful the insert and erase time ;
    vector < Nets* > lineGroup ;
    //cout<<"This is NoPass : \n";
    //for(int i = 0 ; i < NoPassInfo.size();i++)
    //{
    //cout<<NoPassInfo[i][0]<<" "<<NoPassInfo[i][1]<< " "<<NoPassInfo[i][2]<<endl;
    //}
    //cout<<"---------\n";
    //cout<<"This is Pass : \n";
    //for(int i = 0 ; i < PassInfo.size();i++)
    //{
    //cout<<PassInfo[i][0]<<" "<<PassInfo[i][1]<< " "<<PassInfo[i][2]<<endl;
    //}
    //cout<<"---------\n";
    map < Nets* , bool > isModify ;
    // use No Pass PinName to get the Net Infomation no pass = optimize 
    for(int i =  0 ; i < NoPassInfo.size();i++ ) //NoPassInfo [i][0] = PinName , [i][1] = BlockName , [i][2] = BlockPinName
    {
        for ( auto it = SpecialNetsMaps[NoPassInfo[i][0]].NETSMULTIMAPS.begin() ; it != SpecialNetsMaps[NoPassInfo[i][0]].NETSMULTIMAPS.end();it = SpecialNetsMaps[NoPassInfo[i][0]].NETSMULTIMAPS.upper_bound(it->first))
        {
            auto first = SpecialNetsMaps[NoPassInfo[i][0]].NETSMULTIMAPS.lower_bound(it->second.METALNAME);
            auto last = SpecialNetsMaps[NoPassInfo[i][0]].NETSMULTIMAPS.upper_bound(it->second.METALNAME);
            while (first != last)
            {
                if(first->second.ROUNTWIDTH!=0)
                {
                    if(myHelper.isHorizontal(first->second.ABSOLUTE_POINT1, first->second.ABSOLUTE_POINT2))
                    {
                        Point<int> right = ( first->second.ABSOLUTE_POINT1.x > first->second.ABSOLUTE_POINT2.x ) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
                        Point<int> left =  ( first->second.ABSOLUTE_POINT1.x < first->second.ABSOLUTE_POINT2.x ) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
                        first->second.ABSOLUTE_POINT1 = left ;
                        first->second.ABSOLUTE_POINT2 = right ;               
                    }
                    else
                    {
                        Point<int> top = (first->second.ABSOLUTE_POINT1.y > first->second.ABSOLUTE_POINT2.y) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2;
                        Point<int> bottom = (first->second.ABSOLUTE_POINT1.y < first->second.ABSOLUTE_POINT2.y) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
                        first->second.ABSOLUTE_POINT2 = top ;
                        first->second.ABSOLUTE_POINT1 = bottom ;
                    }
                }
                Nets *ptr = new Nets ;
                ptr = & first -> second;
                lineGroup.push_back ( ptr ) ;
                first++;
            }
        }
        Point <int> tmpPoint;
        Nets * start = new Nets ; 
        start = nullptr;
        for ( int i = 0 ; i < lineGroup.size();i++)
        {
            tmpPoint = myHelper.getStartPoint(lineGroup[i]->ABSOLUTE_POINT1 , lineGroup[i]->ABSOLUTE_POINT2);
            if (tmpPoint.x != -1 && tmpPoint.y != -1)
                start = lineGroup[i];
        }
        vector <Nets*> PinToBlockPinLines ;
        //for (int i = 0 ; i < lineGroup.size();i++ )
        //{
        //cout << lineGroup[i]->ABSOLUTE_POINT1 << " " << lineGroup[i]->ABSOLUTE_POINT2<<endl;
        //}
        //cout<<"-------------"<<endl;
        PinToBlockPinLines = DFS ( lineGroup , start, NoPassInfo[i][1] , NoPassInfo[i][2]  );
        //cout <<NoPassInfo[i][0]<<" "<<NoPassInfo[i][1]<<" "<<NoPassInfo[i][2]<<endl;
        //for (int i = 0  ; i < PinToBlockPinLines.size();i++)
        //cout << PinToBlockPinLines[i]->ABSOLUTE_POINT1<<" "<<PinToBlockPinLines[i]->ABSOLUTE_POINT2<<endl;
        //cout<<endl; 
        for (int i =  0  ; i  < PinToBlockPinLines.size()  ; i++ )
        {
            if ( isModify [PinToBlockPinLines[i]] == true )
                continue; 
            else
            {
                PinToBlockPinLines[i]->ROUNTWIDTH *= 2 ; 
                isModify[ PinToBlockPinLines[i] ] = true ;
            }
        } 
        //cout<<"****\n";
        //PinToBlockPinLines.back()->ROUNTWIDTH -= 1000 ;
        DRC(PinToBlockPinLines,isModify);   
        AddVia(PinToBlockPinLines,isModify,NoPassInfo[i][0]);
        lineGroup.resize(0);
    }
}
void PDN::DRC ( vector<Nets*> &lineGroup , map <Nets*,bool>&isModify )
{
    vector<Nets*>NoViaLineGroup;
    for(int i = 0  ; i < lineGroup.size();i++)
    {
        if(lineGroup[i]->ROUNTWIDTH!=0)
            NoViaLineGroup.push_back(lineGroup[i]);
    }
    for(int i = 0 ; i < NoViaLineGroup.size()-1;i++ )
    {
        FineTune(NoViaLineGroup[i],NoViaLineGroup[i+1],isModify);
    }
}
void PDN::FineTune( Nets* & source , Nets* & target , map<Nets*,bool>&isModify )
{
    //if ( isModify[source] )
    //{
    if ( myHelper.isHorizontal( source->ABSOLUTE_POINT1,source->ABSOLUTE_POINT2 ) == true)
    {
        int S_LeftX = source->ABSOLUTE_POINT1.x;
        int S_RightX = source->ABSOLUTE_POINT2.x ;
        int S_Y = source->ABSOLUTE_POINT1.y; //S_Y = 不變得Y
        int S_TopY = S_Y + ( source->ROUNTWIDTH/2 );
        int S_BotY = S_Y - ( source->ROUNTWIDTH/2 );
        //Target is Vetertical so 不變X
        int T_X = target->ABSOLUTE_POINT1.x ; 
        int T_LeftX = T_X - (target->ROUNTWIDTH / 2) ;
        int T_RightX = T_X + (target->ROUNTWIDTH / 2) ;
        int T_TopY = target->ABSOLUTE_POINT2.y;
        int T_BotY = target->ABSOLUTE_POINT1.y;
        //水平線有六種狀況 垂直也有
        if ( S_BotY <= T_BotY && S_TopY >= T_BotY && T_LeftX <= S_RightX && T_RightX >= S_RightX && T_TopY >= S_TopY && T_BotY >= S_BotY    ) //右上ok   
        {
            source->ABSOLUTE_POINT2.x = T_RightX ;
            target->ABSOLUTE_POINT1.y = S_BotY;
        }
        else if ( S_RightX >= T_LeftX  && S_RightX <= T_RightX && T_TopY >= S_BotY && S_TopY >= T_TopY   ) //右下ok
        {
            source->ABSOLUTE_POINT2.x = T_RightX;
            target->ABSOLUTE_POINT2.y = S_TopY;
        }
        else if ( S_TopY >= T_BotY && S_TopY <= T_TopY && S_BotY >= T_BotY && S_BotY <= T_TopY && S_RightX >= T_LeftX && S_RightX <= T_RightX )//右邊ok
        {
            source->ABSOLUTE_POINT2.x = T_RightX ;
        }
        else if ( T_TopY >= S_TopY && T_BotY <= S_TopY && S_LeftX >= T_LeftX && T_RightX >= S_LeftX && S_RightX >= T_X ) //左上(不確定)
        {
            source->ABSOLUTE_POINT1.x = T_LeftX;
            target->ABSOLUTE_POINT1.y = S_BotY;
        }
        else if ( T_TopY >= S_BotY && T_TopY <= S_TopY && T_RightX >= S_LeftX && T_RightX <= S_RightX && T_LeftX <= S_LeftX) // 左下(不確定)
        {
            source->ABSOLUTE_POINT1.x = T_LeftX;
            target->ABSOLUTE_POINT2.y = S_TopY;
        }
        else if ( S_TopY >= T_BotY && S_TopY <= T_TopY && S_BotY >= T_BotY && S_BotY <= T_TopY && S_LeftX >= T_LeftX && S_LeftX <= T_RightX  )  //左邊ok
        {
            source->ABSOLUTE_POINT1.x = T_LeftX ;
        }
        else if ( S_TopY >= T_BotY && T_LeftX >= S_LeftX && S_RightX >= S_LeftX  && T_BotY >= S_BotY   ) //在source裡面的上面
        {
            target->ABSOLUTE_POINT1.y = S_BotY;
        }
        else if ( S_TopY >= T_TopY && T_TopY >= S_BotY && T_LeftX >= S_LeftX && S_RightX >= T_RightX  ) //在source裡面的下面
        {
            target->ABSOLUTE_POINT2.y = S_TopY;
        }
    }
    else //source is Vetertical 
    {
        int S_X = source->ABSOLUTE_POINT1.x ; // source is Vetertical so x is cons 
        int S_LeftX = S_X - ( source->ROUNTWIDTH/2 );
        int S_RightX = S_X + ( source->ROUNTWIDTH/2 );
        int S_TopY = source->ABSOLUTE_POINT2.y;
        int S_BotY = source->ABSOLUTE_POINT1.y;
        int T_Y = target ->ABSOLUTE_POINT1.y; // target is H so y is cons ;
        int T_LeftX = target->ABSOLUTE_POINT1.x ;
        int T_RightX = target->ABSOLUTE_POINT2.x;
        int T_TopY = T_Y + (target->ROUNTWIDTH/2);
        int T_BotY = T_Y - (target->ROUNTWIDTH/2);
        if ( S_TopY >= T_BotY && S_TopY <= T_TopY && T_LeftX >= S_LeftX && T_LeftX <= S_RightX && T_BotY >=S_BotY && T_BotY <= S_TopY )//右上ok
        {
            source->ABSOLUTE_POINT2.y = T_TopY;
            target->ABSOLUTE_POINT1.x = S_LeftX;
        }
        else if ( S_BotY >= T_BotY && S_BotY <= T_TopY && T_LeftX >= S_LeftX && T_LeftX <= S_RightX )//右下ok
        {
            source->ABSOLUTE_POINT1.y = T_BotY;
            target->ABSOLUTE_POINT1.x = S_LeftX;
        }
        else if ( S_TopY <= T_TopY && S_TopY >= T_BotY && S_LeftX >= T_LeftX && S_LeftX <= T_RightX && S_RightX>= T_LeftX && S_RightX<=T_RightX )//上面
        {
            source->ABSOLUTE_POINT2.y = T_TopY;
        }
        else if ( S_TopY >= T_BotY && S_TopY <= T_TopY && T_RightX >= S_LeftX && T_RightX <= S_RightX && T_BotY >= S_BotY && T_TopY > S_BotY ) //左上
        {
            source->ABSOLUTE_POINT2.y = T_TopY;
            target->ABSOLUTE_POINT2.x = S_RightX;
        }
        else if ( S_BotY >= T_BotY && S_BotY <= T_TopY && S_LeftX >= T_LeftX && S_LeftX <= T_RightX && S_TopY >= T_TopY && S_BotY >= T_BotY )// 左下
        {
            source->ABSOLUTE_POINT1.y = T_BotY;
            target->ABSOLUTE_POINT2.x = S_RightX;
        }
        else if ( S_TopY >= T_BotY && S_TopY <= T_TopY && S_LeftX >= T_RightX && S_LeftX <= T_RightX && S_RightX >= T_LeftX && S_RightX <= T_RightX )//下面
        {
            source->ABSOLUTE_POINT1.y = T_BotY;
        }
        else if ( S_TopY >= T_TopY && T_BotY >= S_BotY && T_RightX >= S_LeftX && T_RightX <=S_RightX ) //在source裡的左邊
        {
            target->ABSOLUTE_POINT2.x = S_RightX;   
        }
        else if  (S_TopY >= T_TopY && T_BotY >= S_BotY && T_LeftX >= S_LeftX && S_RightX >= T_LeftX ) //在source的右邊
        {
            target->ABSOLUTE_POINT1.x = S_LeftX;
        }
    }
}



vector <Nets*> PDN::DFS ( vector<Nets*>&lineGroup , Nets* &start  , string blockName ,string blockPinName  ){
    vector <bool> isTrav;
    for (int i = 0 ; i < lineGroup.size();i++)
    {
        if (start == lineGroup[i])
            isTrav.push_back(1);
        else 
            isTrav.push_back(0);
    }
    stack <Nets*> Stack ; 
    Stack.push(start);
    Point<int> tmpPoint;
    pair<string ,string >tmpPair;
    Nets *terminalNet = new Nets ;
    for (int i =  0 ; i < lineGroup.size();i++)
    {
        tmpPoint = myHelper.getEndPoint(lineGroup[i]->ABSOLUTE_POINT1,lineGroup[i]->ABSOLUTE_POINT2);
        if ( tmpPoint.x != -1 && tmpPoint.y != -1)
        {
            tmpPair = myHelper.getBlockMsg(tmpPoint);
            if (tmpPair.first == blockName && tmpPair.second == blockPinName )
            {
                terminalNet = lineGroup[i];
            }
        }
    }
    vector <Nets*> return_vec;
    return_vec.push_back(start);
    while (Stack.size()!=0)
    {
        if (Stack.top()->ABSOLUTE_POINT1 == terminalNet->ABSOLUTE_POINT1 && Stack.top()->ABSOLUTE_POINT2 == terminalNet->ABSOLUTE_POINT2  )
            return return_vec;
        bool flag = 0 ;
        bool checkAllNoCross = 0 ;
        if(Stack.top()->ROUNTWIDTH == 0 )
        {
            for (int i = 0 ; i < lineGroup.size();i++)
            {
                if ( lineGroup[i] == Stack.top() )
                    continue;
                if ( isTrav[i] == 1 )
                    continue;
                if ( Stack.top()->ABSOLUTE_POINT1 == lineGroup[i]->ABSOLUTE_POINT1 && Stack.top()->ABSOLUTE_POINT2 == lineGroup[i]->ABSOLUTE_POINT2  )
                {
                    checkAllNoCross = 1 ;
                    isTrav[i] = 1 ;
                    Stack.push(lineGroup[i]);
                    return_vec.push_back(lineGroup[i]);
                    flag = 1 ;
                    break;
                }
            }
            if(flag)
                continue;
            for (int i  = 0  ; i < lineGroup.size();i++)
            {
                if ( lineGroup[i] == Stack.top() )
                    continue;
                if ( isTrav[i] == 1 )
                    continue;
                if (Stack.top()->ROUNTWIDTH==0 &&lineGroup[i]->ROUNTWIDTH==0 )
                    continue;
                Line tmpLine1(lineGroup[i]->ABSOLUTE_POINT1 , lineGroup[i]->ABSOLUTE_POINT2);
                Line tmpLine2(Stack.top()->ABSOLUTE_POINT1 , Stack.top()->ABSOLUTE_POINT2);
                tmpLine1.isHorizontal = myHelper.isHorizontal(lineGroup[i]->ABSOLUTE_POINT1,lineGroup[i]->ABSOLUTE_POINT2);
                tmpLine2.isHorizontal = myHelper.isHorizontal(Stack.top()->ABSOLUTE_POINT1,Stack.top()->ABSOLUTE_POINT2);
                tmpLine1.Width = lineGroup[i]->ROUNTWIDTH;
                tmpLine2.Width = Stack.top()->ROUNTWIDTH;
                if (Stack.top()->ROUNTWIDTH==0)
                    tmpLine2.isPsedoLine = 1 ;
                if (lineGroup[i]->ROUNTWIDTH==0)
                    tmpLine1.isPsedoLine = 1;
                if (myHelper.isCross( tmpLine1 , tmpLine2  ) )
                {
                    isTrav[i] = 1 ;
                    checkAllNoCross = 1 ;
                    Stack.push(lineGroup[i]);
                    return_vec.push_back(lineGroup[i]);
                    break;
                }
            }
        }
        else 
        {
            for (int i = 0 ; i < lineGroup.size();i++)
            {

                if (  lineGroup[i] == Stack.top()   )
                    continue;
                if ( isTrav[i] == 1 )
                    continue;
                if(lineGroup[i]->ROUNTWIDTH == 0)
                {
                    //cout<<"****"<<endl;
                    Line tmpLine1(lineGroup[i]->ABSOLUTE_POINT1 , lineGroup[i]->ABSOLUTE_POINT2);
                    Line tmpLine2(Stack.top()->ABSOLUTE_POINT1 , Stack.top()->ABSOLUTE_POINT2);
                    tmpLine1.isHorizontal = myHelper.isHorizontal(lineGroup[i]->ABSOLUTE_POINT1,lineGroup[i]->ABSOLUTE_POINT2);
                    tmpLine1.isPsedoLine = 1 ;
                    tmpLine2.isHorizontal = myHelper.isHorizontal(Stack.top()->ABSOLUTE_POINT1,Stack.top()->ABSOLUTE_POINT2);
                    tmpLine1.Width = lineGroup[i]->ROUNTWIDTH;
                    tmpLine2.Width = Stack.top()->ROUNTWIDTH;
                    if (Stack.top()->ROUNTWIDTH==0)
                        tmpLine2.isPsedoLine = 1 ;
                    if ( myHelper.isCross(tmpLine1,tmpLine2) )
                    {
                        checkAllNoCross = 1 ;
                        isTrav[i] = 1 ;
                        Stack.push(lineGroup[i]);
                        return_vec.push_back(lineGroup[i]);
                        flag = 1 ;
                        break;
                    }
                }
            }
            if(flag)
                continue;
            for (int i = 0 ; i < lineGroup.size();i++)
            {
                if ( lineGroup[i] == Stack.top()  )
                    continue;
                if ( isTrav[i] == 1 )
                    continue;
                Line tmpLine1(lineGroup[i]->ABSOLUTE_POINT1 , lineGroup[i]->ABSOLUTE_POINT2);
                Line tmpLine2(Stack.top()->ABSOLUTE_POINT1 , Stack.top()->ABSOLUTE_POINT2);
                tmpLine1.isHorizontal = myHelper.isHorizontal(lineGroup[i]->ABSOLUTE_POINT1,lineGroup[i]->ABSOLUTE_POINT2);
                tmpLine2.isHorizontal = myHelper.isHorizontal(Stack.top()->ABSOLUTE_POINT1,Stack.top()->ABSOLUTE_POINT2);
                tmpLine1.Width = lineGroup[i]->ROUNTWIDTH;
                tmpLine2.Width = Stack.top()->ROUNTWIDTH;
                if (Stack.top()->ROUNTWIDTH==0)
                    tmpLine2.isPsedoLine = 1 ;
                if (lineGroup[i]->ROUNTWIDTH==0)
                    tmpLine1.isPsedoLine = 1;
                if ( myHelper.isCross(tmpLine1,tmpLine2) )
                {
                    checkAllNoCross = 1 ;
                    isTrav[i] = 1 ;
                    Stack.push(lineGroup[i]);
                    return_vec.push_back(lineGroup[i]);
                    flag = 1 ;
                    break;
                }
            }

        }
        if(checkAllNoCross == 0 )
        {
            return_vec.pop_back();
            Stack.pop();
        }
    }
    assert(0);
    return vector <Nets*>() ;
}
void PDN::AddVia( vector<Nets*> &lineGroup , map <Nets* , bool > &isAdd  ,string PinName)
{
    for(int i = 0  ;  i < lineGroup.size();i++)
    {
        //if ( isAdd[lineGroup[i]] == true )
        //continue;
        if ( lineGroup[i]->ROUNTWIDTH == 0  )
        {
            if ( lineGroup[i-1]->METALNAME.compare(lineGroup[i+1]->METALNAME)!=0 && lineGroup[i+1]->ROUNTWIDTH != 0 )//只檢查單科Via via連via還沒做
            {
                pair<Point<int>,Point<int>> getPair = getTwoLineRegion (lineGroup[i-1],lineGroup[i+1] );
                int viaWidth = getViaWidth(lineGroup[i]);
                int RegionLength = getPair.second.x - getPair.first.x ;
                int RegionWidth  = getPair.second.y - getPair.first.y ;
                int HowManyLenCanAdd =  RegionLength / viaWidth;
                int HowManyWidCanAdd = RegionWidth / viaWidth;
                lineGroup[i]->ABSOLUTE_POINT1.x = getPair.first.x + viaWidth / 2 ;
                lineGroup[i]->ABSOLUTE_POINT1.y = getPair.first.y + viaWidth / 2 ;
                int tmpPointX = lineGroup[i]->ABSOLUTE_POINT1.x;
                int tmpPointY = lineGroup[i]->ABSOLUTE_POINT1.y;
                for(int k = 0 ; k < HowManyLenCanAdd  ; k++)
                {
                    tmpPointX = getPair.first.x - viaWidth / 2 ;
                    for(int j = 0 ; j < HowManyWidCanAdd ; j++)
                    {
                        if (k == 0 && j == 0 )
                        {
                            tmpPointX += viaWidth;
                            continue;
                        }
                        tmpPointX += viaWidth;
                        Nets obj;
                        obj.ABSOLUTE_POINT1.x = tmpPointX;
                        obj.ABSOLUTE_POINT1.y = tmpPointY;
                        obj.VIANAME = lineGroup[i]->VIANAME;
                        obj.METALNAME = lineGroup[i]->METALNAME;
                        //cout<<obj.ABSOLUTE_POINT1 <<" "<<obj.ABSOLUTE_POINT2<<" "<<obj.VIANAME<<endl;
                        obj.ROUNTWIDTH = 0 ;
                        SpecialNetsMaps[PinName].NETSMULTIMAPS.insert( make_pair(lineGroup[i]->METALNAME,obj) );                                  
                    }
                    tmpPointY += viaWidth;
                }

            }
            if (lineGroup[i+1]->ROUNTWIDTH ==0 )
            {
                cout<<lineGroup[i]->ABSOLUTE_POINT1<<" "<<lineGroup[i]->ABSOLUTE_POINT2<<lineGroup[i]->VIANAME<<endl;
                vector<Nets*> tempVector;
                tempVector.push_back(lineGroup[i]);
                for(int j = i+1 ; j < lineGroup.size();j++ )
                { 
                    if (lineGroup[j]->ROUNTWIDTH ==0 )
                    {
                        tempVector.push_back(lineGroup[j]);
                    }
                    else
                        break;
                }
                for(int j = 0 ; j < tempVector.size();j++)
                {
                    pair<Point<int>,Point<int>> getPair = getTwoLineRegion (lineGroup[i-1],lineGroup[i+tempVector.size()] );
                    int viaWidth = getViaWidth(lineGroup[i]);
                    int RegionLength = getPair.second.x - getPair.first.x ;
                    int RegionWidth  = getPair.second.y - getPair.first.y ;
                    int HowManyLenCanAdd =  RegionLength / viaWidth;
                    int HowManyWidCanAdd = RegionWidth / viaWidth;
                    tempVector[j]->ABSOLUTE_POINT1.x = getPair.first.x + viaWidth / 2 ;
                    tempVector[j]->ABSOLUTE_POINT1.y = getPair.first.y + viaWidth / 2 ;
                    int tmpPointX = tempVector[j]->ABSOLUTE_POINT1.x;
                    int tmpPointY = tempVector[j]->ABSOLUTE_POINT1.y;
                    for(int k = 0 ; k < HowManyLenCanAdd  ; k++)
                    {
                        tmpPointX = getPair.first.x - viaWidth / 2 ;
                        for(int h = 0 ; h < HowManyWidCanAdd ; h++)
                        {
                            if (k == 0 && h == 0 )
                            {
                                tmpPointX += viaWidth;
                                continue;
                            }
                            tmpPointX += viaWidth;
                            Nets obj;
                            obj.ABSOLUTE_POINT1.x = tmpPointX;
                            obj.ABSOLUTE_POINT1.y = tmpPointY;
                            obj.VIANAME = tempVector[j]->VIANAME;
                            obj.METALNAME = tempVector[j]->METALNAME;
                            //cout<<obj.ABSOLUTE_POINT1 <<" "<<obj.ABSOLUTE_POINT2<<" "<<obj.VIANAME<<endl;
                            obj.ROUNTWIDTH = 0 ;
                            SpecialNetsMaps[PinName].NETSMULTIMAPS.insert( make_pair(tempVector[j]->METALNAME,obj) );                                  
                        }
                        tmpPointY += viaWidth;
                    }   

                }
                i+=tempVector.size();
            }
        }

    }
}

int  PDN::getViaWidth (Nets* via)
{
    int tmp = 0 ;
    int MAX = 0 ;
    for (auto it = ViaMaps[via->VIANAME].InnerMaps.begin() ; it!= ViaMaps[via->VIANAME].InnerMaps.end();++it)
    {   
        tmp  =  it->second.pt2.y * 2 ;
        if (tmp > MAX)
            MAX = tmp ;
    }
    return MAX * UNITS_DISTANCE ; 
}

pair <Point<int>,Point<int> > PDN::getTwoLineRegion( Nets* NET1 , Nets* NET2 )
{
    pair<Point<int>,Point<int>> return_pair;
    Point<int> LeftDown;
    Point<int> RightUp;
    Point<int> CrossPoint;
    if ( myHelper.isHorizontal(NET1->ABSOLUTE_POINT1,NET1->ABSOLUTE_POINT2)  )//NET1 is H NET2 is V
    {
        CrossPoint.x = NET2->ABSOLUTE_POINT1.x ;
        CrossPoint.y = NET1->ABSOLUTE_POINT1.y ;
        LeftDown.x =  CrossPoint.x - ( NET2->ROUNTWIDTH / 2) ; 
        LeftDown.y =  CrossPoint.y - ( NET1->ROUNTWIDTH / 2) ;
        RightUp.x  =  CrossPoint.x + ( NET2->ROUNTWIDTH / 2) ;
        RightUp.y  =  CrossPoint.y + ( NET2->ROUNTWIDTH / 2) ;
        return_pair.first  = LeftDown;
        return_pair.second = RightUp;
    }
    else //NET1 is V NET2 is H  
    {   
        CrossPoint.x = NET1->ABSOLUTE_POINT1.x ;
        CrossPoint.y = NET2->ABSOLUTE_POINT1.y ;
        LeftDown.x = CrossPoint.x - ( NET1->ROUNTWIDTH / 2 );
        LeftDown.y = CrossPoint.y - ( NET2->ROUNTWIDTH / 2 );
        RightUp.x = CrossPoint.x  + ( NET1->ROUNTWIDTH / 2 );
        RightUp.y = CrossPoint.y  + ( NET2->ROUNTWIDTH / 2 );
        return_pair.first  = LeftDown;
        return_pair.second = RightUp;
    }
    return return_pair;
}

PDN::~PDN()
{

}

