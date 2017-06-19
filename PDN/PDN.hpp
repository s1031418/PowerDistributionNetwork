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
class PDN
{
    public:
        PDN(string str);
        ~PDN();
        string WhichCase; 
        PDNHelper myHelper;
        vector <Nets*> DFS ( vector<Nets*>&lineGroup , Nets* &start  , string blockName ,string blockPinName  );
        //vector< vector <Line> >  DFS( vector<Line>& vec_special_net_line , Line& line , vector <Line> & terminals ) ;
        vector <vector <string >> getNoPassInfo ();
        vector <vector <string >> getPassInfo ();
        void FineTune( Nets* & source , Nets* & target , map<Nets*,bool>&isModify  );
        void DRC ( vector<Nets*> &lineGroup , map < Nets* , bool >& isModify  );
        void Optimize();
        void ToSpecialNetsMaps();
    private:
};

#endif /* PDN_hpp */
