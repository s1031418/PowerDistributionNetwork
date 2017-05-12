//
//  Converter.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 10/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "Converter.hpp"
#include "lefrw.h"
#include "defrw.h"
#include "InitialFileParser.hpp"
#include <iostream>
#include <vector>
#include <assert.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
Converter::Converter()
{
    
}
Converter::~Converter()
{
    
}
void Converter::toSpice()
{
    
}
void Converter::toLocationFile()
{
    // print Block location
    FILE * pFile ;
    pFile = fopen("output.txt", "w");
    if( NULL == pFile ) printf("Failed to open file\n");
    
    
    vector<string> BlockNames ;
    for( auto it = ComponentMaps.begin(), end = ComponentMaps.end(); it != end;it = ComponentMaps.upper_bound(it->first))
    {
        BlockNames.push_back(it->first);
    }
    vector<string> BlockMessage ;
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
        fprintf(pFile, "%s ", MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[begin->first].NAME.c_str());
        fprintf(pFile, "%d %d ", get<0>(coordinate).x , get<0>(coordinate).y);
        fprintf(pFile, "%s ", MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[(end)->first].NAME.c_str());
        fprintf(pFile, "%d %d ", get<1>(coordinate).x , get<1>(coordinate).y);
        fprintf(pFile, "%s_%s_%s\n", BlockNames[i].c_str() , MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[begin->first].NAME.c_str() , MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[(end)->first].NAME.c_str());
//        cout << MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[begin->first].NAME << " ";
//        cout << get<0>(coordinate).x << " " << get<0>(coordinate).y << " ";
//        cout << MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[(end)->first].NAME << " ";
//        cout << get<1>(coordinate).x << " " << get<1>(coordinate).y << " ";
//        cout << BlockNames[i] << "_" << MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[begin->first].NAME << "to"  << MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[(end)->first].NAME ;
//        cout << endl;
    }
    // print nets location
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
                        fprintf(pFile, "%s " ,begin->first.c_str());
//                        cout << begin->first << " ";
                        Point<int> right = (begin->second.ABSOLUTE_POINT1.x > begin->second.ABSOLUTE_POINT2.x) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        Point<int> left = (begin->second.ABSOLUTE_POINT1.x < begin->second.ABSOLUTE_POINT2.x) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        // print left_down_corner
                        fprintf(pFile, "%d %d " ,left.x , left.y - (begin->second.ROUNTWIDTH / 2) );
//                        cout << left.x << " " << left.y - (begin->second.ROUNTWIDTH / 2) << " ";
                        // MetalName
                        fprintf(pFile, "%s " ,begin->first.c_str());
//                        cout << begin->first << " ";
                        // print right_up_corner
                        fprintf(pFile, "%d %d " ,right.x , right.y + (begin->second.ROUNTWIDTH / 2) );
//                        cout << right.x << " " << right.y + (begin->second.ROUNTWIDTH / 2) << " " ;
                    }
                    else
                    {
                        // MetalName
//                        cout << begin->first << " ";
                        fprintf(pFile, "%s " ,begin->first.c_str());
                        Point<int> top = (begin->second.ABSOLUTE_POINT1.y > begin->second.ABSOLUTE_POINT2.y) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        Point<int> bottom = (begin->second.ABSOLUTE_POINT1.y < begin->second.ABSOLUTE_POINT2.y) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        // print left_down_corne
                        fprintf(pFile, "%d %d " , bottom.x - (begin->second.ROUNTWIDTH / 2) , bottom.y);
//                        cout << bottom.x - (begin->second.ROUNTWIDTH / 2) << " " << bottom.y << " ";
                        // MetalName
                        fprintf(pFile, "%s " ,begin->first.c_str());
//                        cout << begin->first << " ";
                        // print right_up_corner
                        fprintf(pFile, "%d %d " , top.x + (begin->second.ROUNTWIDTH / 2) , top.y);
//                        cout << top.x + (begin->second.ROUNTWIDTH / 2) << " " << top.y << " " ;
                    }
                    fprintf(pFile, "R%s_%d \n" , PinNames[i].c_str() , R_cnt);
//                    cout << "R" << PinNames[i] << "_" << R_cnt << " ";
                    
//                    cout << endl;
                    R_cnt++ ;
                }
                begin++;
            }
        }
        fprintf(pFile, "\n");
//        cout << endl ;
        
    }
    fclose(pFile);
}
pair<Point<int>, Point<int>> Converter::getBlockCoordinate(int x , int y , int width , int length  , string orient  )
{
    Point<int> pt1 ;
    Point<int> pt2 ;
    pt1.x = x ;
    pt1.y = y ;
    if( orient == "N" || orient == "S" || orient == "FN" || orient == "FS" )
    {
        pt2.x = x + width ;
        pt2.y = y + length ;
    }
    else if( orient == "W" || orient == "E" || orient == "FW" || orient == "FE"  )
    {
        pt2.x = x + length ;
        pt2.y = y + width ;
    }
    return make_pair(pt1, pt2);
}


bool Converter::isHorizontal(Point<int> pt1 , Point<int> pt2)
{
    if( pt1.y == pt2.y)
        return true ;
    else
        return false ;
}
void Converter::putFile(string FileName , vector<string> Data)
{
    fstream fs(FileName , ios::out);
    if(!fs) cerr << "Failed to open file";
    for(int i = 0 ; i < Data.size() ; i++)
    {
        fs << Data[i] ;
    }
    fs.close();
    
}
