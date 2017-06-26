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
void DetailRouter::InitGraph_SP(RouteConstraint constraint)
{
    
}
void DetailRouter::CutByPitch(double pitch , set<int> & Horizontal , set<int> & Vertical )
{
//    int x_diff = DIEAREA.pt2.x - DIEAREA.pt1.x ;
//    int y_diff = DIEAREA.pt2.y - DIEAREA.pt1.y ;
//    double unit_length = pitch * UNITS_DISTANCE ;
//    // Set Vertical Line
//    for( int i = unit_length ; i <= x_diff ; i += unit_length )
//        Vertical.insert(i);
//    
//    
//    // Set Horizontal Line
//    for( int i = unit_length ; i <= y_diff ; i += unit_length )
//        Horizontal.insert(i);
//    
//    // DIEAREA is not align .
//    if( *Vertical.end() != DIEAREA.pt2.x ) Vertical.insert(DIEAREA.pt2.x);
//    if( *Horizontal.end() != DIEAREA.pt2.y ) Horizontal.insert(DIEAREA.pt2.y);
}

void DetailRouter::InitGrids()
{
//    cout << "Begin Initialize MultiLayer Grid Graph ..." << endl;
//    clock_t Start = clock();
//    for( auto layer : LayerMaps )
//    {
//        // 代表屬於Metal層
//        if( layer.second.TYPE == "ROUTING" )
//        {
//            set<int> Horizontal , Vertical ;
//            CutByPitch(layer.second.PITCH_DISTANCE , Horizontal , Vertical);
//            vector< vector< Grid > > Grids ; 
//            Point<int> startpoint(0,0) ;
//            for( auto h : Horizontal )
//            {
//                vector<Grid> temp ;
//                for(auto v : Vertical)
//                {
//                    Grid grid ;
//                    Point<int> CrossPoint(v,h);
//                    grid.width = CrossPoint.x - startpoint.x ;
//                    grid.length = CrossPoint.y - startpoint.y ;
//                    grid.startpoint = startpoint ;
//                    
//                    pair<bool, string> result = RouteHelper.IsBlock(startpoint, Point<int>( startpoint.x + grid.width , startpoint.y + grid.length ) );
//                    if( get<0>(result) )
//                    {
//                        auto begin = MacroMaps[get<1>(result)].obs.InnerMaps.begin();
//                        auto end = --MacroMaps[get<1>(result)].obs.InnerMaps.end() ;
//                        grid.lowermetal = stoi(begin->first.substr(5));
//                        grid.uppermetal = stoi(end->first.substr(5));
//                    }
//                    startpoint.x += (CrossPoint.x - startpoint.x) ;
//                    temp.push_back(grid);
//                }
//                startpoint.x = 0 ;
//                startpoint.y = h ;
//                Grids.push_back(temp);
//            }
//            MLGrids.insert(make_pair(layer.first, Grids));
//        }
//    }
//    clock_t End = clock();
//    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
//    cout << "Initialize MultiLayer Grid Graph Done" << endl ;
//    cout << "We cost " << duration << "(s)" << endl;
}


void DetailRouter::DetailRoute()
{
    gr.Route();
    for( auto g : gr.GlobalSolutions )
    {
        auto powerPinCoordindate = RouteHelper.getPowerPinCoordinate(g.PowerSourceName);
        auto targetBlock = RouteHelper.getBlock(g.TargetName.first, g.TargetName.second);
        pair<Point<int>, Point<int>> targetPinCoordinate = make_pair(targetBlock.LeftDown, targetBlock.RightUp);
        
    }
}
void DetailRouter::XY_Route(vector<Coordinate3D> paths , pair<Point<int>, Point<int>> source , pair<Point<int>, Point<int>> target)
{
    vector<segment> progress ;
    segment s ;
    
    int up = 0 , down = 0, left = 0 ,right =0 ;
    // h->true , v->false
    bool initDirection = (paths[0].x != paths[1].x) ? true : false ;
    for(int i = 0 ; i < paths.size() ; i++)
    {
        bool nextDirection = ( paths[i+1].x - paths[i].x > 0 ) ? true : false ;
        // 遇到轉彎
        if( initDirection != nextDirection )
        {
            up = down = left = right = 0 ;
        }
        if( paths[i].x != paths[i+1].x )
        {
            if(paths[i+1].x - paths[i].x > 0) right++ ;
            if(paths[i+1].x - paths[i].x < 0) left++ ;
        }
        if( paths[i].y != paths[i+1].y )
        {
            if(paths[i+1].y - paths[i].y > 0) up++ ;
            if(paths[i+1].y - paths[i].y < 0) down++ ;
        }
        if( right > 0 ) { s.direction = "RIGHT"; s.progress = right ; }
        if( left > 0 ) { s.direction = "LEFT"; s.progress = left ; }
        if( up > 0 ) { s.direction = "UP"; s.progress = up ; }
        if( down > 0 ) { s.direction = "DOWN"; s.progress = down ; }
    }
    vector<Point<int>> CenterPoints ;
    for( auto p : paths )
    {
        int X = (gr.Grids[p.y][p.x].startpoint.x + gr.Grids[p.y][p.x].width)/2 ;
        int Y = (gr.Grids[p.y][p.x].startpoint.y + gr.Grids[p.y][p.x].length)/2 ;
        CenterPoints.push_back(Point<int>(X,Y));
    }
    
}
void DetailRouter::printGlobalSols()
{
    for( auto g : gr.GlobalSolutions )
    {
        cout << "PowerSourceName:" << g.PowerSourceName << endl;
        cout << "TargetName:" << g.TargetName.first << " " << g.TargetName.second << endl;
        cout << "Source Index:" << g.source.x << "," << g.source.y << "," << g.source.z << endl;
        for( auto t : g.targets)
            cout << "target index:" << t.x << "," << t.y << "," << t.z << endl;
        for( auto p : g.paths )
            cout << p.x << "," << p.y << "," << p.z << endl;
        cout << "---------------------------------------------------------" << endl;
    }
}
