//
//  DetailRouter.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 13/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "DetailRouter.hpp"




DetailRouter::DetailRouter()
{
    InitGrids();
}

DetailRouter::~DetailRouter()
{
    
}

void DetailRouter::CutByPitch(double pitch , set<int> & Horizontal , set<int> & Vertical )
{
    int x_diff = DIEAREA.pt2.x - DIEAREA.pt1.x ;
    int y_diff = DIEAREA.pt2.y - DIEAREA.pt1.y ;
    double unit_length = pitch * UNITS_DISTANCE ;
    // Set Vertical Line
    for( int i = unit_length ; i <= x_diff ; i += unit_length )
        Vertical.insert(i);
    
    
    // Set Horizontal Line
    for( int i = unit_length ; i <= y_diff ; i += unit_length )
        Horizontal.insert(i);
    
    // DIEAREA is not align .
    if( *Vertical.end() != DIEAREA.pt2.x ) Vertical.insert(DIEAREA.pt2.x);
    if( *Horizontal.end() != DIEAREA.pt2.y ) Horizontal.insert(DIEAREA.pt2.y);
}

void DetailRouter::InitGrids()
{
    for( auto layer : LayerMaps )
    {
        // 代表屬於Metal層
        if( layer.second.TYPE == "ROUTING" )
        {
            set<int> Horizontal , Vertical ;
            CutByPitch(layer.second.PITCH_DISTANCE , Horizontal , Vertical);
            vector< vector< Grid > > Grids ; 
            Point<int> startpoint(0,0) ;
            for( auto h : Horizontal )
            {
                vector<Grid> temp ;
                for(auto v : Vertical)
                {
                    Grid grid ;
                    Point<int> CrossPoint(v,h);
                    grid.width = CrossPoint.x - startpoint.x ;
                    grid.length = CrossPoint.y - startpoint.y ;
                    grid.startpoint = startpoint ;
                    
                    pair<bool, string> result = RouteHelper.IsBlock(startpoint, Point<int>( startpoint.x + grid.width , startpoint.y + grid.length ) );
                    if( get<0>(result) )
                    {
                        auto begin = MacroMaps[get<1>(result)].obs.InnerMaps.begin();
                        auto end = --MacroMaps[get<1>(result)].obs.InnerMaps.end() ;
                        grid.lowermetal = stoi(begin->first.substr(5));
                        grid.uppermetal = stoi(end->first.substr(5));
                    }
                    startpoint.x += (CrossPoint.x - startpoint.x) ;
                    temp.push_back(grid);
                }
                startpoint.x = 0 ;
                startpoint.y = h ;
                Grids.push_back(temp);
            }
            MLGrids.insert(make_pair(layer.first, Grids));
        }
    }
    cout << "Done" << endl;
}

void DetailRouter::DetailRoute()
{
    
    gr.Route();
    
}
