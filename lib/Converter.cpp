//
//  Converter.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 10/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "Converter.hpp"
#include "lefrw.h"
#include "Debugger.hpp"
#include "defrw.h"
#include "InitialFileParser.hpp"
#include <iostream>
#include <assert.h>

Converter::Converter()
{
    
}
Converter::~Converter()
{
    
}
void Converter::toSpice()
{
    
    
    
    
    
}
void Converter::toDebugMsg()
{
    // print Block location
    //cout << "Blocks:" << endl;
    vector<string> BlockNames;
    for( auto it = ComponentMaps.begin(), end = ComponentMaps.end(); it != end;it = ComponentMaps.upper_bound(it->first))
    {
        BlockNames.push_back(it->first);
    }
    for(int i = 0 ; i < BlockNames.size() ; i++)
    {
        
        
        pair<Point<int>,Point<int>> coordinate = getBlockCoordinate(ComponentMaps[BlockNames[i]].STARTPOINT.x ,
                                                               ComponentMaps[BlockNames[i]].STARTPOINT.y ,
                                                               UNITS_DISTANCE * MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].WIDTH ,
                                                               UNITS_DISTANCE * MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].LENGTH ,
                                                               ComponentMaps[BlockNames[i]].ORIENT );
        auto begin = MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps.begin();
        auto end = MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps.end();
        end-- ; 
        cout << MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[begin->first].NAME << " ";
        cout << get<0>(coordinate).x << " " << get<0>(coordinate).y << " ";
        cout << MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[(end)->first].NAME << " ";
        cout << get<1>(coordinate).x << " " << get<1>(coordinate).y << " ";
        cout << BlockNames[i] << "_" << MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[begin->first].NAME << "to"  << MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[(end)->first].NAME ;
        cout << endl;
    }
    // print nets location
    //cout << "Nets:" << endl;
    vector<string> PinNames;
    for( auto it = PinMaps.begin(), end = PinMaps.end(); it != end;it = PinMaps.upper_bound(it->first))
    {
        PinNames.push_back(it->first);
    }
    for (int i = 0; i < PinNames.size(); i++) {
        int R_cnt = 1 ; // Resistance Count
        for( auto it = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.begin(), end = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.end(); it != end;it = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.upper_bound(it->first))
        {
            
            auto begin = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.lower_bound(it->first);
            auto end2 = SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS.upper_bound(it->first);
            while (begin != end2) {
                if( begin->second.ABSOLUTE_POINT2.x != -1 && begin->second.ABSOLUTE_POINT2.y != -1 ) // not print via message
                {
                    
                    
                    if(isHorizontal(begin->second.ABSOLUTE_POINT1, begin->second.ABSOLUTE_POINT2))
                    {
                        // MetalName
                        cout << begin->first << " ";
                        Point<int> right = (begin->second.ABSOLUTE_POINT1.x > begin->second.ABSOLUTE_POINT2.x) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        Point<int> left = (begin->second.ABSOLUTE_POINT1.x < begin->second.ABSOLUTE_POINT2.x) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        // print left_down_corner
                        cout << left.x << " " << left.y - (begin->second.ROUNTWIDTH / 2) << " ";
                        // MetalName
                        cout << begin->first << " ";
                        // print right_up_corner
                        cout << right.x << " " << right.y + (begin->second.ROUNTWIDTH / 2) << " " ;
                    }
                    else
                    {
                        // MetalName
                        cout << begin->first << " ";
                        Point<int> top = (begin->second.ABSOLUTE_POINT1.y > begin->second.ABSOLUTE_POINT2.y) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        Point<int> bottom = (begin->second.ABSOLUTE_POINT1.y < begin->second.ABSOLUTE_POINT2.y) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        // print left_down_corne
                        cout << bottom.x - (begin->second.ROUNTWIDTH / 2) << " " << bottom.y << " ";
                        // MetalName
                        cout << begin->first << " ";
                        // print right_up_corner
                        cout << top.x + (begin->second.ROUNTWIDTH / 2) << " " << top.y << " " ;
                    }
                    
                    cout << "R" << PinNames[i] << "_" << R_cnt << " ";
//                    cout << begin->second.ABSOLUTE_POINT1.x << " ";
//                    cout << begin->second.ABSOLUTE_POINT1.y << " ";
//                    cout << begin->second.ABSOLUTE_POINT2.x << " ";
//                    cout << begin->second.ABSOLUTE_POINT2.y ;
                    // x1 , y1
                    
                    // x2 , y2
                    
                    cout << endl;
                    R_cnt++ ;
                }
                begin++;
            }
        }
        cout << endl ;
        
    }
}
pair<Point<int>, Point<int>> Converter::getBlockCoordinate(int x , int y , int width , int length  , string orient  )
{
    Point<int> pt1 ;
    Point<int> pt2 ;
    pair<int,int> direction = getDirection(orient , width , length );
    pt1.x = x ;
    pt1.y = y ;
    pt2.x = x + get<0>(direction) ;
    pt2.y = y + get<1>(direction) ;
    // decide left_down_corner and right_up_corner ;
    if( orient == "N" || orient == "S" || orient == "FW" || orient == "FE" )
    {
        if( pt1.x < pt2.x && pt1.y < pt2.y )
        {
            // pt1 is left_down_corner , pt2 is right_up_corner
            return make_pair(pt1, pt2);
        }
        else
        {
            // pt2 is left_down_corner , pt1 is right_up_corner
            return make_pair(pt2, pt1);
        }
    }
    else if( orient == "W" || orient == "E" || orient == "FN" || orient == "FS"  )
    {
        // pt3 is left_down_corner , pt4 is right_up corner
        Point<int> pt3 ;
        Point<int> pt4 ;
        if( pt1.x < pt2.x && pt1.y > pt2.y )
        {
            // pt1 is left_up_corner , pt2 is right_down_corner
            pt3.x = pt1.x ;
            pt3.y = pt2.y ;
            pt4.x = pt2.x ;
            pt4.y = pt1.y ;
        }
        else
        {
            // pt1 is right_down_corner , pt2 is left_up_corner
            pt3.x = pt2.x ;
            pt3.y = pt1.y ;
            pt4.x = pt1.x ;
            pt4.y = pt2.y ;
        }
        return make_pair(pt3, pt4);
    }
   
    assert(0);
    return make_pair(pt1, pt2);
}
pair<int,int> Converter::getDirection(string orient , int  width , int  length )
{
    int x = 0 , y = 0 ;
    
    
    if(orient == "N")
    {
        x = width ;
        y = length ;
    }
    else if(orient == "S")
    {
        x = -1 * width ;
        y = -1 * length ;
        
    }
    else if(orient == "W")
    {
        x = -1 * length ;
        y = width ;
        
    }
    else if(orient == "E")
    {
        x = length ;
        y = -1 * width ;
    }
    else if(orient == "FN")
    {
        x = -1 * width ;
        y = length ;
    }
    else if(orient == "FS")
    {
        x = width ;
        y = -1 * length ;
    }
    else if(orient == "FW")
    {
        x = length ;
        y = width ;
    }
    else if(orient == "FE")
    {
        x = -1 * length ;
        y = -1 * width ;
    }
    return make_pair(x, y);
}
bool Converter::isHorizontal(Point<int> pt1 , Point<int> pt2)
{
    if( pt1.y == pt2.y)
        return true ;
    else
        return false ;
}