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


GlobalRouter::~GlobalRouter()
{
    
}

void GlobalRouter::toGridGraph()
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
void GlobalRouter::InitGrids()
{
    cout << "Begin Initialize Global Grid Graph ..." << endl;
    clock_t Start = clock();
    CutByBlockBoundary();
//    CutByUserDefine();
    Point<int> startpoint(0,0) ;
    for( auto h : Horizontal )
    {
        vector<Grid> temp ;
        for(auto v : Vertical)
        {
            Grid grid ;
            grid.capacities.resize(highest+1);
            Point<int> CrossPoint(v,h);
            grid.width = CrossPoint.x - startpoint.x ;
            grid.length = CrossPoint.y - startpoint.y ;
            grid.startpoint = startpoint ;
            pair<bool, string> result = RouterHelper.IsBlock(startpoint, Point<int>( startpoint.x + grid.width , startpoint.y + grid.length ) );
            if( get<0>(result) )
            {
                auto begin = MacroMaps[get<1>(result)].obs.InnerMaps.begin();
                auto end = --MacroMaps[get<1>(result)].obs.InnerMaps.end() ;
                grid.lowermetal = stoi(begin->first.substr(5));
                grid.uppermetal = stoi(end->first.substr(5));
            }
            // 代表非Block的區域
            if( grid.lowermetal == 0 && grid.uppermetal  == 0)
            {
                for( int i = lowest ; i <= highest ; i++ )
                    grid.capacities[i] = grid.width * grid.length ;
            }
            else
            {
                for( int i = lowest ; i <= highest ; i++ )
                {
                    if( i >= grid.lowermetal && i <= grid.uppermetal )
                        grid.capacities[i] = 0 ;
                    else
                        grid.capacities[i] = grid.width * grid.length ;
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
    cout << "Initialize Global Grid Graph Done" << endl ;
    cout << "We cost " << duration << "(s)" << endl;
//    printAllGrids();
    
}
void GlobalRouter::printAllGrids()
{
    
    for(auto a : Grids)
    {
        for(auto b : a)
        {
            Point<int> rightup(b.startpoint.x+b.width , b.startpoint.y + b.length);
            cout << "LeftDown:" << b.startpoint << " RightUp:" <<  rightup;
            cout << "[ " << b.lowermetal << "," << b.uppermetal << " ]" << endl;
            cout << "OverFlow:" << boolalpha << b.overflow << endl;
            cout << "Capacities:" << endl ;
            for( int i = 0 ; i < b.capacities.size() ; i++ )
                cout << "Layer" << i << ":" << b.capacities[i] << endl;
            cout << "----------------------------------------------------------------" << endl;
        }
        
    }
}
TwoDGridLocation GlobalRouter::get2DGridLocation(int x , int y)
{
    int x_lowest = 0 ;
    int x_highest = (int)Grids[0].size() - 1;
    int y_lowest = 0 ;
    int y_highest = (int)Grids.size() -1 ;
    
    if( x == x_lowest && y == y_lowest ) return vertex_leftdown ;
    else if( x == x_lowest && y == y_highest ) return vertex_leftup ;
    else if( x == x_highest && y == y_lowest ) return vertex_rightdown ;
    else if( x == x_highest && y == y_highest ) return vertex_rightup ;
    else if ( x == x_lowest ) return border_left ;
    else if( x == x_highest ) return border_right ;
    else if( y == y_lowest ) return border_bottom ;
    else if( y == y_highest ) return border_top ;
    else return other_2D ;
    assert(0);
    return other_2D ;
}
pair<TwoDGridLocation, ThreeDGridLocation> GlobalRouter::getGridLocation(int x , int y , int z )
{
    TwoDGridLocation TwoD = get2DGridLocation(x, y);
    ThreeDGridLocation ThreeD = get3DGridLocation(z);
    return make_pair(TwoD, ThreeD);
}
ThreeDGridLocation GlobalRouter::get3DGridLocation( int z)
{
    if( z == lowest ) return bottom ;
    else if (z == highest) return top ;
    else return other_3D ;
    assert(0);
    return other_3D ;
}
int GlobalRouter::translate3D_1D(int x , int y , int z)
{
    return translate2D_1D(x, y) +  (int)((z - 1) * Grids[0].size() * Grids.size() );
}
tuple<int,int,int> GlobalRouter::translate1D_3D(int index)
{
    int z = ( index / (int)(Grids[0].size() * Grids.size()) ) + 1 ;
    index %= (int)(Grids[0].size() * Grids.size()) ;
    auto xy = translate1D_2D(index);
    return make_tuple(xy.first, xy.second , z);
}
unsigned GlobalRouter::cost(int z , bool horizontal , Grid & grid)
{
    string key ;
    if( z == 1 ) key = "M1" ;
    if( z == 2 ) key = "M2" ;
    if( z == 3 ) key = "M3" ;
    if( z == 4 ) key = "M4" ;
    if( z == 5 ) key = "M5" ;
    if( z == 6 ) key = "M6" ;
    if( z == 7 ) key = "M7" ;
    if( z == 8 ) key = "M8" ;
    if( z == 9 ) key = "M9" ;
    if( z == 10 ) key = "M10" ;
    if( z == 11 ) key = "M11" ;
    if( z == 12 ) key = "M12" ;
    if( z == 13 ) key = "M13" ;
    if( z == 14 ) key = "M14" ;
    if( z == 15 ) key = "M15" ;
    unsigned weights = stod(WeightsMaps[key]) * 100 ;
    unsigned area = ( horizontal == true ) ? DEFAULT_WIDTH * grid.width : DEFAULT_WIDTH * grid.length ;
    return weights * area ;
}
void GlobalRouter::InitGraph_SP()
{
    cout << "Begin Initialize 3D Shortest Path Graph ..." << endl;
    clock_t Start = clock();
    int Size = (int)(Grids[0].size() * Grids.size() * highest ) ;
    graph.resize(Size);
    int RowSize = (int)Grids.size() - 1;
    int ColSize = (int)Grids[0].size() - 1;
//    int weight = 0 ;
    Edge edge ;
    for( int z = lowest ; z <= highest ; z++ )
    {
        for( int y = 0 ; y <= ColSize ; y++)
        {
            for( int x = 0 ; x <= RowSize ; x++ )
            {
                int from = translate3D_1D(x, y, z);
                vector<Edge> edges ;
                auto location = getGridLocation(x, y, z);
                switch (location.first)
                {
                    case vertex_rightup:
                        edge.to = translate3D_1D(x-1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x-1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y-1, z);
                        edge.weight = cost(z, false, Grids[y-1][x]);
                        edges.push_back(edge);
                        break;
                    case vertex_rightdown:
                        edge.to = translate3D_1D(x-1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x-1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y+1, z);
                        edge.weight = cost(z, false, Grids[y+1][x]);
                        edges.push_back(edge);
                        break;
                    case vertex_leftup:
                        edge.to = translate3D_1D(x+1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x+1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y-1, z);
                        edge.weight = cost(z, false, Grids[y-1][x]);
                        edges.push_back(edge);
                        break;
                    case vertex_leftdown:
                        edge.to = translate3D_1D(x+1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x+1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y+1, z);
                        edge.weight = cost(z, false, Grids[y+1][x]);
                        edges.push_back(edge);
                        break;
                    case border_top:
                        edge.to = translate3D_1D(x+1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x+1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x-1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x-1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y-1, z);
                        edge.weight = cost(z, false, Grids[y-1][x]);
                        edges.push_back(edge);
                        break;
                    case border_bottom :
                        edge.to = translate3D_1D(x+1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x+1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x-1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x-1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y+1, z);
                        edge.weight = cost(z, false, Grids[y+1][x]);
                        edges.push_back(edge);
                        break;
                    case border_right :
                        edge.to = translate3D_1D(x-1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x-1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y+1, z);
                        edge.weight = cost(z, false, Grids[y+1][x]);
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y-1, z);
                        edge.weight = cost(z, false, Grids[y-1][x]);
                        edges.push_back(edge);
                        break;
                    case border_left:
                        edge.to = translate3D_1D(x+1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x+1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y+1, z);
                        edge.weight = cost(z, false, Grids[y+1][x]);
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y-1, z);
                        edge.weight = cost(z, false, Grids[y-1][x]);
                        edges.push_back(edge);
                        break;
                    case other_2D:
                        edge.to = translate3D_1D(x+1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x+1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x-1 , y , z);
                        edge.weight = cost(z, true, Grids[y][x-1]) ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y+1, z);
                        edge.weight = cost(z, false, Grids[y+1][x]);
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x, y-1, z);
                        edge.weight = cost(z, false, Grids[y-1][x]);
                        edges.push_back(edge);
                        break;
                    default:
                        break;
                }
                switch (location.second)
                {
                    // 目前假設走via 沒有 penalty
                    case top:
                        edge.to = translate3D_1D(x , y , z-1);
                        edge.weight = 0 ;
                        edges.push_back(edge);
                        break;
                    case bottom:
                        edge.to = translate3D_1D(x , y , z+1);
                        edge.weight = 0 ;
                        edges.push_back(edge);
                        break;
                    case other_3D:
                        edge.to = translate3D_1D(x , y , z+1);
                        edge.weight = 0 ;
                        edges.push_back(edge);
                        
                        edge.to = translate3D_1D(x , y , z-1);
                        edge.weight = 0 ;
                        edges.push_back(edge);
                        break;
                    default:
                        break;
                }
                for( auto edge : edges )
                {
                    graph.AddEdge(from, edge.to, edge.weight);
                }
            }
        }
    }
    
    
//    for(int row = 0 ; row <= RowSize ; row++)
//    {
//        for( int col = 0 ; col <= ColSize ; col++ )
//        {
//            // 四周端點
//            if( (row == 0 && col == 0) || (row == RowSize && col == 0 )  || ( row == 0 && col == ColSize ) || ( row == RowSize && col == ColSize ) )
//            {
//                // 左下
//                if( row == 0 && col == 0 )
//                {
//                    int weight = ( RouterHelper.IsBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D(col , row), translate2D_1D(col, row + 1) , weight );
//                    weight = ( RouterHelper.IsBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D(col , row), translate2D_1D(col+1 , row) , weight );
//                }
//                //左上
//                else if ( row == RowSize && col == 0  )
//                {
//                    int weight = ( RouterHelper.IsBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D(col , row), translate2D_1D(col , row -1 ), weight);
//                    weight = ( RouterHelper.IsBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D(col , row), translate2D_1D( col + 1 , row ), weight);
//                }
//                //右下
//                else if (row == 0 && col == ColSize)
//                {
//                    int weight = ( RouterHelper.IsBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D(col , row ), translate2D_1D( col  , row + 1 ), weight);
//                    weight = ( RouterHelper.IsBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col - 1 , row ), weight);
//                }
//                //右上
//                else
//                {
//                    int weight = ( RouterHelper.IsBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col  , row - 1), weight);
//                    weight = ( RouterHelper.IsBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D(col , row ), translate2D_1D( col - 1 , row ), weight);
//                }
//            }
//            // 四周
//            else if( col == 0 || row == 0 || row == RowSize || col == ColSize )
//            {
//                // 下
//                if( row == 0 )
//                {
//                    int weight = ( RouterHelper.IsBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row + 1 ), weight);
//                    weight = ( RouterHelper.IsBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col +1 , row ), weight);
//                    weight = ( RouterHelper.IsBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col-1 , row ), weight);
//                }
//                // 左
//                else if ( col == 0 )
//                {
//                    int weight = ( RouterHelper.IsBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row + 1 ), weight);
//                    weight = ( RouterHelper.IsBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col +1 , row ), weight);
//                    weight = ( RouterHelper.IsBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row - 1), weight);
//                }
//                // 上
//                else if ( row == RowSize )
//                {
//                    int weight = ( RouterHelper.IsBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row - 1), weight);
//                    weight = ( RouterHelper.IsBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col +1 , row ), weight);
//                    weight = ( RouterHelper.IsBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col-1 , row ), weight);
//                }
//                // 右
//                else
//                {
//                    int weight = ( RouterHelper.IsBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row + 1 ), weight);
//                    weight = ( RouterHelper.IsBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col-1 , row ), weight);
//                    weight = ( RouterHelper.IsBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
//                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row - 1), weight);
//                }
//            }
//            // 四周
//            else
//            {
//                int weight = ( RouterHelper.IsBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
//                graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row + 1 ), weight);
//                weight = ( RouterHelper.IsBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
//                graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row - 1), weight);
//                weight = ( RouterHelper.IsBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
//                graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col-1 , row ), weight);
//                weight = ( RouterHelper.IsBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
//                graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col +1 , row ), weight);
//            }
//        }
//    }
    clock_t End = clock();
    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
    cout << "3D Shortest Path Graph Done" << endl ;
    cout << "We cost " << duration << "(s)" << endl;
}
pair<int, int> GlobalRouter::getGridCoordinate( Point<int> LeftDown , Point<int> RightUp )
{
    // find X coordinate of LeftDownX and RightUpX
    int LastX = 0 , CurrentX = 0 ;
    int LeftX = 0 , RightX = 0 ;
    for( int i = 0 ; i < Grids[0].size() ; i++)
    {
        CurrentX = LastX + Grids[0][i].width;
        if( LeftDown.x >= LastX && LeftDown.x <= CurrentX  )
            LeftX = i;
        if( RightUp.x >= LastX && RightUp.x <= CurrentX  )
            RightX = i;
        LastX = CurrentX ;
    }
    bool X_Mutliple = ( LeftX == RightX ) ? false : true ;
    // find Y coordinate of LeftDownY and RightUpY
    int LastY = 0 , CurrentY = 0 ;
    int BottomY = 0 , TopY = 0 ;
    for( int i = 0 ; i < Grids.size() ; i++)
    {
        CurrentY = LastY + Grids[i][0].length;
        if( LeftDown.y >= LastY && LeftDown.y <= CurrentY  )
            BottomY = i;
        if( RightUp.y >= LastY && RightUp.y <= CurrentY  )
            TopY = i;
        LastY = CurrentY ;
    }
    bool Y_Mutliple = ( BottomY == TopY ) ? false : true ;
    if( !X_Mutliple && !Y_Mutliple ) return  make_pair(RightX , TopY);
    if( X_Mutliple && !Y_Mutliple )
    {
        if( RightX - LeftX > 1 )
        {
            return make_pair(LeftX+1 , TopY);;
        }
        else
        {
            int Right_Boundary = Grids[0][LeftX].startpoint.x + Grids[0][LeftX].width ;
            if( Right_Boundary - LeftDown.x >= RightUp.x - Right_Boundary )
                return make_pair(LeftX , TopY);
            else
                return make_pair(RightX , TopY);
        }
    }
    if( !X_Mutliple && Y_Mutliple )
    {
        if( TopY - BottomY > 1 )
        {
            return make_pair(LeftX , BottomY + 1);;
        }
        else
        {
            int Up_Boundary = Grids[0][BottomY].startpoint.y + Grids[0][BottomY].length ;
            if( Up_Boundary - LeftDown.y >= RightUp.y - Up_Boundary )
                return make_pair(LeftX , BottomY);
            else
                return make_pair(LeftX , TopY);
        }
    }
    // 不預期會有Pin穿過2d的AREA
    assert(0);
}
pair<int, int> GlobalRouter::getGridCoordinate( Point<int> pt )
{
    // find X coordinate of LeftDownX and RightUpX
    int LastX = 0 , CurrentX = 0 ;
    int X = 0 ;
    for( int i = 0 ; i < Grids[0].size() ; i++)
    {
        CurrentX = LastX + Grids[0][i].width;
        if( pt.x >= LastX && pt.x <= CurrentX  )
            X = i;
        LastX = CurrentX ;
    }
    // find Y coordinate of LeftDownY and RightUpY
    int LastY = 0 , CurrentY = 0 ;
    int Y = 0 ;
    for( int i = 0 ; i < Grids.size() ; i++)
    {
        CurrentY = LastY + Grids[i][0].length;
        if( pt.y >= LastY && pt.y <= CurrentY  )
            Y = i;
        LastY = CurrentY ;
    }
    return make_pair(X, Y);
}
pair<int, int> GlobalRouter::ripple(int X , int Y)
{
    return make_pair(0, 0);
}
void GlobalRouter::Route()
{
    cout << "Begin Global Routing ..." << endl;
    clock_t Start = clock();
    // 走Dijkstra時，將所有Block都不視為障礙物
    // 但是使用Flute的時候，並不會讓steiner point 落在 障礙物上
    InitGraph_SP();
    flute_net flute;
    cout << "Global Routing:" << endl;
    for( auto it = Connection.begin(), end = Connection.end(); it != end;it = Connection.upper_bound(it->first))
    {
        cout << "PowerPin( Start ) : " << it->first ;
        // PowerPin( Start )
        pair< Point<int>, Point<int> > SCoordinate = RouterHelper.getPowerPinCoordinate(PinMaps[it->first].STARTPOINT.x, PinMaps[it->first].STARTPOINT.y, PinMaps[it->first].RELATIVE_POINT1, PinMaps[it->first].RELATIVE_POINT2, PinMaps[it->first].ORIENT);
        pair<int, int> SGridCoordinate = getGridCoordinate(get<0>(SCoordinate), get<1>(SCoordinate));
        cout << "( " << get<0>(SGridCoordinate) << " , " << get<1>(SGridCoordinate) << " )" << endl;
        
        vector<Point<int>> Points ;
        Grid SGrid = Grids[SGridCoordinate.second][SGridCoordinate.first] ;
        Point<int> Middle ( SGrid.startpoint.x +  SGrid.width / 2  , SGrid.startpoint.y + SGrid.length / 2);
        Points.push_back( Middle);
        auto beginning = Connection.lower_bound(it->first);
        auto endding = Connection.upper_bound(it->first);
        // 儲存所有Source Target的Point，轉換成Grid座標儲存
        while (beginning != endding)
        {
            // BlockPin(Target)
            Block block = RouterHelper.getBlock(beginning->second.BlockName, beginning->second.BlockPinName);
            pair<int, int> TGridCoordinate = getGridCoordinate(block.LeftDown, block.RightUp);
            if( block.Direction == LEFT ) get<0>(TGridCoordinate) -= 1  ;
            if( block.Direction == RIGHT ) get<0>(TGridCoordinate) += 1 ;
            if( block.Direction == TOP ) get<1>(TGridCoordinate) += 1;
            if( block.Direction == DOWN ) get<1>(TGridCoordinate) -= 1;
            cout << "BlockPin( Target ) : " << beginning->second.BlockName << "_" << beginning->second.BlockPinName;
            cout << "( " << get<0>(TGridCoordinate) << " , " << get<1>(TGridCoordinate) << " )" << endl;
            Grid TGrid = Grids[TGridCoordinate.second][TGridCoordinate.first] ;
            Point<int> Middle ( TGrid.startpoint.x +  TGrid.width / 2  , TGrid.startpoint.y + TGrid.length / 2);
            Points.push_back(Middle);
            beginning++;
        }
        flute.getSteinerTree(Points);
        for( int x = 0 ; x < flute.SteinerTree.size() ; x++ )
        {
            int X = flute.SteinerTree[x].x ;
            int Y = flute.SteinerTree[x].y ;
            int Target = flute.SteinerTree[x].target ;
            cout << x << ":" << "x=" << X << " y=" << Y ;
            cout << " e=" << Target << " ";
            cout << endl;
        }
        for( int x = 0 ; x < flute.SteinerTree.size() ; x++ )
        {
            int X = flute.SteinerTree[x].x ;
            int Y = flute.SteinerTree[x].y ;
            int Target = flute.SteinerTree[x].target ;
            

            pair<int,int> SGridCoordinate = getGridCoordinate(Point<int>(X,Y));
            Point<int> terminal = getTerminalCoordinate(X, Y, Target, flute.SteinerTree);
            pair<int,int> TGridCoordinate = getGridCoordinate(terminal);
            Grid SGrid = Grids[SGridCoordinate.second][SGridCoordinate.first];
            if( RouterHelper.IsBlock(SGrid))
            {
                //ripple(<#int X#>, <#int Y#>);
                cout << "Steiner Point is invalid " << endl;
                assert(0);
            }
            if( SGridCoordinate.first == TGridCoordinate.first && SGridCoordinate.second == TGridCoordinate.second ) continue;
            cout << " START:(" << SGridCoordinate.first << "," << SGridCoordinate.second << ")";
            cout << " TERMINAL:(" << TGridCoordinate.first << "," << TGridCoordinate.second << ")";
            cout << endl;
            graph.Dijkstra( translate2D_1D(SGridCoordinate.first, SGridCoordinate.second));
            vector<int> Path  = graph.getPath( translate2D_1D ( TGridCoordinate.first ,TGridCoordinate.second ) );
            
            printPath(Path);
            cout << endl;
//            cout << "Grid:" ;
//            cout << gridCoordinate.first << "," << gridCoordinate.second << " " ;
//            if( flute.SteinerTree[x].isSteiner ) cout << "s";
//            cout << x << ":" << "x=" << X << " y=" << Y ;
//            cout << " e=" << Target << " ";
//            cout << endl;
        }
        cout << "------------------------------------end------------------------------------" << endl;
    }
    clock_t End = clock();
    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
    cout << "Global Routing Done" << endl ;
    cout << "We cost " << duration << "(s)" << endl;
}
Point<int> GlobalRouter::getTerminalCoordinate(int x , int y , int target , vector<node> & SteierTree)
{
    int OldTarget = target , newTarget = target;
    int TargetX = SteierTree[target].x ;
    int TargetY = SteierTree[target].y ;
    while ( TargetX == x && TargetY == y  )
    {
        OldTarget = newTarget ;
        newTarget = SteierTree[target].target ;
        if( OldTarget == newTarget ) break;
        TargetX = SteierTree[newTarget].x ;
        TargetY = SteierTree[newTarget].y;
    }
    return Point<int>(TargetX,TargetY);
}
void GlobalRouter::printPath(vector<int> & path)
{
    for(int i = (int)path.size() - 1 ; i >= 0 ; i--)
    {
        pair<int, int> Two_D_Coor = translate1D_2D(path[i]) ;
        cout << "("<< Two_D_Coor.first << "," << Two_D_Coor.second << ")" ;
        if( i != 0 ) cout << "->" ;
    }
    
}
int GlobalRouter::translate2D_1D(int x , int y)
{
    size_t size = Grids[0].size() ;
    return y * (int)size + x ;
}
pair<int, int> GlobalRouter::translate1D_2D(int index)
{
    size_t size = Grids[0].size();
    int y = index / (int)size ;
    int x = index % (int)size ;
    return make_pair(x, y);
}





