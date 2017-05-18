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
    root = new Root[1];
}
Converter::~Converter()
{
    
}
void Converter::initLineVec(std::multimap<std::string,Nets> NetsMM , vector<Line>& line_vec)
{
    
    // foreach NetsMM
    for( auto it = NetsMM.begin(), end = NetsMM.end(); it != end;it = NetsMM.upper_bound(it->first))
    {
        auto first = NetsMM.lower_bound(it->second.METALNAME);
        auto last = NetsMM.upper_bound(it->second.METALNAME);
        // foreach NetsMM's Key
        while (first != last) {
            Line line ;
            if(isHorizontal(first->second.ABSOLUTE_POINT1, first->second.ABSOLUTE_POINT2))
            {
                Point<int> right = ( first->second.ABSOLUTE_POINT1.x > first->second.ABSOLUTE_POINT2.x ) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
                Point<int> left =  ( first->second.ABSOLUTE_POINT1.x < first->second.ABSOLUTE_POINT2.x ) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
                line.pt1 = left ;
                line.pt2 = right ;
                line.MetalName = first->second.METALNAME ;
                line.isHorizontal = true ;
                line.Width = first->second.ROUNTWIDTH;
                line_vec.push_back(line);
            }
            else
            {
                Point<int> top = (first->second.ABSOLUTE_POINT1.y > first->second.ABSOLUTE_POINT2.y) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2;
                Point<int> bottom = (first->second.ABSOLUTE_POINT1.y < first->second.ABSOLUTE_POINT2.y) ? first->second.ABSOLUTE_POINT1 : first->second.ABSOLUTE_POINT2 ;
                line.pt1 = bottom ;
                line.pt2 = top ;
                line.MetalName = first->second.METALNAME ;
                line.isHorizontal = false ;
                line.Width = first->second.ROUNTWIDTH;
                line_vec.push_back(line);
            }
            first++;
        }

    }
}
Line * Converter::getStartLine(string PinNames,vector<Line> & line_vec)
{
    Lptr lptr = new Line[1] ;
    
    for (int j = 0 ; j < line_vec.size(); j++) {
        if(line_vec[j].pt1 == PinMaps[PinNames].STARTPOINT || line_vec[j].pt2 == PinMaps[PinNames].STARTPOINT)
        {
            *(lptr) = line_vec[j];
        }
    }
    return lptr ;
}
void Converter::insert(Lptr newlptr)
{
    Lptr temp = root->link;
    while (true)
    {
        
        if( isCross(*temp, *newlptr) )
        {
            temp->llink = newlptr;
            break;
        }
        else if(temp->llink != nullptr)
        {
            temp = temp->llink ;
        }
        else
            break;
    }
}
void Converter::inorder(Lptr lptr)
{
    if (lptr)
    {
        inorder(lptr->llink);
        cout << lptr->pt1 << " " << lptr->pt2 << endl;
        inorder(lptr->rlink);
    }

}
void Converter::Build()
{
    int cnt = 0 ;
    vector<string> PinNames;
    for( auto it = PinMaps.begin(), end = PinMaps.end(); it != end;it = PinMaps.upper_bound(it->first))
    {
        PinNames.push_back(it->first);
    }
    for(int i = 0 ; i < PinNames.size() ; i++)
    {
        vector<Line> line_vec ;
        initLineVec(SpecialNetsMaps[ PinNames[i] ].NETSMULTIMAPS , line_vec);
        // intialization
        Lptr StartLine = getStartLine(PinNames[i],line_vec);
        root->link = StartLine ;
        root->line_count += 1 ;
        while (true)
        {
            if(cnt == 3) break; 
            vector<Line> CrossSets = getCrossSets(*StartLine, line_vec);
            if( CrossSets.size() == 1 )
            {
                Lptr newline = new Line[1];
                *newline = CrossSets[0];
                insert(newline);
                root->line_count += 1 ;
                eraseline_vec(line_vec,*StartLine);
                *StartLine = CrossSets[0] ;
            }
            else if (CrossSets.size() == 2)
            {
                break;
            }
            cnt++ ;
        }
    }
}
void Converter::toSpice()
{
    // 第一行必須為註解
    // 元件名稱+代號 正節點代號 負節點代號 元件數值
    int V_cnt = 1 ;
    
    
    cout << "Comments" << endl;
    vector<string> PinNames;
    for( auto it = PinMaps.begin(), end = PinMaps.end(); it != end;it = PinMaps.upper_bound(it->first))
    {
        PinNames.push_back(it->first);
    }
    for(int i = 0 ; i < PinNames.size() ; i++)
    {
        int I_cnt = 1 ;
        int R_cnt = 1 ;
        // print Voltage
        // 假設 Pin 沒有 port ，否則應該有迴圈，目前先寫沒有port版本
//        Point<int> start = PinMaps[ PinNames[i] ].STARTPOINT ;
        cout << "V_" << PinNames[i] << "_"<< V_cnt << " " << PinMaps[ PinNames[i] ].METALNAME << "_";
        cout << PinMaps[ PinNames[i] ].STARTPOINT.x << " " << PinMaps[ PinNames[i] ].STARTPOINT.y << " " ;
        cout << "gnd " << VoltageMaps[ PinNames[i] ] << endl;
        
        Build();
        inorder(root->link);
        
        // print Current
        for(auto c : SpecialNetsMaps[ PinNames[i] ].DESTINATIONMAPS )
        {
            
            cout << "I_" << PinNames[i] << "_" << I_cnt << " ";
            
            cout << endl;
            I_cnt++;
        }
        
        
    }
    
}
void Converter::eraseline_vec(vector<Line>& line_vec , Line line)
{
    for(int i = 0 ; i < line_vec.size() ; i++)
    {
        if( line_vec[i] == line )
            line_vec.erase(line_vec.begin() + i);
    }
}
bool Converter::isCross(Line line1 , Line line2)
{
    if(line1.isHorizontal && !line2.isHorizontal)
    {
        
        if( line1.pt2.y > line2.pt1.y && line1.pt2.y < line2.pt2.y && line2.pt2.x > line1.pt1.x && line2.pt2.x < line1.pt2.x ) // isIntersaction
            return true ;
        else
            return false ;
        
    }
    else if(!line1.isHorizontal && line2.isHorizontal)
    {
        if( line2.pt2.y > line1.pt1.y && line2.pt2.y < line1.pt2.y && line1.pt2.x > line2.pt1.x && line1.pt2.x < line2.pt2.x ) // isIntersaction
            return true ;
        else
            return false ;
    }
    else
        return false;
}
vector<Line> Converter::getCrossSets(Line OriginLine , vector<Line>& Candidate )
{
    vector<Line> CrossSets ;
    // foreach line_vec
    for (int i = 0; i < Candidate.size(); i++) {
        if(isCross(OriginLine, Candidate[i]) )
        {
            CrossSets.push_back(Candidate[i]);
        }
    }
    return CrossSets;
}
void Converter::createSubStree(std::multimap<std::string,Nets> NetsMM, string PinName)
{
    
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
bool Converter::isIntersaction(Point<int> pt1 , Point<int> pt2 , Point<int> pt3 , Point<int> pt4 )
{
    Point<int> right , left , top , bottom ;
    if(isHorizontal(pt1, pt2))
    {
        // pt1 pt2 is horizontal , pt3 pt4 is vertical
        right = ( pt1.x > pt2.x ) ? pt1 : pt2 ;
        left =  ( pt1.x < pt2.x ) ? pt1 : pt2 ;
        top = (pt3.y > pt4.y) ? pt3 : pt4;
        bottom = (pt3.y < pt4.y) ? pt3 : pt4 ;
    }
    else
    {
        // pt1 pt2 is vertical , pt3 pt4 is horizontal
        right = ( pt3.x > pt4.x ) ? pt3 : pt4 ;
        left =  ( pt3.x < pt4.x ) ? pt3 : pt4 ;
        top = (pt1.y > pt2.y) ? pt1 : pt2;
        bottom = (pt1.y < pt2.y) ? pt1 : pt2 ;
    }
    return ( right.y > bottom.y && right.y < top.y && top.x > left.x && top.x < right.x ) ;// isIntersaction
}
bool Converter::isIntersaction(Point<int> pt1 , Point<int> pt2 , Point<int> pt3 , Point<int> pt4 , int VerticalLineWidth , int HorizontalLineWidth )
{
    // 可以修改return 交集的方向
    Point<int> right , left , top , bottom ;
    if(isHorizontal(pt1, pt2))
    {
        // pt1 pt2 is horizontal , pt3 pt4 is vertical
        right = ( pt1.x > pt2.x ) ? pt1 : pt2 ;
        left =  ( pt1.x < pt2.x ) ? pt1 : pt2 ;
        top = (pt3.y > pt4.y) ? pt3 : pt4;
        bottom = (pt3.y < pt4.y) ? pt3 : pt4 ;
    }
    else
    {
        // pt1 pt2 is vertical , pt3 pt4 is horizontal
        right = ( pt3.x > pt4.x ) ? pt3 : pt4 ;
        left =  ( pt3.x < pt4.x ) ? pt3 : pt4 ;
        top = (pt1.y > pt2.y) ? pt1 : pt2;
        bottom = (pt1.y < pt2.y) ? pt1 : pt2 ;
    }
    //水平線的Y座標在垂直線之間，垂直線的X座標在水平線之間
    if( right.y > bottom.y && right.y < top.y && top.x > left.x && top.x < right.x ) // isIntersaction
    {
        // interaction in left_up_corner and no parallel
        if( (top.x - VerticalLineWidth) ==  left.x &&  (left.y + HorizontalLineWidth) == top.y )
            return true ;
        // interaction in right_down_corner and no parallel
        else if( (bottom.x + VerticalLineWidth) == right.x && (right.y - HorizontalLineWidth) == bottom.y )
            return true;
        else
            return true ; //還沒寫
    }
    else
    {
        return false;
    }
    
}
/*
Point<int> Converter::getIntersactionPoint(Point<int> pt1 , Point<int> pt2 , multimap<string,Nets> & NetsMM)
{
    
    for( auto it = NetsMM.begin(), end = NetsMM.end(); it != end;it = NetsMM.upper_bound(it->first))
    {
        auto first = NetsMM.lower_bound(it->first);
        auto last = NetsMM.upper_bound(it->first);
        while (first != last) {
            
                
            first++;
        }
    }
    
    
}
 */
