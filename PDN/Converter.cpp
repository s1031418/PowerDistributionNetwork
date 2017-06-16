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
void Converter::initConverterState()
{
    
    // init pin_name
    for( auto it = PinMaps.begin(), end = PinMaps.end(); it != end;it = PinMaps.upper_bound(it->first))
    {
        PinNames.push_back(it->first);
    }
}
void Converter::initCrossPointMap(map<Line , vector<Point<int>>,MyComparator> & CrossPointMap ,map<string , vector<Line>> & lineMap , string PinName , pair<string, Point<int>> & Voltage_Msg , vector<pair<string, Point<int>>> & Current_Msg)
{
    for( auto vec : lineMap )
    {
        for(int i = 0 ; i < vec.second.size() ; i++)
        {
            
            vector<Point<int>> CrossPoints;
            // 判斷是不是起點
            Point<int> Result = myhelper.getStartPoint(vec.second[i].pt1 ,vec.second[i].pt2 );
            if( Result != Point<int>() )
            {
                CrossPoints.push_back(Result);
                get<0>(Voltage_Msg) = vec.second[i].MetalName;
                get<1>(Voltage_Msg) = Result ;
            }
            // 判斷是不是終點
            Result = myhelper.getEndPoint(vec.second[i].pt1 , vec.second[i].pt2) ;
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
void Converter::printOutputFile()
{
    FILE * pFile = nullptr ;
    pFile = fopen("output_files", "w");
    fprintf(pFile, "# The metal usage report\n" );
    long double TotalUsage = 0 ;
    
    for(auto metal : MetalUsage)
    {
        
        string alias = getAlias(metal.first);
        fprintf(pFile, "%s %Lf\n" , alias.c_str() , metal.second);
        TotalUsage += stod(WeightsMaps[alias]) * metal.second ;
    }
    
    fprintf(pFile, "Total %Lf\n" , TotalUsage);
    fclose(pFile);
}
string Converter::getAlias(string name)
{
    if( name == "METAL1" ) return "M1";
    if( name == "METAL2" ) return "M2";
    if( name == "METAL3" ) return "M3";
    if( name == "METAL4" ) return "M4";
    if( name == "METAL5" ) return "M5";
    if( name == "METAL6" ) return "M6";
    if( name == "METAL7" ) return "M7";
    if( name == "METAL8" ) return "M8";
    if( name == "METAL9" ) return "M9";
    if( name == "METAL10" ) return "M10";
    if( name == "METAL11" ) return "M11";
    if( name == "METAL12" ) return "M12";
    if( name == "METAL13" ) return "M13";
    if( name == "METAL14" ) return "M14";
    if( name == "METAL15" ) return "M15";
    // 假如METAL超過15層
    assert(0);
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
        // key是Metal層
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
        ///////////////////////////////////////////////////////
        // 為了產生 IR_DROP通不通過 ，不是必須的
        for( int i = 0 ; i < Current_Msg.size() ; i++ )
        {
            string metal = get<0>(Voltage_Msg);
            Point<int> Point = get<1>(Voltage_Msg);
            transform(metal.begin(), metal.end(), metal.begin(), ::tolower); // transform metal to lowercase
            string key = metal.append("_").append(to_string(Point.x).append("_").append(to_string(Point.y)) );
            metal = get<0>(Current_Msg[i]);
            Point = get<1>(Current_Msg[i]);
            transform(metal.begin(), metal.end(), metal.begin(), ::tolower); // transform metal to lowercase
            string value = metal.append("_").append(to_string(Point.x).append("_").append(to_string(Point.y)) );
            DestinationMap.insert(make_pair(key, value));
        }
        ///////////////////////////////////////////////////////
        caluMetalUse(lineMap);
        printResistance(CrossPointMap , PinName , ViaTable , pFile);
        printVoltage(get<0>(Voltage_Msg), get<1>(Voltage_Msg), PinName , pFile);
        printCurrent(Current_Msg , PinName , pFile);
    }
    printOutputFile();
    
    
    fprintf(pFile, ".tran 1ns 1ns\n");
    fprintf(pFile, ".end\n");
    fprintf(pFile, ".control\n");
    fprintf(pFile, "set noaskquit\n");
    fprintf(pFile, "run\n");
    fprintf(pFile, "quit\n");
    fprintf(pFile, ".enddc\n");
    fclose(pFile);
    
}
void Converter::toOutputFile()
{
    FILE * pFile = nullptr ;
    pFile = fopen("output_files", "w");
    fprintf(pFile, "# The metal usage report\n" );
    long double TotalUsage = 0 ;
    
    for(auto metal : MetalUsage)
    {
        
        string alias = getAlias(metal.first);
        fprintf(pFile, "%s %Lf\n" , alias.c_str() , metal.second);
        TotalUsage += stod(WeightsMaps[alias]) * metal.second ;
    }
    
    fprintf(pFile, "Total %Lf\n" , TotalUsage);
    fprintf(pFile, "\n\n");
    fprintf(pFile, "# The IR drop of each power pin (%)\n" );
    for( auto x : ng.DropMap )
    {
        fprintf(pFile, "%s/%s %f\n" , x.first.first.c_str() , x.first.second.c_str() , x.second);
    }
    
    
    
    fclose(pFile);
}
void Converter::toNgspice()
{
    string cmd;
    cmd.append("./ngspice ").append(CaseName).append(".sp -o ").append(CaseName).append("_ngspice");
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
        
        pair<string, string> BlockMsg = myhelper.getBlockMsg(get<1>(C));
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
//have error 
void Converter::toOutputDef()
{
    int StartIndex = 0 ;
    int EndIndex = 0 ;
    int Rowindex = 0 ;
    vector<string> temp = OriginDef ;

    for(auto line : temp)
    {
        if( line.find("SPECIALNETS") != string::npos )
        {
            if(StartIndex == 0)
            {
                StartIndex = Rowindex ;
                Rowindex++;
                continue;
            }
            if(EndIndex == 0)
                EndIndex = Rowindex ;
            if(EndIndex != 0)
                break;
        }
        Rowindex++;
    }
    // vector 用 erase 要注意 performace
    temp.erase(temp.begin() + StartIndex, temp.begin()+EndIndex + 1 );
    vector<string> NewDefSp;
    NewDefSp.push_back("SPECIALNETS " + to_string(SpecialNetsMaps.size()) + " ;");
    
    for( auto nets : SpecialNetsMaps )
    {
        string line = "- " + nets.first + " " ;
        line.append(myhelper.DestinationMapToString(nets.second.DESTINATIONMAPS));
        NewDefSp.push_back(line);
        NewDefSp.push_back(myhelper.NETSMULTIMAPSToString(nets.second.NETSMULTIMAPS));
        NewDefSp.push_back(" + USE " + nets.second.USE );
        NewDefSp.push_back(" ;");
    }
    for(auto x : NewDefSp)
    {
        temp.insert(temp.begin() + StartIndex, x);
        StartIndex++ ;
    }
    temp.insert(temp.begin() + StartIndex, "END SPECIALNETS");
    string output = CaseName + "_output.def";
    fstream fout(output , ios::out);
    for( auto x : temp )
        fout << x << endl;
     
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
        
        
        pair<Point<int>,Point<int>> coordinate = myhelper.getBlockCoordinate(ComponentMaps[BlockNames[i]].STARTPOINT.x ,
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
    toSpice();
    toNgspice();
    ng.init(CaseName);
    ng.ConcatIR_Drop() ;
    ng.printStats(DestinationMap);
    toOutputFile();
    DestinationMap.clear();
}
void Converter::Visualize()
{
    system("java -jar JavaApplication5.jar  ");
}
void Converter::caluMetalUse(map<string , vector<Line>> & lineMap)
{
    vector<Line> Pseudolines;
    for( auto metal : lineMap )
    {
        // key直為某兩條線，value為那兩條線的重疊面積
        map<string,long double> duplicate;
        //還沒有扣掉重疊的
        long double Fake_Area = 0 ;
        for( int i = 0 ; i < metal.second.size(); i++ )
        {
            if( metal.second[i].isPsedoLine ) continue ;
            long double length = (metal.second[i]).isHorizontal ? metal.second[i].pt2.x - metal.second[i].pt1.x : metal.second[i].pt2.y - metal.second[i].pt1.y ;
            long double area = metal.second[i].Width * length / 1000000 ;
            Fake_Area += area;
            for(int j = 0 ; j < metal.second.size() ; j++)
            {
                if( metal.second[j].isPsedoLine ) continue ;
                // line map 會將同層的金屬當key ，所以進來的一定都是同層的金屬
                // 假如同層金屬且有交叉，不會是Metal遇到VIA
                if( myhelper.isCross(metal.second[i], metal.second[j]) )
                {
                    Point<int> cross = myhelper.getCrossPoint(metal.second[i], metal.second[j]);
                    string key = to_string(cross.x).append(to_string(cross.y));
                    double dup_area = (metal.second[i].Width) * (metal.second[j].Width) / 1000000;
                    if( duplicate.find(key) == duplicate.end())
                        duplicate.insert( make_pair(key, dup_area));
                }
            }
        }
        long double Real_Area = Fake_Area ;
        // 減去線重疊的面積
        for( auto area : duplicate )
        {
            Real_Area -= area.second;
        }
        if( MetalUsage.find(metal.first) == MetalUsage.end() )
            MetalUsage.insert(make_pair(metal.first, Real_Area));
        else
            MetalUsage[metal.first] += Real_Area ;
    }
}