void GlobalRouter::CutByUserDefine()
{
    int x_diff = DIEAREA.pt2.x - DIEAREA.pt1.x ;
    int y_diff = DIEAREA.pt2.y - DIEAREA.pt1.y ;
    int default_length = 1 * UNITS_DISTANCE ;
    int scaling_length = default_length * Scaling ;
    
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


void GlobalRouter::CutByBlockBoundary()
{
    for (auto Conponent : ComponentMaps)
    {
        pair<Point<int> , Point<int>> BlockCoordinate = RouterHelper.getBlockCoordinate(Conponent.second.STARTPOINT.x, Conponent.second.STARTPOINT.y,MacroMaps[Conponent.second.MACROTYPE].WIDTH * UNITS_DISTANCE, MacroMaps[Conponent.second.MACROTYPE].LENGTH * UNITS_DISTANCE, Conponent.second.ORIENT);
        Vertical.insert(get<0>(BlockCoordinate).x);
        Vertical.insert(get<1>(BlockCoordinate).x);
        Horizontal.insert(get<0>(BlockCoordinate).y);
        Horizontal.insert(get<1>(BlockCoordinate).y);
    }
    Vertical.insert(DIEAREA.pt2.x);
    Horizontal.insert(DIEAREA.pt2.y);
    if( *Vertical.begin() == DIEAREA.pt1.x ) Vertical.erase(Vertical.begin());
    if( *Horizontal.begin() == DIEAREA.pt1.y ) Horizontal.erase(Horizontal.begin());
    
}
