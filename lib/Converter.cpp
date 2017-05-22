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
Converter::Converter(string casename)
{
    CaseName = casename ;
    initConverterState();
}
Converter::~Converter()
{
    
}
void Converter::BuildPowerMaps()
{
    // key pinName , value block
    for(auto pin : PinMaps)
    {
        string orient = pin.second.ORIENT ;
        Block block ;
        pair< Point<int>, Point<int> > PowerPinCoordinate = getPowerPinCoordinate(pin.second.STARTPOINT.x, pin.second.STARTPOINT.y, pin.second.RELATIVE_POINT1 , pin.second.RELATIVE_POINT2, orient);
        block.LeftDown = get<0>(PowerPinCoordinate);
        block.RightUp = get<1>(PowerPinCoordinate);
        PowerMaps.insert(make_pair(pin.first, block));
    }
}
pair< Point<int>, Point<int> > Converter::getPowerPinCoordinate(int x , int y , Point<int> r_pt1, Point<int> r_pt2  , string orient)
{
    Point<int> pt1 ;
    Point<int> pt2 ;
    Point<int> LeftDown;
    Point<int> RightUp;
    if (orient == "E")
    {
        pt1.x = x + r_pt1.y ;
        pt1.y = y - r_pt1.x ;
        pt2.x = x + r_pt2.y ;
        pt2.y = y - r_pt2.x ;
    }
    else if( orient == "N")
    {
        pt1.x = x + r_pt1.x ;
        pt2.x = x + r_pt2.x ;
        pt1.y = y + r_pt1.y ;
        pt2.y = y + r_pt2.y ;
    }
    else if (orient == "S")
    {
        pt1.x = x - r_pt1.x ;
        pt2.x = x - r_pt2.x ;
        pt1.y = y - r_pt1.y ;
        pt2.y = y - r_pt2.y ;
    }
    else if (orient == "W")
    {
        pt1.x = x - r_pt1.y ;
        pt2.x = x - r_pt2.y ;
        pt1.y = y + r_pt1.x ;
        pt2.y = y + r_pt2.x ;
    }
    else if (orient == "FN")
    {
        pt1.x = x - r_pt1.x ;
        pt2.x = x - r_pt2.x ;
        pt1.y = y + r_pt1.y ;
        pt2.y = y + r_pt2.y ;
    }
    else if(orient == "FS")
    {
        pt1.x = x + r_pt1.x ;
        pt2.x = x + r_pt2.x ;
        pt1.y = y - r_pt1.y ;
        pt2.y = y - r_pt2.y ;
    }
    else if(orient == "FE")
    {
        pt1.x = x - r_pt1.y;
        pt2.x = x - r_pt2.y;
        pt1.y = y - r_pt1.x ;
        pt2.y = y - r_pt2.x ;
    }
    else if(orient == "FW")
    {
        pt1.x = x + r_pt1.y ;
        pt2.x = x + r_pt2.y ;
        pt1.y = y + r_pt1.x;
        pt2.y = y + r_pt2.x;
    }
    LeftDown.x = (pt1.x < pt2.x ) ? pt1.x : pt2.x ;
    LeftDown.y = (pt1.y < pt2.y ) ? pt1.y : pt2.y ;
    RightUp.x = (pt1.x > pt2.x ) ? pt1.x : pt2.x ;
    RightUp.y = (pt1.y > pt2.y ) ? pt1.y : pt2.y ;
    return make_pair(LeftDown, RightUp);
}
void Converter::initConverterState()
{
    
    // init pin_name
    for( auto it = PinMaps.begin(), end = PinMaps.end(); it != end;it = PinMaps.upper_bound(it->first))
    {
        PinNames.push_back(it->first);
    }
    BuildBlockMaps();
    BuildPowerMaps();
}
void Converter::initCrossPointMap(map<Line , vector<Point<int>>,MyComparator> & CrossPointMap ,map<string , vector<Line>> & lineMap , string PinName , pair<string, Point<int>> & Voltage_Msg , vector<pair<string, Point<int>>> & Current_Msg)
{
    for( auto vec : lineMap )
    {
        for(int i = 0 ; i < vec.second.size() ; i++)
        {
            
            vector<Point<int>> CrossPoints;
            // 判斷是不是起點
            Point<int> Result = myhelper.getStartPoint(PowerMaps, vec.second[i].pt1 ,vec.second[i].pt2 );
            if( Result != Point<int>() )
            {
                CrossPoints.push_back(Result);
                get<0>(Voltage_Msg) = vec.second[i].MetalName;
                get<1>(Voltage_Msg) = Result ;
            }
            // 判斷是不是終點
            Result = myhelper.getEndPoint(BlockMaps, vec.second[i].pt1 , vec.second[i].pt2) ;
            if( Result != Point<int>() )
            {
                pair<string, Point<int>> temp ;
                get<0>(temp) = vec.second[i].MetalName ;
                get<1>(temp) = Result;
                if( Current_Msg.empty() ) Current_Msg.push_back(temp);
                
                if( !Current_Msg.empty() )
                {
                    //判斷有重複就不能insert
                    bool insert = true;
                    for(auto C : Current_Msg)
                    {
                        if( get<0>(C) == get<0>(temp) && get<1>(C) == get<1>(temp))
                            insert = false;
                    }
                    if(insert)
                        Current_Msg.push_back(temp);
                }
                CrossPoints.push_back(Result);
            }
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
void Converter::initLineMap(std::multimap<std::string,Nets> NetsMM ,map<string , vector<Line>> & LineMap , vector<Line> & ViaTable)
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
                line.ViaName = first->second.VIANAME ;
                for( auto x :ViaMaps[first->second.VIANAME].InnerMaps )
                {
                    if(LayerMaps[x.first].TYPE == "ROUTING")
                    {
                        line.ViaMetal.push_back(x.first);
                    }
                }
                for(auto layer : line.ViaMetal)
                    LineMap[layer].push_back(line);
                ViaTable.push_back(line);
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
void Converter::toSpice()
{
    FILE * pFile ;
    string output = CaseName + ".sp" ;
    pFile = fopen(output.c_str(), "w");
    if( NULL == pFile ) printf("Failed to open file\n");
    fprintf(pFile, "#Comments\n");
//    cout << "#Comments" << endl;
    for(auto PinName : PinNames)
    {
        map<string , vector<Line>> lineMap;
        for(auto it = SpecialNetsMaps[PinName].NETSMULTIMAPS.begin() , end = SpecialNetsMaps[PinName].NETSMULTIMAPS.end() ; it != end ; it =  SpecialNetsMaps[PinName].NETSMULTIMAPS.upper_bound(it->first))
        {
            vector<Line> vec_line ;
            lineMap.insert(make_pair(it->first, vec_line));
        }
        pair<string, Point<int>> Voltage_Msg ;
        vector<pair<string, Point<int>>> Current_Msg;
        vector<Line> ViaTable ;
        initLineMap(SpecialNetsMaps[PinName].NETSMULTIMAPS, lineMap , ViaTable);
        map<Line , vector<Point<int>>,MyComparator> CrossPointMap ;
        initCrossPointMap(CrossPointMap, lineMap , PinName , Voltage_Msg , Current_Msg);
        
        printResistance(CrossPointMap , PinName , ViaTable , pFile);
        printVoltage(get<0>(Voltage_Msg), get<1>(Voltage_Msg), PinName , pFile);
        printCurrent(Current_Msg , PinName , pFile);
    }
    fprintf(pFile, ".tran 1ns 1ns\n");
    fprintf(pFile, ".end\n");
    fprintf(pFile, ".control\n");
    fprintf(pFile, "set noaskquit\n");
    fprintf(pFile, "run\n");
    fprintf(pFile, "quit\n");
    fprintf(pFile, ".enddc\n");
//    cout << ".tran 1ns 1ns" << endl;
//    cout << ".end" << endl;
//    cout << ".control" << endl;
//    cout << "set noaskquit" << endl;
//    cout << "run" << endl;
//    cout << "quit" << endl;
//    cout << ".enddc" << endl;
    
    fclose(pFile);
    string cmd;
    cmd.append("./ngspice ").append(CaseName).append(".sp -o ").append(CaseName).append("_ngspice");
    cout << cmd << endl;
    system(cmd.c_str());
}
void Converter::printVoltage(string MetalName , Point<int> StartPoint , string PinName , FILE * pFile)
{
//    fprintf(pFile, "a\n");
    fprintf(pFile, "V_%s_1 %s_%d_%d gnd %s\n" , PinName.c_str() , MetalName.c_str() , StartPoint.x , StartPoint.y , VoltageMaps[PinName].c_str());
//    cout << "V_" << PinName << "_1 " << MetalName << "_" << StartPoint.x << "_" << StartPoint.y << " gnd " << VoltageMaps[PinName] << endl ;
    
}
void Converter::printCurrent(vector<pair<string, Point<int>>> & Current_Msg , string PinName , FILE * pFile )
{
    int cnt = 1 ;
    
    for(auto C : Current_Msg)
    {
        string title("I" + PinName + "_");
        title.append(to_string(cnt));
        fprintf(pFile, "%s %s_%d_%d gnd " , title.c_str() , get<0>(C).c_str(), get<1>(C).x , get<1>(C).y);
//        cout << title << " " << get<0>(C) << "_" << get<1>(C).x << "_" << get<1>(C).y << " ";
//        cout << "gnd " ;
        pair<string, string> BlockMsg = myhelper.getBlockMsg(BlockMaps, get<1>(C));
        auto begin = CurrnetMaps.lower_bound(get<0>(BlockMsg));
        auto end = CurrnetMaps.upper_bound(get<0>(BlockMsg));
        while (begin != end)
        {
            if(begin->second.NAME == get<1>(BlockMsg))
            {
                fprintf(pFile, "%f" , stof( begin->second.CURRENTDRAWN )/ 1000 );
//                cout << (stof( begin->second.CURRENTDRAWN )/ 1000) ;
            }
            begin++;
        }
        fprintf(pFile, "\n");
//        cout << endl;
        cnt++ ;
    }
    fprintf(pFile, "\n");
//    cout << endl;
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
            block.BlockPinName = blockpin.second.Name;
            BlockMaps[component.first].push_back(block);
        }
        
    }
   
}
void Converter::printResistance(map<Line , vector<Point<int>>,MyComparator> & CrossPointMap , string PinName , vector<Line> & ViaTable , FILE * pFile)
{
    
    int cnt =1 ;
    // print wire
    for(auto x : CrossPointMap)
    {
        Line line =x.first ;
        if(x.second.size() == 0) // Psudo Line
        {
            continue ;
        }
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
        {
            string title("R" + PinName + "_");
            title.append(to_string(cnt));
            fprintf(pFile, "%s %s_%d_%d %s_%d_%d " , title.c_str() , line.MetalName.c_str() , x.second[i].x , x.second[i].y , line.MetalName.c_str() , x.second[i+1].x , x.second[i+1].y);
//            cout << title << " " ;
//            cout << line.MetalName << "_" << x.second[i].x << "_" << x.second[i].y << " ";
//            cout << line.MetalName << "_" << x.second[i+1].x << "_" << x.second[i+1].y << " ";
            double length = ( line.isHorizontal ) ? x.second[i+1].x - x.second[i].x : x.second[i+1].y - x.second[i].y ;
            fprintf(pFile, "%g\n" , myhelper.calculateResistance(LayerMaps[line.MetalName].RESISTANCE_RPERSQ, line.Width, length));
//            cout << myhelper.calculateResistance(LayerMaps[line.MetalName].RESISTANCE_RPERSQ, line.Width, length);
//            cout << endl;
            cnt++;
        }
        
    }
    // print via
    for(auto via : ViaTable)
    {
        string title("R" + PinName + "_");
        title.append(to_string(cnt));
        fprintf(pFile, "%s %s_%d_%d %s_%d_%d %g\n" , title.c_str() , via.ViaMetal[0].c_str() , via.pt1.x , via.pt1.y , via.ViaMetal[1].c_str() , via.pt1.x , via.pt1.y , ViaMaps[via.ViaName].RESISTANCE);
//        cout << title << " " ;
//        cout << via.ViaMetal[0] << "_" << via.pt1.x << "_" << via.pt1.y  << " ";
//        cout << via.ViaMetal[1] << "_" << via.pt1.x << "_" << via.pt1.y << " ";
//        cout << ViaMaps[via.ViaName].RESISTANCE ;
//        cout << endl;
        cnt++;
    }
}

void Converter::toLocationFile()
{
    toSpice();
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
        fprintf(pFile, "%s~%s\n" , MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[begin->first].NAME.c_str() , MacroMaps[ ComponentMaps[BlockNames[i]].MACROTYPE ].obs.InnerMaps[(end)->first].NAME.c_str());
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
                    fprintf(pFile, "*\n" );
//                    fprintf(pFile, "R%s_%d \n" , PinNames[i].c_str() , R_cnt);
                    R_cnt++ ;
                }
                begin++;
            }
        }
        fprintf(pFile, "\n");
    }
    fclose(pFile);
    ngspice ng(CaseName) ;
    ng.concat() ;
    system("java -jar JavaApplication5.jar");
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


