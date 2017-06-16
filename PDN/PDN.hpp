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
struct MyComparator
{
    bool operator()(const Line & left , const Line & right) const
    {
        if ( left.pt1.x > right.pt1.x )
            return true;
        else if ( left.pt1.x < right.pt1.x )
            return false;
        else 
        {
            if (left.pt1.y > right.pt1.y)
                return true ;
            else if (left.pt1.y < right.pt1.y)
                return false;
            else 
            {
                if (left.pt2.x > right.pt2.x)
                    return true;
                else if (left.pt2.x < right.pt2.x)
                    return false;
                else 
                {
                    if (left.pt2.y > left.pt2.y)
                        return true;
                    else if (left.pt2.y < left.pt2.y)
                        return false;
                    else 
                    {
                        if(left.MetalName.compare(right.MetalName)!=0)
                            return true;
                        else 
                            return false;
                    }
                }
            }
        }
    }
}; 


class PDN
{
public:
    PDN();
    ~PDN();
    map <  string , map < string  , vector <Line> > >  List;
    vector< vector <Line> >  DFS( vector<Line>& vec_special_net_line , Line& line , vector <Line> & terminals ) ;
private:
};

#endif /* PDN_hpp */
