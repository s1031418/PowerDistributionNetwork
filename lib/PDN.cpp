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
    Init();
}


PDN::~PDN()
{
}
void PDN::Init()
{
    for( auto it = PinMaps.begin(), end = PinMaps.end(); it != end;it = PinMaps.upper_bound(it->first))
    {
        PinNames.push_back(it->first);
    }
    initLineVec();
    initGraph();
}
void PDN::initGraph()
{
    int RowIndex = 0;
    int ColumnIndex = 0;
    Point<int> StartPoint(0,0) ;
    int LastX = 0 , LastY = 0 ;
    vector<Grid> temp ;
    for( auto h : Horizontal )
    {
        for(auto v : Vertical)
        {
            Grid grid ;
            Point<int> CrossPoint(v,h);
            grid.Width = CrossPoint.x - StartPoint.x ;
            grid.Length = CrossPoint.y - StartPoint.y ;
            StartPoint.x += (CrossPoint.x - StartPoint.x) ;
            cout << grid.Width << " " << grid.Length << endl;
        }
        StartPoint.x = 0 ;
        StartPoint.y = h ;
    }
    cout << "-----------------------------"<<endl;
    for(auto h = Horizontal.begin() ; h != Horizontal.end() ; ++h )
    {
        
        for( auto v = Vertical.begin() ; v != Vertical.end() ; ++v )
        {
            
            Grid grid ;
            Point<int> CrossPoint(*v,*h);
            if( v  == Vertical.begin())
            {
                grid.Width = *v  ;
                LastX = grid.Width;
            }
            else
            {
                grid.Width = *v - LastX ;
                LastX = *v ;
            }
            if( h == Horizontal.begin() )
            {
                grid.Length = *h  ;
                LastY = grid.Length;
            }
            else
            {
                grid.Length = *h - LastY;
            }
            
            cout << grid.Width << " " << grid.Length << endl;
            temp.push_back(grid);
            ColumnIndex++;
        }
        Graph.push_back(temp);
        temp.clear();
        LastY = *h ;
        RowIndex++;
    }
    cout << "-----------------------------"<<endl;
    for(auto a : Graph)
    {
        for(auto b : a )
            cout << b.Width << " " << b.Length << endl;
    }
}
void PDN::initLineVec()
{
    for (auto Conponent : ComponentMaps)
    {
        pair<Point<int> , Point<int>> BlockCoordinate = myhelper.getBlockCoordinate(Conponent.second.STARTPOINT.x, Conponent.second.STARTPOINT.y,MacroMaps[Conponent.second.MACROTYPE].WIDTH * UNITS_DISTANCE, MacroMaps[Conponent.second.MACROTYPE].LENGTH * UNITS_DISTANCE, Conponent.second.ORIENT);
        Vertical.insert(get<0>(BlockCoordinate).x);
        Vertical.insert(get<1>(BlockCoordinate).x);
        Horizontal.insert(get<0>(BlockCoordinate).y);
        Horizontal.insert(get<1>(BlockCoordinate).y);
    }
    Vertical.insert(DIEAREA.pt2.x);
    Horizontal.insert(DIEAREA.pt2.y);
}
void PDN::optimize()
{
    
}
void PDN::toSpice()
{
    
}
