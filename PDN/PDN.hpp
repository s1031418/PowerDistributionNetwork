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
        PDN();
        ~PDN();
        map <  string , map < string  , vector <Line> > >  ADJ_List;
        vector< vector <Line> >  DFS( vector<Line>& vec_special_net_line , Line& line , vector <Line> & terminals ) ;
    private:
};

#endif /* PDN_hpp */
