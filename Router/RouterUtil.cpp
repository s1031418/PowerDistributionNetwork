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
vector<Rectangle> RouterUtil::getBlockRectangle()
{
    vector<Rectangle> temp ;
    for(auto block : BlockMap)
    {
        Rectangle rect(block.second.LeftDown , block.second.RightUp);
        temp.push_back(rect);
    }
    return temp ; 
}
void RouterUtil::InitBlockMap()
{
    for( auto component : ComponentMaps )
    {
        BlockCoordinate blockCoordinate ;
        pair<Point<int>,Point<int>> coordinate = this->getBlockCoordinate(component.second.STARTPOINT.x, component.second.STARTPOINT.y, MacroMaps[ component.second.MACROTYPE ].WIDTH * UNITS_DISTANCE, MacroMaps[ component.second.MACROTYPE ].LENGTH * UNITS_DISTANCE, component.second.ORIENT);
        blockCoordinate.LeftDown = coordinate.first ;
        blockCoordinate.RightUp = coordinate.second ;
        auto firstLayer = MacroMaps[ component.second.MACROTYPE ].obs.InnerMaps.begin();
        auto lastLayer = --MacroMaps[ component.second.MACROTYPE ].obs.InnerMaps.end();
        int lower = stoi(firstLayer->first.substr(5));
        int upper = stoi(lastLayer->first.substr(5));
        blockCoordinate.lowerMetal = lower ;
        blockCoordinate.upperMetal = upper ;
        BlockMap.insert(make_pair(component.first, blockCoordinate));
    }
}

