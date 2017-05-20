//
//  PDNHelper.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 20/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef PDNHelper_hpp
#define PDNHelper_hpp

#include <stdio.h>
#include "Components.h"
#include <cmath>
#include <string>
using namespace std ;
class PDNHelper {
    
public:
    bool isHorizontal(Point<int> pt1 , Point<int> pt2);
    double calculateResistance(double rpsq , int width , double length );
    pair<Point<int>, Point<int>> getBlockCoordinate(int x , int y , int width , int length  , string orient ) ;
};

#endif /* PDNHelper_hpp */
//for (int i = 0; i < PinNames.size(); i++)
//{
//    for( auto it = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.begin(), end = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.end(); it != end;it = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.upper_bound(it->first))
//    {
//        auto first = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.lower_bound(it->first);
//        auto last = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.upper_bound(it->first);
//        while (first != last)
//        {
//            Line line ;
//            if(myhelper.isHorizontal(first->second.ABSOLUTE_POINT1, first->second.ABSOLUTE_POINT2))
//            {
//                // 水平線 pt1存left pt2存right
//                Point<int> right = (first->second.ABSOLUTE_POINT1.x > first->second.ABSOLUTE_POINT2.x) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
//                Point<int> left = (first->second.ABSOLUTE_POINT1.x < first->second.ABSOLUTE_POINT2.x) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
//                line.MetalName = first->second.METALNAME ;
//                line.Width = first->second.ROUNTWIDTH ;
//                line.pt1 = left ;
//                line.pt2 = right ;
//                HorizontalLines.push_back(line);
//            }
//            else
//            {
//                // 垂直線 pt1存bottom pt2存top
//                Point<int> top = (first->second.ABSOLUTE_POINT1.y > first->second.ABSOLUTE_POINT2.y) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
//                Point<int> bottom = (first->second.ABSOLUTE_POINT1.y < first->second.ABSOLUTE_POINT2.y) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
//                line.MetalName = first->second.METALNAME ;
//                line.Width = first->second.ROUNTWIDTH ;
//                line.pt1 = bottom ;
//                line.pt2 = top ;
//                VerticalLines.push_back(line);
//            }
//            
//            first++;
//            
//        }
//        
//    }
//}
//// 水平線依照y軸排序，y軸越小排越前面
//std::sort(HorizontalLines.begin(), HorizontalLines.end(), [](const Line & line1, const Line & line2)
//          {
//              if( line1.pt1.y < line2.pt1.y )
//                  return true ;
//              else if(line1.pt1.y == line2.pt1.y)
//              {
//                  if(line1.pt1.x < line2.pt1.x)
//                      return true ;
//                  else
//                      return false;
//              }
//              else
//              {
//                  return false ;
//              }
//          });
//// 垂直線依照x軸排序，x軸越小排越前面
//std::sort(VerticalLines.begin(), VerticalLines.end(),[](const Line & line1, const Line & line2 )
//          {
//              if( line1.pt1.x < line2.pt1.x )
//                  return true ;
//              else if(line1.pt1.x == line2.pt1.x)
//              {
//                  if(line1.pt1.y < line2.pt1.y)
//                      return true ;
//                  else
//                      return false;
//              }
//              else
//              {
//                  return false ;
//              }
//          });
