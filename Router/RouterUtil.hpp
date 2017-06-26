//
//  RouterUtil.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 13/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef RouterUtil_hpp
#define RouterUtil_hpp

#include <stdio.h>
#include "RouterComponents.hpp"
#include <string>
#include "lefrw.h"
#include "defrw.h"
#include "PDNHelper.hpp"
using namespace std; 
class RouterUtil : public PDNHelper 
{
    
public:
    RouterUtil();
    ~RouterUtil();
    
    
    // 判斷這個grid是否為block
    bool IsBlock(Grid grid);
    // 第一個為是不是屬於這個Block，第二個這個Block Name
    pair<bool, string> IsBlock(Point<int> LeftDown , Point<int> RightUp);
    
    
    string getLCS(string s1,string s2);
    
    int translateMetalNameToInt(string MetalName);
    
    string translateIntToMetalName(int layer);
    
    
private:
    
    
    void InitBlockMap();
    // key:BlockName , value:那個block的左下及右上座標
    map<string, pair<Point<int>, Point<int>>> BlockMap;
};


#endif /* RouterUtil_hpp */
