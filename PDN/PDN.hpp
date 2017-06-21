//
//  PDN.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 17/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef PDN_hpp
#define PDN_hpp
#include <utility>   
#include <stdio.h>
#include <vector>
#include <string>
#include <list>
#include "../Parsers/Components.h"
#include "Converter.hpp"
#include "PDNHelper.hpp"
#include <set>
#include <cmath>
using namespace std ;
class myPair
{
    public:
    string first;
    string second;
    myPair ()
        :first(),second()
    {
    };
    ~myPair (){}
    myPair (const string  str1 , const string  str2 )
    {
        first = str1;
        second = str2;
    };

    myPair (myPair const & cp) 
        :first(cp.first),second(cp.second)
    {
    };
    myPair operator = ( myPair const &cp  )
    {
        first = cp.first;
        second = cp.second;
    };
    void set (string str1,string str2)
    {
        first = str1;
        second = str2;
    }   
};

class PDN
{
    public:
        PDN(string str);
        ~PDN();
        string WhichCase; 
        PDNHelper myHelper;
        //map < Nets* , vector<int> > Routing_Resource_Map;
        //未完成的拿routingResource;
        void getRoutingResource ();
        vector <Nets*> DFS ( vector<Nets*>&lineGroup , Nets* &start  , string blockName ,string blockPinName  );
        //利用NGSPICE抓出來沒過的線段的PinName BlockName BlockPinName <PinName>放在[i][0] <BlockName>放在[i][1] <BlockPinName>放在[i][2] ;
        vector <vector <string >> getNoPassInfo ();
        //同上 抓出過的
        vector <vector <string >> getPassInfo ();
        //上條線根下條線如果加寬會有issue產生 故做微調 多打金屬補齊
        void FineTune( Nets* & source , Nets* & target , map<Nets*,bool>&isModify  );
        //檢查線是否跨層 是的話就打好打滿VIA
        void AddVia( vector<Nets*> &lineGroup , map <Nets* , bool > &isAdd  , string PinName);
        //給PowerPinName BlockName BlockPinName 找出他的部分線段 用的時候要小心是否改過了
        void DRC ( vector<Nets*> &lineGroup , map < Nets* , bool >& isModify  );
        //判斷兩條線的2個X座標是否存在於對方的線段中
        bool isHorizontalExist( Nets* NET1,Nets* NET2 );
        //開啟優化引擎(目前只有*2)
        void Optimize();
        //拿到兩條線的交叉區域(只准丟進來一條H一條V的"線") 不能丟進VIA return pair first 是左下角 second 是右上角;
        pair<Point<int>,Point<int>> getTwoLineRegion( Nets* NET1 , Nets* NET2 );
        int getViaWidth ( Nets * via ) ;
    private:
};

#endif /* PDN_hpp */
