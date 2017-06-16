//
//  PDN.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 17/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef PDN_hpp
#define PDN_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <list>
#include "../Parsers/Components.h"
#include "PDNHelper.hpp"
#include <set>
using namespace std ;

class PDN
{
public:
    PDN();
    ~PDN();
    map <  string , map < string  , vector <Line> > >  List;
    void  DFS( vector<Line>& vec_special_net_line , Line& line ) ;
private:
};

#endif /* PDN_hpp */
