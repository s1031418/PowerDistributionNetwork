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
#include <assert.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
Converter::Converter()
{
    initConverterState();
}
Converter::~Converter()
{
    
}
void Converter::initConverterState()
{
    // init pin_name
    for( auto it = PinMaps.begin(), end = PinMaps.end(); it != end;it = PinMaps.upper_bound(it->first))
    {
        PinNames.push_back(it->first);
    }
    BuildBlockMaps();
    BuildCrossPointMap();
}
void Converter::initCrossPointMap(map<Line , vector<Point<int>>,MyComparator> & CrossPointMap ,map<string , vector<Line>> & lineMap , string PinName )
{
    Point<int> PowerPinST = PinMaps[PinName].STARTPOINT ;
    for( auto vec : lineMap )
    {
        for(int i = 0 ; i < vec.second.size() ; i++)
        {
            vector<Point<int>> CrossPoints;
            // 判斷是不是起點
            if(vec.second[i].pt1 == PowerPinST || vec.second[i].pt2 == PowerPinST)
                CrossPoints.push_back(PowerPinST);
            // 判斷是不是終點
            Point<int> Result = myhelper.getEndPoint(BlockMaps, vec.second[i].pt1) ;
            if( Result.x != -1 && Result.y != -1 )
                CrossPoints.push_back(Result);
            Result = myhelper.getEndPoint(BlockMaps, vec.second[i].pt2) ;
            if( Result.x != -1 && Result.y != -1 )
                CrossPoints.push_back(Result);
            
            for(int j = 0 ; j < vec.second.size() ; j++)
            {
                if( !vec.second[i].isPsedoLine  )//代表不是psudo line
                {
                    
                    Point<int> CrossPoint = myhelper.getCrossPoint(vec.second[i], vec.second[j]);
                    if( CrossPoint.x != -1 && CrossPoint.y != -1 )
                    {
                        bool isDuplicate = false ;
                        for(int k = 0 ; k < CrossPoints.size() ; k++)
                        {
                            if(CrossPoints[k] == CrossPoint)
                            {
                                isDuplicate = true ;
                                break ;
                            }
                            
                        }
                        if(!isDuplicate)
                            CrossPoints.push_back(CrossPoint);
                    }
                }
            }
            CrossPointMap.insert(make_pair(vec.second[i], CrossPoints));
        }
    }
        
}
void Converter::initLineMap(std::multimap<std::string,Nets> NetsMM ,map<string , vector<Line>> & LineMap)
{
    
    // foreach NetsMM
    for( auto it = NetsMM.begin(), end = NetsMM.end(); it != end;it = NetsMM.upper_bound(it->first))
    {
        auto first = NetsMM.lower_bound(it->second.METALNAME);
        auto last = NetsMM.upper_bound(it->second.METALNAME);
        // foreach NetsMM's Key
        while (first != last) {
            Line line ;
            if( first->second.ROUNTWIDTH == 0 ) // VIA
            {
                line.pt1 = first->second.ABSOLUTE_POINT1 ;
                line.isPsedoLine = true ; 
                for( auto x :ViaMaps[first->second.VIANAME].InnerMaps )
                {
                    if(LayerMaps[x.first].TYPE == "ROUTING")
                    {
                        line.ViaMetal.push_back(x.first);
                        
                        //LineMap[x.first].push_back(line);
                    }
                }
                for(auto layer : line.ViaMetal)
                    LineMap[layer].push_back(line);
                first++ ;
                continue;
            }
            if(myhelper.isHorizontal(first->second.ABSOLUTE_POINT1, first->second.ABSOLUTE_POINT2))
            {
                Point<int> right = ( first->second.ABSOLUTE_POINT1.x > first->second.ABSOLUTE_POINT2.x ) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
                Point<int> left =  ( first->second.ABSOLUTE_POINT1.x < first->second.ABSOLUTE_POINT2.x ) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
                line.pt1 = left ;
                line.pt2 = right ;
                line.isHorizontal = true ;
                line.MetalName = first->second.METALNAME;
                line.Width = first->second.ROUNTWIDTH;
                LineMap[first->second.METALNAME].push_back(line) ;
            }
            else
            {
                Point<int> top = (first->second.ABSOLUTE_POINT1.y > first->second.ABSOLUTE_POINT2.y) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2;
                Point<int> bottom = (first->second.ABSOLUTE_POINT1.y < first->second.ABSOLUTE_POINT2.y) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
                line.pt1 = bottom ;
                line.pt2 = top ;
                line.isHorizontal = false ;
                line.MetalName = first->second.METALNAME;
                line.Width = first->second.ROUNTWIDTH;
                LineMap[first->second.METALNAME].push_back(line) ;
            }
            first++;
        }

    }
}
void Converter::toSpice2()
{
    
    for(auto PinName : PinNames)
    {
        map<string , vector<Line>> lineMap;
        for(auto it = SpecialNetsMaps[PinName].NETSMULTIMAPS.begin() , end = SpecialNetsMaps[PinName].NETSMULTIMAPS.end() ; it != end ; it =  SpecialNetsMaps[PinName].NETSMULTIMAPS.upper_bound(it->first))
        {
            vector<Line> vec_line ;
            lineMap.insert(make_pair(it->first, vec_line));
        }
        initLineMap(SpecialNetsMaps[PinName].NETSMULTIMAPS, lineMap);
        map<Line , vector<Point<int>>,MyComparator> CrossPointMap ;
        initCrossPointMap(CrossPointMap, lineMap , PinName);
        
        print(CrossPointMap);
        
    }
    
    
}
pair<Point<int>, Point<int>> Converter::getOrder(Point<int> pt1, Point<int> pt2)
{
    if(myhelper.isHorizontal(pt1, pt2))
    {
        Point<int> right = ( pt1.x > pt2.x ) ? pt1 : pt2 ;
        Point<int> left =  ( pt1.x < pt2.x ) ? pt1 : pt2 ;
        return make_pair(left, right);
    }
    else
    {
        Point<int> top = ( pt1.y > pt2.y ) ? pt1 : pt2 ;
        Point<int> bottom =  ( pt1.y < pt2.y ) ? pt1 : pt2 ;
        return make_pair(bottom, top);
    }
    assert(0);
}
int Converter::getDistance(Point<int> pt1, Point<int> pt2)
{
    if(myhelper.isHorizontal(pt1, pt2))
        return abs(pt1.x - pt2.x);
    else
        return abs(pt1.y - pt2.y);
}
Point<int> Converter::FlipY(float x_axis , Point<int> pt , FlipOrient orientation)
{
    int x = 0 , y = 0;
    if( orientation == RIGHT)
    {
        float distance = x_axis - pt.x ;
        x = pt.x + 2 * distance;
    }
    else
    {
        float distance = pt.x - x_axis  ;
        x = pt.x - 2 * distance;
    }
    y = pt.y;
    return Point<int>(x,y);
}
Point<int> Converter::FlipX(float y_axis , Point<int> pt , FlipOrient orientation)
{
    
    int x = 0 , y = 0;
    if( orientation == TOP)
    {
        float distance = y_axis - pt.y ;
        y = pt.y + 2 * distance ;
    }
    else
    {
        float distance =  pt.y - y_axis;
        y = pt.y - 2 * distance ;
    }
    x = pt.x ;
    return Point<int>(x,y);

}
pair<Point<int>, Point<int>> Converter::getRotatePoint(Point<int> BlcokLeftDown , Point<int> BlcokRightUp , Point<int> BlockPinLeftDown , Point<int> BlockPinRightUp , string orient)
{
    int BlockWidth = BlcokRightUp.x - BlcokLeftDown.x ;
    int BlockLength = BlcokRightUp.y - BlcokLeftDown.y ;
    int BlockPinWidth = BlockPinRightUp.x - BlockPinLeftDown.x ;
    int BlockPinLength = BlockPinRightUp.y - BlockPinLeftDown.y ;
    float x_axis = BlcokLeftDown.x + (BlockWidth / 2);
    float y_axis = BlcokLeftDown.y + (BlockLength / 2);
    if(orient == "N")
    {
        return make_pair(BlockPinLeftDown, BlockPinRightUp);
    }
    else if (orient == "FN")
    {
        Point<int> RightDown= FlipY(x_axis, BlockPinLeftDown , RIGHT); //變成右下角
        Point<int> LeftUp = FlipY(x_axis, BlockPinRightUp , RIGHT); //變成左上角
        return make_pair(Point<int>( LeftUp.x, RightDown.y), Point<int>( RightDown.x , LeftUp.y ));
    }
    else if (orient == "W")
    {
        int difference = BlockPinLeftDown.y - BlcokLeftDown.y ;
        pair<Point<int>, Point<int>> BlockCoordinate = getBlockCoordinate(BlcokLeftDown.x, BlcokLeftDown.y,BlockWidth , BlockLength, "W");
        Point<int> LeftDown( get<1>(BlockCoordinate).x - difference - BlockPinLength, get<0>(BlockCoordinate).y);
        Point<int> RightUp( get<1>(BlockCoordinate).x - difference, get<0>(BlockCoordinate).y + BlockPinWidth);
        return make_pair(LeftDown, RightUp);
    }
    else if (orient == "FW")
    {
        pair<Point<int>, Point<int>> W = getRotatePoint(BlcokLeftDown, BlcokRightUp, BlockPinLeftDown, BlockPinRightUp, "W");
        pair<Point<int>, Point<int>> BlockCoordinate = getBlockCoordinate(BlcokLeftDown.x, BlcokLeftDown.y,BlockWidth , BlockLength, "W");
        x_axis = get<0>(BlockCoordinate).x + BlockLength / 2 ;
        Point<int> RightDown = FlipY(x_axis, get<0>(W) , LEFT);
        Point<int> LeftUp = FlipY(x_axis, get<1>(W), LEFT);
        return make_pair(Point<int>( LeftUp.x, RightDown.y), Point<int>( RightDown.x , LeftUp.y ));
    }
    else if (orient == "S")
    {
        
        return make_pair( FlipY(x_axis, FlipX(y_axis, BlockPinRightUp, TOP), RIGHT), FlipY(x_axis, FlipX(y_axis, BlockPinLeftDown, TOP), RIGHT));
    }
    else if (orient == "FS")
    {
        pair<Point<int>, Point<int>> S = getRotatePoint(BlcokLeftDown, BlcokRightUp, BlockPinLeftDown, BlockPinRightUp, "S");
        Point<int> RightDown = FlipY(x_axis, get<0>(S), LEFT);
        Point<int> LeftUp = FlipY(x_axis, get<1>(S), LEFT);
        return make_pair(Point<int>( LeftUp.x, RightDown.y), Point<int>( RightDown.x , LeftUp.y ));
    }
    else if (orient == "E")
    {
        int difference = BlockPinLeftDown.y - BlcokLeftDown.y ;
        pair<Point<int>, Point<int>> BlockCoordinate = getBlockCoordinate(BlcokLeftDown.x, BlcokLeftDown.y,BlockWidth , BlockLength, "E");
        return make_pair(Point<int>( get<0>(BlockCoordinate).x + difference, get<1>(BlockCoordinate).y - BlockPinWidth) , Point<int>(get<0>(BlockCoordinate).x + difference + BlockPinLength , get<1>(BlockCoordinate).y));
    }
    else if (orient == "FE")
    {
        pair<Point<int>, Point<int>> E = getRotatePoint(BlcokLeftDown, BlcokRightUp, BlockPinLeftDown, BlockPinRightUp, "E");
        x_axis = BlcokLeftDown.x + BlockLength / 2 ;
        Point<int> LeftUp = FlipY(x_axis, get<1>(E), RIGHT);
        Point<int> RightDown = FlipY(x_axis, get<0>(E), RIGHT);
        return make_pair(Point<int>( LeftUp.x, RightDown.y), Point<int>( RightDown.x , LeftUp.y ));
    }
    assert(0);
    return make_pair(Point<int>(0,0), Point<int>(0,0));
}
void Converter::BuildBlockMaps()
{
    
    for( auto component : ComponentMaps )
    {
        Point<int> BlockLeftDown = component.second.STARTPOINT ;
        Point<int> BlockRightUp ;
        BlockRightUp.x = BlockLeftDown.x + (MacroMaps[component.second.MACROTYPE].WIDTH * UNITS_DISTANCE);
        BlockRightUp.y = BlockLeftDown.y + (MacroMaps[component.second.MACROTYPE].LENGTH * UNITS_DISTANCE);
        string orient = component.second.ORIENT ;
        vector<Block> temp ;
        // init BlockMap ， 把每個key都塞一個空的vector
        BlockMaps.insert(make_pair(component.first,temp));
        for( auto blockpin : MacroMaps[component.second.MACROTYPE].BlockPinMaps)
        {
            
            Block block ;
            for(auto innerlayer : blockpin.second.InnerMaps)
            {
                block.Metals.push_back(innerlayer.first);
            }
            Point<float> LeftDownScaling = (blockpin.second.InnerMaps[block.Metals[0]].pt1) * UNITS_DISTANCE ;
            Point<float> RightUpScaling = (blockpin.second.InnerMaps[block.Metals[0]].pt2) * UNITS_DISTANCE ;
            Point<int> BlockPinLeftDown , BlockPinRightUp;
            BlockPinLeftDown.x = BlockLeftDown.x + LeftDownScaling.x ;
            BlockPinLeftDown.y = BlockLeftDown.y + LeftDownScaling.y ;
            BlockPinRightUp.x = BlockLeftDown.x + RightUpScaling.x ;
            BlockPinRightUp.y = BlockLeftDown.y + RightUpScaling.y ;
            pair<Point<int>, Point<int>> RotatePoint = getRotatePoint(BlockLeftDown , BlockRightUp , BlockPinLeftDown, BlockPinRightUp, orient);
            block.LeftDown = get<0>(RotatePoint) ;
            block.RightUp = get<1>(RotatePoint);
            
            BlockMaps[component.first].push_back(block);
        }
        
    }
}
void Converter::print(map<Line , vector<Point<int>>,MyComparator> & CrossPointMap)
{
    for(auto x : CrossPointMap)
    {
        Line line =x.first ;
        // print via
        
        
        if(x.second.size() == 0) // Psudo Line
        {
            cout << line.ViaMetal[0] << "_" << line.pt1.x << "_" << line.pt1.y  << " " << line.ViaMetal[1] << line.pt1.x << "_" << line.pt1.y << endl;
            continue ;
        }
        
        // print wire
        if(myhelper.isHorizontal(line.pt1, line.pt2))
        {
            sort(x.second.begin(), x.second.end(), [](const Point<int> & pt1, const Point<int> & pt2)
                 {
                     return pt1.x < pt2.x ;
                 });
        }
        else
        {
            sort(x.second.begin(), x.second.end(), [](const Point<int> & pt1, const Point<int> & pt2)
                 {
                     return pt1.y < pt2.y ;
                 });
        }
        for(int i = 0 ; i < x.second.size() -1 ; i++)
            cout << line.MetalName << " " << x.second[i] << " " << x.second[i+1] << endl;
        
    }
    cout << endl;

}
double Converter::calculateResistance(double rpsq , int width , double length )
{
    return rpsq * length / width ;
}
void Converter::toLocationFile()
{
    // print Block location
    FILE * pFile ;
    pFile = fopen("/Users/Jeff/Documents/c++/EDA_Contest2017(PDN)/EDA_Contest2017(PDN)/output.txt", "w");
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
                    
                    
                    if(myhelper.isHorizontal(begin->second.ABSOLUTE_POINT1, begin->second.ABSOLUTE_POINT2))
                    {
                        // MetalName
                        fprintf(pFile, "%s " ,begin->first.c_str());
                        Point<int> right = (begin->second.ABSOLUTE_POINT1.x > begin->second.ABSOLUTE_POINT2.x) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        Point<int> left = (begin->second.ABSOLUTE_POINT1.x < begin->second.ABSOLUTE_POINT2.x) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        // print left_down_corner
                        fprintf(pFile, "%d %d " ,left.x , left.y - (begin->second.ROUNTWIDTH / 2) );
                        // MetalName
                        fprintf(pFile, "%s " ,begin->first.c_str());
                        // print right_up_corner
                        fprintf(pFile, "%d %d " ,right.x , right.y + (begin->second.ROUNTWIDTH / 2) );
                    }
                    else
                    {
                        // MetalName
                        fprintf(pFile, "%s " ,begin->first.c_str());
                        Point<int> top = (begin->second.ABSOLUTE_POINT1.y > begin->second.ABSOLUTE_POINT2.y) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        Point<int> bottom = (begin->second.ABSOLUTE_POINT1.y < begin->second.ABSOLUTE_POINT2.y) ? begin->second.ABSOLUTE_POINT1 : begin->second.ABSOLUTE_POINT2 ;
                        // print left_down_corne
                        fprintf(pFile, "%d %d " , bottom.x - (begin->second.ROUNTWIDTH / 2) , bottom.y);
                        // MetalName
                        fprintf(pFile, "%s " ,begin->first.c_str());
                        // print right_up_corner
                        fprintf(pFile, "%d %d " , top.x + (begin->second.ROUNTWIDTH / 2) , top.y);
                    }
                    fprintf(pFile, "R%s_%d \n" , PinNames[i].c_str() , R_cnt);
                    R_cnt++ ;
                }
                begin++;
            }
        }
        fprintf(pFile, "\n");
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