RouterUtil::~RouterUtil()
{
    
}
int RouterUtil::getRectArea(Rectangle rect1)
{
    int X = rect1.RightUp.x - rect1.LeftDown.x ;
    int Y = rect1.RightUp.y - rect1.LeftDown.y ;
    return X * Y ;
}
CrossInfo RouterUtil::isCrossWithBlock(Rectangle rect1 )
{
    for(auto block : BlockMap)
    {
        Rectangle rect2 ;
        // block 增加boundary的寬度( 1/2 * Width + minimal space )
        rect2.LeftDown = block.second.LeftDown ;
        rect2.RightUp = block.second.RightUp ;
        // 目前minimal space 先 hardcode 2 ，不同層有不同spacing，會浪費resource 
//        rect2.LeftDown.x -= ((0.5*DEFAULT_WIDTH + 2 )*UNITS_DISTANCE);
//        rect2.LeftDown.y -= ((0.5*DEFAULT_WIDTH + 2 )*UNITS_DISTANCE);
//        rect2.RightUp.x += ((0.5*DEFAULT_WIDTH + 2 )*UNITS_DISTANCE);
//        rect2.RightUp.y += ((0.5*DEFAULT_WIDTH + 2 )*UNITS_DISTANCE);
        if( isCross(rect1, rect2) )
        {
            CrossInfo crossinfo ;
            crossinfo.isCross = true ;
            crossinfo.lowerMetal = block.second.lowerMetal ;
            crossinfo.upperMetal = block.second.upperMetal ;
            // 原本的rect 在 block 裡面
            if ( rect1.LeftDown.x >= rect2.LeftDown.x && rect1.LeftDown.x <= rect2.RightUp.x
                && rect1.RightUp.x >= rect2.LeftDown.x && rect1.RightUp.x <= rect2.RightUp.x
                 && rect1.LeftDown.y >= rect2.LeftDown.y && rect1.LeftDown.y <= rect2.RightUp.y
                   && rect1.RightUp.y >= rect2.LeftDown.y && rect1.RightUp.y <= rect2.RightUp.y )
            {
                crossinfo.isUpEdgeBlock = true ;
                crossinfo.isDownEdgeBlock = true ;
                crossinfo.isLeftEdgeBlock = true ;
                crossinfo.isRightEdgeBlock = true ;
            }
            
            // 上面
            // 左右邊都在裡面
            if( rect1.LeftDown.y >= rect2.LeftDown.y && rect1.LeftDown.y <= rect2.RightUp.y && rect1.RightUp.y > rect2.RightUp.y
               && rect1.LeftDown.x >= rect2.LeftDown.x && rect1.LeftDown.x <= rect2.RightUp.x
               && rect1.RightUp.x >= rect2.LeftDown.x && rect1.RightUp.x <= rect2.RightUp.x )
            {
                crossinfo.isLeftEdgeBlock = true ;
                crossinfo.isRightEdgeBlock = true ;
                crossinfo.isDownEdgeBlock = true ;
            }
            // 左邊還在裡面，右邊超過了
            else if( rect1.LeftDown.y >= rect2.LeftDown.y && rect1.LeftDown.y <= rect2.RightUp.y && rect1.RightUp.y > rect2.RightUp.y
                    && rect1.LeftDown.x >= rect2.LeftDown.x && rect1.LeftDown.x <= rect2.RightUp.x
                    && rect1.RightUp.x > rect2.RightUp.x  )
            {
                crossinfo.isLeftEdgeBlock = true ;
                crossinfo.isDownEdgeBlock = true ;
            }
            // 右邊還在裡面，左邊超過了
            else if( rect1.LeftDown.y >= rect2.LeftDown.y && rect1.LeftDown.y <= rect2.RightUp.y && rect1.RightUp.y > rect2.RightUp.y
                    && rect1.RightUp.x >= rect2.LeftDown.x && rect1.RightUp.x <= rect2.RightUp.x
                    && rect1.LeftDown.x < rect2.LeftDown.x )
            {
                crossinfo.isRightEdgeBlock = true ;
                crossinfo.isDownEdgeBlock = true ;
            }
            // 下面
            // 左右邊都在裡面
            
            else if( rect1.RightUp.y >= rect2.LeftDown.y && rect1.RightUp.y <= rect2.RightUp.y && rect1.LeftDown.y < rect2.LeftDown.y
                    && rect1.LeftDown.x >= rect2.LeftDown.x && rect1.LeftDown.x <= rect2.RightUp.x
                    && rect1.RightUp.x >= rect2.LeftDown.x && rect1.RightUp.x <= rect2.RightUp.x )
            {
                crossinfo.isLeftEdgeBlock = true ;
                crossinfo.isRightEdgeBlock = true ;
                crossinfo.isUpEdgeBlock = true ;
            }
            // 左邊還在裡面，右邊超過了
            else if( rect1.RightUp.y >= rect2.LeftDown.y && rect1.RightUp.y <= rect2.RightUp.y && rect1.LeftDown.y < rect2.LeftDown.y
                    && rect1.LeftDown.x >= rect2.LeftDown.x && rect1.LeftDown.x <= rect2.RightUp.x
                    && rect1.RightUp.x > rect2.RightUp.x  )
            {
                crossinfo.isLeftEdgeBlock = true ;
                crossinfo.isUpEdgeBlock = true ;
            }
            // 右邊還在裡面，左邊超過了
            else if( rect1.RightUp.y >= rect2.LeftDown.y && rect1.RightUp.y <= rect2.RightUp.y && rect1.LeftDown.y < rect2.LeftDown.y
                    && rect1.RightUp.x >= rect2.LeftDown.x && rect1.RightUp.x <= rect2.RightUp.x
                    && rect1.LeftDown.x < rect2.LeftDown.x )
            {
                crossinfo.isRightEdgeBlock = true ;
                crossinfo.isUpEdgeBlock = true ;
            }
            // 左面
            
            // 上下邊都在裡面
            else if( rect1.RightUp.x >= rect2.LeftDown.x && rect1.RightUp.x <= rect2.RightUp.x && rect1.LeftDown.x < rect2.LeftDown.x
                    && rect1.LeftDown.y >= rect2.LeftDown.y && rect1.LeftDown.y <= rect2.RightUp.y
                    && rect1.RightUp.y >= rect2.LeftDown.y && rect1.RightUp.y <= rect2.RightUp.y )
            {
                crossinfo.isUpEdgeBlock = true ;
                crossinfo.isDownEdgeBlock = true ;
                crossinfo.isRightEdgeBlock = true ;
            }
            // 上邊還在裡面，下邊超過了
            else if( rect1.RightUp.x >= rect2.LeftDown.x && rect1.RightUp.x <= rect2.RightUp.x && rect1.LeftDown.x < rect2.LeftDown.x
                    && rect1.RightUp.y >= rect2.LeftDown.y && rect1.RightUp.y <= rect2.RightUp.y
                    && rect1.LeftDown.y < rect2.LeftDown.y  )
            {
                crossinfo.isRightEdgeBlock = true ;
                crossinfo.isUpEdgeBlock = true ;
            }
            // 下邊還在裡面，上邊超過了
            else if( rect1.RightUp.x >= rect2.LeftDown.x && rect1.RightUp.x <= rect2.RightUp.x && rect1.LeftDown.x < rect2.LeftDown.x 
                    && rect1.LeftDown.y >= rect2.LeftDown.y && rect1.LeftDown.y <= rect2.RightUp.y
                    && rect1.RightUp.y > rect2.RightUp.y )
            {
                crossinfo.isRightEdgeBlock = true ;
                crossinfo.isDownEdgeBlock = true ;
            }
            // 右面
            
            // 上下邊都在裡面
            else if( rect1.LeftDown.x >= rect2.LeftDown.x && rect1.LeftDown.x <= rect2.RightUp.x && rect1.RightUp.x > rect2.RightUp.x
                    && rect1.LeftDown.y >= rect2.LeftDown.y && rect1.LeftDown.y <= rect2.RightUp.y
                    && rect1.RightUp.y >= rect2.LeftDown.y && rect1.RightUp.y <= rect2.RightUp.y )
            {
                crossinfo.isUpEdgeBlock = true ;
                crossinfo.isDownEdgeBlock = true ;
                crossinfo.isLeftEdgeBlock = true ;
            }
            // 上邊還在裡面，下邊超過了
            else if( rect1.LeftDown.x >= rect2.LeftDown.x && rect1.LeftDown.x <= rect2.RightUp.x && rect1.RightUp.x > rect2.RightUp.x
                    && rect1.RightUp.y >= rect2.LeftDown.y && rect1.RightUp.y <= rect2.RightUp.y
                    && rect1.LeftDown.y < rect2.LeftDown.y  )
            {
                crossinfo.isLeftEdgeBlock = true ;
                crossinfo.isUpEdgeBlock = true ;
            }
            // 下邊還在裡面，上邊超過了
            else if( rect1.LeftDown.x >= rect2.LeftDown.x && rect1.LeftDown.x <= rect2.RightUp.x && rect1.RightUp.x > rect2.RightUp.x
                    && rect1.LeftDown.y >= rect2.LeftDown.y && rect1.LeftDown.y <= rect2.RightUp.y
                    && rect1.RightUp.y > rect2.RightUp.y )
            {
                crossinfo.isLeftEdgeBlock = true ;
                crossinfo.isDownEdgeBlock = true ;
            }
            return crossinfo ;
            
        }
    }
    return CrossInfo();
}
int RouterUtil::getCrossArea(Rectangle rect1 , Rectangle rect2)
{
    
    int innerLeft = rect1.LeftDown.x > rect2.LeftDown.x ? rect1.LeftDown.x : rect2.LeftDown.x ;
    int innerRight = rect1.RightUp.x < rect2.RightUp.x ? rect1.RightUp.x : rect2.RightUp.x ;
    int innerTop = rect1.RightUp.y < rect2.RightUp.y ? rect1.RightUp.y : rect2.RightUp.y;
    int innerBottom = rect1.LeftDown.y > rect2.LeftDown.y ? rect1.LeftDown.y : rect2.LeftDown.y;
    
    
    int innerWidth = innerTop > innerBottom ? (innerTop - innerBottom) : 0;
    int innerHeight = innerRight > innerLeft ? (innerRight - innerLeft) : 0;
    int innerArea = innerWidth * innerHeight;
    return innerArea ;
//    return (rect1.RightUp.x - rect1.LeftDown.x) * (rect1.RightUp.y - rect1.LeftDown.y) + (rect2.RightUp.x - rect2.LeftDown.x) * (rect2.RightUp.y - rect2.LeftDown.y) - innerArea;
}
Rectangle RouterUtil::getCrossRect(Rectangle rect1 , Rectangle rect2)
{
    if( !isCross(rect1, rect2) )
        return Rectangle( Point<int>(-1,-1)  , Point<int>(-1,-1) );
    
    
    int innerLeft = rect1.LeftDown.x > rect2.LeftDown.x ? rect1.LeftDown.x : rect2.LeftDown.x ;
    int innerRight = rect1.RightUp.x < rect2.RightUp.x ? rect1.RightUp.x : rect2.RightUp.x ;
    int innerTop = rect1.RightUp.y < rect2.RightUp.y ? rect1.RightUp.y : rect2.RightUp.y;
    int innerBottom = rect1.LeftDown.y > rect2.LeftDown.y ? rect1.LeftDown.y : rect2.LeftDown.y;
    
    return Rectangle( Point<int>( innerLeft , innerBottom ) , Point<int>( innerRight , innerTop ) );
//    int innerHeight = innerTop > innerBottom ? (innerTop - innerBottom) : 0;
//    int innerWidth = innerRight > innerLeft ? (innerRight - innerLeft) : 0;
//    int innerArea = innerWidth * innerHeight;
    
    
}
bool RouterUtil::isCross(Rectangle rect1 , Rectangle rect2)
{
    
    pair<int, int> rect1Center = make_pair( ( rect1.LeftDown.x + rect1.RightUp.x ) / 2 , ( rect1.LeftDown.y + rect1.RightUp.y ) / 2 );
    pair<int, int> rect2Center = make_pair( ( rect2.LeftDown.x + rect2.RightUp.x ) / 2 , ( rect2.LeftDown.y + rect2.RightUp.y ) / 2 );
    int verticalDistance = abs(rect1Center.second - rect2Center.second);
    int horizontalDistance = abs(rect1Center.first - rect2Center.first);
    int rect1Width = rect1.RightUp.x - rect1.LeftDown.x ;
    int rect1Height = rect1.RightUp.y - rect1.LeftDown.y ;
    int rect2Width = rect2.RightUp.x - rect2.LeftDown.x ;
    int rect2Height = rect2.RightUp.y - rect2.LeftDown.y ;
    int verticalThreshold = (rect1Height + rect2Height)/2 ;
    int horizontalThreshold = (rect1Width + rect2Width)/2 ;
    if( verticalDistance > verticalThreshold || horizontalDistance > horizontalThreshold )
        return false ; 
    return true ; 
    
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
//    get<0>(result) = false;
//    get<1>(result) = "";
//    for(auto block : BlockMap)
//    {
//        if( LeftDown.x >= block.second.first.x
//           && RightUp.x <= block.second.second.x
//           && LeftDown.y >= block.second.first.y
//           && RightUp.y <= block.second.second.y )
//        {
//            get<0>(result) = true;
//            get<1>(result) = ComponentMaps[block.first].MACROTYPE;
//        }
//    }
    return result;
}
string RouterUtil::getAlias(string MetalName)
{
    if( MetalName == "METAL15" ) return "M15";
    if( MetalName == "METAL14" ) return "M14";
    if( MetalName == "METAL13" ) return "M13";
    if( MetalName == "METAL12" ) return "M12";
    if( MetalName == "METAL11" ) return "M11";
    if( MetalName == "METAL10" ) return "M10";
    if( MetalName == "METAL9" ) return "M9";
    if( MetalName == "METAL8" ) return "M8";
    if( MetalName == "METAL7" ) return "M7";
    if( MetalName == "METAL6" ) return "M6";
    if( MetalName == "METAL5" ) return "M5";
    if( MetalName == "METAL4" ) return "M4";
    if( MetalName == "METAL3" ) return "M3";
    if( MetalName == "METAL2" ) return "M2";
    if( MetalName == "METAL1" ) return "M1";
    assert(0);
}
int RouterUtil::getWeights(int layer)
{
    string key = getAlias(translateIntToMetalName(layer));
    int weights = stod(WeightsMaps[key])*100;
    return weights; 
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
vector<int> RouterUtil::getPowerPinGridCandidate(Rectangle rect)
{
    vector<int> candidates ;
    int xLowerBound = rect.LeftDown.x / DEFAULT_PITCH ;
    int xUpperBound = rect.RightUp.x / DEFAULT_PITCH;
    vector<int> xCandidates ;
    for( int i = xLowerBound + 1 ; i <= xUpperBound ; i += DEFAULT_PITCH )
        xCandidates.push_back(i);
    int yLowerBound = rect.LeftDown.y / DEFAULT_PITCH ;
    int yUpperBound = rect.RightUp.y / DEFAULT_PITCH;
    vector<int> yCandidates ;
    for( int i = yLowerBound + 1 ; i <= yUpperBound ; i += DEFAULT_PITCH )
        yCandidates.push_back(i);
    return vector<int>();
}
