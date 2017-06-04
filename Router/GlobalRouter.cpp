//
//  GlobalRouter.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 31/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "GlobalRouter.hpp"




GlobalRouter::GlobalRouter()
{
    initGraph();
}


GlobalRouter::~GlobalRouter()
{
    
}


void GlobalRouter::initGraph()
{
    initBlockMap();
    initLineVec();
    Point<int> StartPoint(0,0) ;
    for( auto h : Horizontal )
    {
        vector<Grid> temp ;
        for(auto v : Vertical)
        {
            Grid grid ;
            Point<int> CrossPoint(v,h);
            grid.Width = CrossPoint.x - StartPoint.x ;
            grid.Length = CrossPoint.y - StartPoint.y ;
            grid.StartPoint = StartPoint ;
            
            pair<bool, string> result = isBlock(StartPoint, Point<int>( StartPoint.x + grid.Width , StartPoint.y + grid.Length ) );
            if( get<0>(result) )
            {
                auto begin = MacroMaps[get<1>(result)].obs.InnerMaps.begin();
                auto end = --MacroMaps[get<1>(result)].obs.InnerMaps.end() ;
                grid.LowerMetal = stoi(begin->first.substr(5));
                grid.UpperMetal = stoi(end->first.substr(5));
            }
            cout << StartPoint << endl;
            StartPoint.x += (CrossPoint.x - StartPoint.x) ;
            temp.push_back(grid);
        }
        StartPoint.x = 0 ;
        StartPoint.y = h ;
        Graph.push_back(temp);
    }
    
    for(auto a : Graph)
    {
        for(auto b : a)
        {
            Point<int> rightup(b.StartPoint.x+b.Width , b.StartPoint.y + b.Length);
            cout << "LeftDown:" << b.StartPoint << " RightUp:" <<  rightup;
            cout << "[ " << b.LowerMetal << "," << b.UpperMetal << " ]" << endl;
        }
    }
}
pair<bool, string> GlobalRouter::isBlock(Point<int> LeftDown , Point<int> RightUp)
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
            get<1>(result) = block.first;
        }
    }
    return result;
}
void GlobalRouter::initBlockMap()
{
    for( auto component : ComponentMaps )
    {
        pair<Point<int>,Point<int>> coordinate = helper.getBlockCoordinate(component.second.STARTPOINT.x, component.second.STARTPOINT.y, MacroMaps[ component.second.MACROTYPE ].WIDTH * UNITS_DISTANCE, MacroMaps[ component.second.MACROTYPE ].LENGTH * UNITS_DISTANCE, component.second.ORIENT);
        BlockMap.insert(make_pair(component.second.MACROTYPE, coordinate));
    }
}
void GlobalRouter::Route()
{
    Graph_SP g ;
    for(int row = 0 ; row < Graph.size() ; row++)
    {
        for( int col = 0 ; col < Graph[row].size() ; col++ )
        {
            
        }
    }
    
}
unsigned long GlobalRouter::translate(int row , int column)
{
    size_t size = Graph[0].size();
    return row * size + column ;
}

void GlobalRouter::initLineVec()
{
    for (auto Conponent : ComponentMaps)
    {
        pair<Point<int> , Point<int>> BlockCoordinate = helper.getBlockCoordinate(Conponent.second.STARTPOINT.x, Conponent.second.STARTPOINT.y,MacroMaps[Conponent.second.MACROTYPE].WIDTH * UNITS_DISTANCE, MacroMaps[Conponent.second.MACROTYPE].LENGTH * UNITS_DISTANCE, Conponent.second.ORIENT);
        Vertical.insert(get<0>(BlockCoordinate).x);
        Vertical.insert(get<1>(BlockCoordinate).x);
        Horizontal.insert(get<0>(BlockCoordinate).y);
        Horizontal.insert(get<1>(BlockCoordinate).y);
    }
    Vertical.insert(DIEAREA.pt2.x);
    Horizontal.insert(DIEAREA.pt2.y);
}
