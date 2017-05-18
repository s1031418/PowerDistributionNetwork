//
//  PDN.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 17/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "PDN.hpp"

#include "defrw.h"
#include "lefrw.h"


PDN::PDN()
{
    // create my own data structure
    BuildDataStructure();
}


PDN::~PDN()
{
//    FreeMemory(root);
}

//void PDN::FreeMemory(Root * root)
//{
//    
//}

void PDN::optimize()
{
    
}
void PDN::toSpice()
{
    
}
void PDN::BuildDataStructure()
{
    //root = new Root[1];
    
//    vector<Line> VerticalLine , HorizontalLine ;
//    getLineVec(VerticalLine , HorizontalLine);
//    int firstX = VerticalLine[0].pt1.x ;
//    int firstY = HorizontalLine[0].pt1.y ;
//    ColumnNodePointer cp = new ColumnNode[1];
//    WireNodePointer wp1 = new WireNode[1];
//    WireNodePointer wp2 = new WireNode[1];
//    cp->index = firstX ;
//    wp1->MetalName = VerticalLine[0].MetalName;
//    wp1->pt = VerticalLine[0].pt1 ;
//    wp1->V_Width = VerticalLine[0].Width ;
//    
//    
//    
//    root->ulink = cp ;
//    for(int i = 0 ; i < VerticalLine.size() ; i++)
//    {
//        ColumnNodePointer cp = new ColumnNode[1];
//        cp->index = VerticalLine
//        WireNodePointer wp = new WireNode[1];
//        root->rlink = cp ;
//        
//    }
}
//void PDN::createColumnNode()
//{
//    
//}
//void PDN::getLineVec(vector<Line> & VertialLine_Vec , vector<Line> & HorizontalLine_Vec )
//{
//    vector<string> PinNames;
//    for( auto it = PinMaps.begin(), end = PinMaps.end(); it != end;it = PinMaps.upper_bound(it->first))
//    {
//        PinNames.push_back(it->first);
//    }
//    for (int i = 0; i < PinNames.size(); i++)
//    {
//        for( auto it = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.begin(), end = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.end(); it != end;it = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.upper_bound(it->first))
//        {
//            auto first = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.lower_bound(it->first);
//            auto last = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.upper_bound(it->first);
//            while (first != last)
//            {
//                Line line ;
//                if(isHorizontal(first->second.ABSOLUTE_POINT1, first->second.ABSOLUTE_POINT2))
//                {
//                    // 水平線 pt1存left pt2存right
//                    Point<int> right = (first->second.ABSOLUTE_POINT1.x > first->second.ABSOLUTE_POINT2.x) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
//                    Point<int> left = (first->second.ABSOLUTE_POINT1.x < first->second.ABSOLUTE_POINT2.x) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
//                    line.MetalName = first->second.METALNAME ;
//                    line.Width = first->second.ROUNTWIDTH ;
//                    line.pt1 = left ;
//                    line.pt2 = right ;
//                    HorizontalLine_Vec.push_back(line);
//                }
//                else
//                {
//                    // 垂直線 pt1存bottom pt2存top
//                    Point<int> top = (first->second.ABSOLUTE_POINT1.y > first->second.ABSOLUTE_POINT2.y) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
//                    Point<int> bottom = (first->second.ABSOLUTE_POINT1.y < first->second.ABSOLUTE_POINT2.y) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
//                    line.MetalName = first->second.METALNAME ;
//                    line.Width = first->second.ROUNTWIDTH ;
//                    line.pt1 = bottom ;
//                    line.pt2 = top ;
//                    VertialLine_Vec.push_back(line);
//                }
//                
//                first++;
//                
//            }
//            
//        }
//    }
//    // 水平線依照y軸排序，y軸越小排越前面
//    std::sort(HorizontalLine_Vec.begin(), HorizontalLine_Vec.end(), [](const Line & line1, const Line & line2)
//         {
//             if( line1.pt1.y < line2.pt1.y )
//                 return true ;
//             else if(line1.pt1.y == line2.pt1.y)
//             {
//                 if(line1.pt1.x < line2.pt1.x)
//                     return true ;
//                 else
//                     return false;
//             }
//             else
//             {
//                 return false ;
//             }
//         });
//    // 垂直線依照x軸排序，x軸越小排越前面
//    std::sort(VertialLine_Vec.begin(), VertialLine_Vec.end(),[](const Line & line1, const Line & line2 )
//         {
//             if( line1.pt1.x < line2.pt1.x )
//                 return true ;
//             else if(line1.pt1.x == line2.pt1.x)
//             {
//                 if(line1.pt1.y < line2.pt1.y)
//                     return true ;
//                 else
//                     return false;
//             }
//             else
//             {
//                 return false ;
//             }
//         });
//}
//bool PDN::isHorizontal(Point<int> pt1 , Point<int> pt2)
//{
//    if( pt1.y == pt2.y)
//        return true ;
//    else
//        return false ;
//}
