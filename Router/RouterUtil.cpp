//
//  RouterUtil.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 13/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "RouterUtil.hpp"




RouterUtil::RouterUtil()
{
    InitBlockMap();
}
void RouterUtil::InitBlockMap()
{
    for( auto component : ComponentMaps )
    {
        pair<Point<int>,Point<int>> coordinate = this->getBlockCoordinate(component.second.STARTPOINT.x, component.second.STARTPOINT.y, MacroMaps[ component.second.MACROTYPE ].WIDTH * UNITS_DISTANCE, MacroMaps[ component.second.MACROTYPE ].LENGTH * UNITS_DISTANCE, component.second.ORIENT);
        BlockMap.insert(make_pair(component.first, coordinate));
    }
}

RouterUtil::~RouterUtil()
{
    
}
bool RouterUtil::IsBlock(Grid grid)
{
    if( grid.lowermetal == 0 && grid.uppermetal == 0 )
        return false ;
    else
        return true ;
}
int RouterUtil::translateMetalNameToInt(string MetalName)
{
    if( MetalName == "METAL10" ) return 10;
    if( MetalName == "METAL9" ) return 9;
    if( MetalName == "METAL8" ) return 8;
    if( MetalName == "METAL7" ) return 7;
    if( MetalName == "METAL6" ) return 6;
    if( MetalName == "METAL5" ) return 5;
    if( MetalName == "METAL4" ) return 4;
    if( MetalName == "METAL3" ) return 3;
    if( MetalName == "METAL2" ) return 2;
    if( MetalName == "METAL1" ) return 1;
    assert(0);
}
string RouterUtil::translateIntToMetalName(int layer)
{
    if( layer == 12 ) return "METAL12";
    if( layer == 11 ) return "METAL11";
    if( layer == 10 ) return "METAL10";
    if( layer == 9 ) return "METAL9";
    if( layer == 8 ) return "METAL8";
    if( layer == 7 ) return "METAL7";
    if( layer == 6 ) return "METAL6";
    if( layer == 5 ) return "METAL5";
    if( layer == 4 ) return "METAL4";
    if( layer == 3 ) return "METAL3";
    if( layer == 2 ) return "METAL2";
    if( layer == 1 ) return "METAL1";
    assert(0);
}
pair<bool, string> RouterUtil::IsBlock(Point<int> LeftDown , Point<int> RightUp)
{
    pair<bool, string> result ;
    get<0>(result) = false;
    get<1>(result) = "";
    for(auto block : BlockMap)
    {
        if( LeftDown.x >= block.second.first.x
           && RightUp.x <= block.second.second.x
           && LeftDown.y >= block.second.first.y
           && RightUp.y <= block.second.second.y )
        {
            get<0>(result) = true;
            get<1>(result) = ComponentMaps[block.first].MACROTYPE;
        }
    }
    return result;
}
string RouterUtil::getLCS(string s1,string s2)
{
    if(s1==""||s2=="")
        return "";
    size_t m=s1.size()+1;
    size_t n=s2.size()+1;
    int lcs[100][100];
    size_t i,j;
    for(i=0;i<m;i++)
        for(j=0;j<n;j++)
            lcs[i][j]=0;
    for(i=1;i<m;i++)
        for(j=1;j<n;j++)
        {
            if(s1[i-1]==s2[j-1])
                lcs[i][j]=lcs[i-1][j-1]+1;
            else
                lcs[i][j]=lcs[i-1][j]>=lcs[i][j-1]?lcs[i-1][j]:lcs[i][j-1];
        }
    i=m-2;
    j=n-2;
    string ss="";
    string ans="";
    while(i!=-1&&j!=-1)
    {
        if(s1[i]==s2[j])
        {
            ss+=s1[i];
            i--;
            j--;
        }
        else
        {
            if(lcs[i+1][j+1]==lcs[i][j])
            {
                i--;
                j--;
            }
            else
            {
                if(lcs[i][j+1]>=lcs[i+1][j])
                    i--;
                else
                    j--;
            }
        }
    }
    reverse(ss.begin(),ss.end());
    return ss ;
}
