//
//  Router.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 27/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "Router.hpp"




Router::Router()
{
    // Init lowest and highest
    vector<string> metals ;
    for( auto x : LayerMaps )
    {
        if( x.second.TYPE == "ROUTING" )
            metals.push_back(x.first);
    }
    lowest = stoi(metals[0].substr(5));
    highest = stoi(metals[metals.size()-1].substr(5));
    InitGrids();
//    toGridGraph();
}

Router::~Router()
{
    
}
void Router::toGridGraph()
{
    bool init = true ;
    
    for( auto row : Grids )
    {
        for( auto col : row )
        {
            if( init ){ cout<< " + FIXED" ; init = false ;}
            else cout << " NEW ";
            if( RouterHelper.IsBlock( col )) cout << " METAL5 " ;
            else cout << " METAL6 " ;
            cout << col.width << " " ;
            cout << " + SHAPE STRIPE ";
            cout << " ( "<< col.startpoint.x + col.width / 2 ;
            cout << " "<< col.startpoint.y << " )" ;
            cout << " ( "<< col.startpoint.x + col.width / 2 ;
            cout << " "<< col.startpoint.y + col.length << " )" ;
            cout << endl;
        }
    }
    
}

void Router::Route()
{
    cout << "Begin Routing ..." << endl;
    clock_t Start = clock();
    int YSize = (int)Grids.size() ;
    int XSize = (int)Grids[0].size() ;
    int Size = XSize * YSize * highest ;
    Graph_SP graph_sp(Size);
    InitGraph_SP(graph_sp);
    
    // 先不決定ordering
    
    for( auto it = Connection.begin() ; it != Connection.end() ; it = Connection.upper_bound(it->first))
    {
        cout << it->first << " " << it->second.BlockName << " " << it->second.BlockPinName << endl;
        auto powerPinCoordinate = RouterHelper.getPowerPinCoordinate(it->first);
        auto BlockPinCoordinate = RouterHelper.getBlock(it->second.BlockName, it->second.BlockPinName);
        cout << "Source Absolute Coordinate:" << powerPinCoordinate.LeftDown << "," << powerPinCoordinate.RightUp << endl;
        cout << "PowerPin Absolute Coordinate:" << BlockPinCoordinate.LeftDown << ","  << BlockPinCoordinate.RightUp << endl;
    }
    
    clock_t End = clock();
    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
    cout << "Routing Done" << endl ;
    cout << "We cost " << duration << "(s)" << endl;
    
    
}
Coordinate3D Router::translate1D_3D(int index)
{
    int YSize = (int)Grids.size() ;
    int XSize = (int)Grids[0].size() ;
    int total = XSize*YSize;
    int z = index / total ;
    int coordinate2d = index % total ;
    int x = coordinate2d % XSize ;
    int y = coordinate2d / XSize ;
    return Coordinate3D(x,y,z+1);
}
int Router::translate3D_1D(Coordinate3D coordinate3d)
{
    int YSize = (int)Grids.size() ;
    int XSize = (int)Grids[0].size() ;
    int total = XSize*YSize;
    int x = coordinate3d.x ;
    int y = coordinate3d.y ;
    int z = coordinate3d.z ;
    return (z-1)*total + x + y * XSize ;
}
vector<pair<Direction3D, int>> Router::translateToFriendlyForm( vector<Coordinate3D> & Paths )
{
    int cnt = 1 ;
    Direction3D initDirection = topOrient ;
    if( (int)(Paths[1].x - Paths[0].x) > 0) initDirection = rightOrient ;
    if( (int)(Paths[1].x - Paths[0].x) < 0) initDirection = leftOrient ;
    if( (int)(Paths[1].y - Paths[0].y) > 0) initDirection = upOrient ;
    if( (int)(Paths[1].y - Paths[0].y) < 0) initDirection = downOrient ;
    if( (int)(Paths[1].z - Paths[0].z) > 0) initDirection = topOrient ;
    if( (int)(Paths[1].z - Paths[0].z) < 0) initDirection = bottomOrient ;
    vector<pair<Direction3D, int>> paths ;
    for( int i = 1 ; i < Paths.size()-1 ; i++ )
    {
        Direction3D direction = topOrient ;
        if( (int)(Paths[i+1].x - Paths[i].x) > 0) direction = rightOrient ;
        if( (int)(Paths[i+1].x - Paths[i].x) < 0) direction = leftOrient ;
        if( (int)(Paths[i+1].y - Paths[i].y) > 0) direction = upOrient ;
        if( (int)(Paths[i+1].y - Paths[i].y) < 0) direction = downOrient ;
        if( (int)(Paths[i+1].z - Paths[i].z) > 0) direction = topOrient ;
        if( (int)(Paths[i+1].z - Paths[i].z) < 0) direction = bottomOrient ;
        
        if( direction != initDirection )
        {
            paths.push_back(make_pair(initDirection, cnt));
            cnt = 0 ;
            initDirection = direction;
        }
        cnt++;
    }
    paths.push_back(make_pair(initDirection, cnt));
    for( auto p : paths )
    {
        if( p.first == 0 )  cout << "UP";
        if( p.first == 1 )  cout << "DOWN";
        if( p.first == 2 )  cout << "LEFT";
        if( p.first == 3 )  cout << "RIGHT";
        if( p.first == 4 )  cout << "TOP";
        if( p.first == 5 )  cout << "BOTTOM";
        cout << " " << p.second << endl;
    }
    return paths;
}
void Router::TestToOutoutDef(  vector<Coordinate3D> & paths )
{
    auto source = paths[0];
    auto startPoint = Grids[source.y][source.x].startpoint ;
    Point<int> oringinTargetPoint = startPoint;
    Point<int> targetPoint = startPoint;
    auto friendlyForm = translateToFriendlyForm(paths);
    for( int i = 0 ; i < friendlyForm.size() ; i++ )
    {
        
        Direction3D diection = friendlyForm[i].first ;
        Direction3D nextDirection = friendlyForm[i+1].first ;
        int progress = friendlyForm[i].second ;
        if( diection == upOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x , oringinTargetPoint.y + (progress * DEFAULT_PITCH * UNITS_DISTANCE));
            targetPoint = oringinTargetPoint ;
            targetPoint.y += (DEFAULT_WIDTH * UNITS_DISTANCE / 2) ;
            
            cout  << startPoint << " " << targetPoint << endl;
        }
        else if( diection == downOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x , oringinTargetPoint.y - (progress * DEFAULT_PITCH * UNITS_DISTANCE));
            targetPoint = oringinTargetPoint ;
            targetPoint.y -= (DEFAULT_WIDTH * UNITS_DISTANCE / 2) ;
            cout  << startPoint << " " << targetPoint << endl;
        }
        else if( diection == leftOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x - (progress * DEFAULT_PITCH * UNITS_DISTANCE), oringinTargetPoint.y );
            targetPoint = oringinTargetPoint ;
            targetPoint.x -= (DEFAULT_WIDTH * UNITS_DISTANCE / 2) ;
            cout  << startPoint << " " << targetPoint << endl;
        }
        else if( diection == rightOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x + (progress * DEFAULT_PITCH * UNITS_DISTANCE), oringinTargetPoint.y );
            targetPoint = oringinTargetPoint ;
            targetPoint.x += (DEFAULT_WIDTH * UNITS_DISTANCE / 2) ;
            cout  << startPoint << " " << targetPoint << endl;
        }
        else if( diection == topOrient )
        {
            for( int i = 0 ; i < progress ; i++ )
                cout << oringinTargetPoint << " " << "via" << endl;
            
        }
        else if( diection == bottomOrient )
        {
            for( int i = 0 ; i < progress ; i++ )
                cout << oringinTargetPoint << " " << "via" << endl;
            
        }
        //-------------------------------//
        //update source point
        startPoint = oringinTargetPoint ;
        if( nextDirection == upOrient )
        {
            startPoint.y -= DEFAULT_WIDTH / 2 * UNITS_DISTANCE;
            
        }
        if( nextDirection == downOrient )
        {
            startPoint.y += DEFAULT_WIDTH / 2 * UNITS_DISTANCE;
        }
        if( nextDirection == leftOrient )
        {
            startPoint.x += DEFAULT_WIDTH / 2 * UNITS_DISTANCE;
        }
        if( nextDirection == rightOrient )
        {
            startPoint.x -= DEFAULT_WIDTH / 2 * UNITS_DISTANCE;
        }
    }
    
}
void Router::InitGraph_SP( Graph_SP & graph_sp )
{
    cout << "Begin Initialize 3D Shortest Path Graph ..." << endl;
    clock_t Start = clock();
    int YSize = (int)Grids.size() ;
    int XSize = (int)Grids[0].size() ;
    int Right = 1 ;
    int Left = -1 * Right ;
    int Up = XSize ;
    int Down = -1 * Up ;
    int Top =  ( XSize - 1 ) * Right + ( YSize - 1 ) * Up + 1;
    int Bottom = -1 * Top ;
    
    for( int z = lowest ; z <= highest ; z++)
    {
        int x = 0 , y = 0 ;
        // down boundary
        // up boundary
        for( int i = 0 ; i < XSize ; i++ )
        {
            boundList.insert(x + (z-1)*Top  );
            boundList.insert(x + ( YSize - 1 ) * Up + (z-1)*Top);
            x +=  Right ;
        }
        // left boundary
        // right boundary
        for( int i = 0 ; i < YSize ; i++ )
        {
            boundList.insert(y + (z-1)*Top );
            boundList.insert(y + ( XSize - 1 ) * Right + (z-1)*Top);
            y +=  Up ;
        }
    }
    for( int z = lowest ; z <= highest ; z++ )
    {
        for( int y = 0 ; y < YSize ; y++)
        {
            for( int x = 0 ; x < XSize ; x++ )
            {
                int index = translate3D_1D(Coordinate3D(x,y,z));
                // not boundary list
                if( boundList.find(index) == boundList.end() )
                {
                    // 因為不是boundary 所以不用check會不會超出index
                    if( Grids[y][x].Edges[z].rightEdge )
                    {
                        graph_sp.AddEdge(index, index+Right, 1);
                        graph_sp.AddEdge(index+Right, index, 1);
                    }
                    else
                    {
                        graph_sp.AddEdge(index, index+Right, Max_Distance);
                        graph_sp.AddEdge(index+Right, index, Max_Distance);
                    }
                    if( Grids[y][x].Edges[z].leftEdge )
                    {
                        graph_sp.AddEdge(index, index+Left, 1);
                        graph_sp.AddEdge(index+Left, index, 1);
                    }
                    else
                    {
                        graph_sp.AddEdge(index, index+Left, Max_Distance);
                        graph_sp.AddEdge(index+Left, index, Max_Distance);
                    }
                    if( Grids[y][x].Edges[z].upEdge )
                    {
                        graph_sp.AddEdge(index, index+Up, 1);
                        graph_sp.AddEdge(index+Up, index, 1);
                    }
                    else
                    {
                        graph_sp.AddEdge(index, index+Up, Max_Distance);
                        graph_sp.AddEdge(index+Up, index, Max_Distance);
                    }
                    if( Grids[y][x].Edges[z].downEdge )
                    {
                        graph_sp.AddEdge(index, index+Down, 1);
                        graph_sp.AddEdge(index+Down, index, 1);
                    }
                    else
                    {
                        graph_sp.AddEdge(index, index+Down, Max_Distance);
                        graph_sp.AddEdge(index+Down, index, Max_Distance);
                    }
                    if( z == lowest )
                    {
                        graph_sp.AddEdge(index, index+Top, 1);
                    }
                    else if (z == highest)
                    {
                        graph_sp.AddEdge(index, index+Bottom, 1);
                    }
                    else
                    {
                        graph_sp.AddEdge(index, index+Top, 1);
                        graph_sp.AddEdge(index, index+Bottom, 1);
                    }
                }
                
            }
        }
    }
    clock_t End = clock();
    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
    //    printAllGrids();
    cout << "3D Shortest Path Graph Done" << endl ;
    cout << "We cost " << duration << "(s)" << endl;
}
void Router::CutByUserDefine()
{
    int x_diff = DIEAREA.pt2.x - DIEAREA.pt1.x ;
    int y_diff = DIEAREA.pt2.y - DIEAREA.pt1.y ;
    int default_length = DEFAULT_PITCH * UNITS_DISTANCE ;
    int scaling_length = default_length  ;
    
    // Set Vertical Line
    for( int i = scaling_length ; i <= x_diff ; i += scaling_length )
        Vertical.insert(i);
    
    
    // Set Horizontal Line
    for( int i = scaling_length ; i <= y_diff ; i += scaling_length )
        Horizontal.insert(i);
    
    // DIEAREA is not align .
    if( *Vertical.end() != DIEAREA.pt2.x ) Vertical.insert(DIEAREA.pt2.x);
    if( *Horizontal.end() != DIEAREA.pt2.y ) Horizontal.insert(DIEAREA.pt2.y);
}
void Router::printAllGrids()
{
    for(auto a : Grids)
    {
        for(auto b : a)
        {
            Point<int> rightup(b.startpoint.x+b.width , b.startpoint.y + b.length);
            
            
            cout << "LeftDown:" << b.startpoint << " RightUp:" <<  rightup;
            
                cout << "[ " << b.lowermetal << "," << b.uppermetal << " ]" << endl;
            for( int i = 1 ; i < b.Edges.size() ; i++ )
            {
                cout << "Layer:" << i << " ";
                
                cout << boolalpha << "Left Edge:" << b.Edges[i].leftEdge<< " ";
                cout << boolalpha << "Right Edge:" << b.Edges[i].rightEdge << " ";
                cout << boolalpha << "Up Edge:" << b.Edges[i].upEdge << " ";
                cout << boolalpha << "Down Edge:" << b.Edges[i].downEdge << " ";
                cout << endl;
            }
            cout << "Capacities:" << endl ;
            for( int i = 0 ; i < b.capacities.size() ; i++ )
                cout << "Layer" << i << ":" << b.capacities[i] << endl;
            cout << "----------------------------------------------------------------" << endl;
            
        }
    }
}
void Router::InitGrids()
{
    cout << "Begin Initialize  Grid Graph ..." << endl;
    clock_t Start = clock();
    //    CutByBlockBoundary();
    CutByUserDefine();
    Point<int> startpoint(0,0) ;
    for( auto h : Horizontal )
    {
        vector<Grid> temp ;
        for(auto v : Vertical)
        {
            Grid grid ;
            grid.capacities.resize(highest+1);
            grid.Edges.resize(highest+1);
            Point<int> CrossPoint(v,h);
            grid.width = CrossPoint.x - startpoint.x ;
            grid.length = CrossPoint.y - startpoint.y ;
            grid.startpoint = startpoint ;
            auto result = RouterHelper.isCrossWithBlock(Rectangle( grid.startpoint , Point<int>( grid.startpoint.x + grid.width , grid.startpoint.y + grid.length ) ));
            
            for( int z = lowest ; z <= highest ; z++ )
            {
                if( result.isCross )
                {
                    int lowerMetal = result.lowerMetal ;
                    int upperMetal = result.upperMetal ;
                    if( z >= lowerMetal && z <= upperMetal)
                    {
                        if( result.isUpEdgeBlock ) grid.Edges[z].upEdge = false ;
                        if( result.isDownEdgeBlock ) grid.Edges[z].downEdge = false ;
                        if( result.isLeftEdgeBlock ) grid.Edges[z].leftEdge = false ;
                        if( result.isRightEdgeBlock ) grid.Edges[z].rightEdge = false ;
                    }
                }
                else
                {
                    grid.Edges[z].upEdge = true ;
                    grid.Edges[z].downEdge = true ;
                    grid.Edges[z].leftEdge = true ;
                    grid.Edges[z].rightEdge = true ;
                }
            }
            startpoint.x += (CrossPoint.x - startpoint.x) ;
            temp.push_back(grid);
        }
        startpoint.x = 0 ;
        startpoint.y = h ;
        Grids.push_back(temp);
    }
    clock_t End = clock();
    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
    cout << "Initialize  Grid Graph Done" << endl ;
    cout << "We cost " << duration << "(s)" << endl;
//    printAllGrids();
    
}
