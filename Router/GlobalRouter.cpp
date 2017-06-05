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
    initGrids();
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
            if( isBlock( col )) cout << " METAL5 " ;
            else cout << " METAL6 " ;
            cout << col.Width << " " ;
            cout << " + SHAPE STRIPE ";
            cout << " ( "<< col.StartPoint.x + col.Width / 2 ;
            cout << " "<< col.StartPoint.y << " )" ;
            cout << " ( "<< col.StartPoint.x + col.Width / 2 ;
            cout << " "<< col.StartPoint.y + col.Length << " )" ;
            cout << endl;
        }
    }
    
}
void GlobalRouter::initGrids()
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
            StartPoint.x += (CrossPoint.x - StartPoint.x) ;
            temp.push_back(grid);
        }
        StartPoint.x = 0 ;
        StartPoint.y = h ;
        Grids.push_back(temp);
    }
    
//    for(auto a : Grids)
//    {
//        for(auto b : a)
//        {
//            Point<int> rightup(b.StartPoint.x+b.Width , b.StartPoint.y + b.Length);
//            cout << "LeftDown:" << b.StartPoint << " RightUp:" <<  rightup;
//            cout << "[ " << b.LowerMetal << "," << b.UpperMetal << " ]" << endl;
//        }
//    }
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
bool GlobalRouter::isBlock(Grid grid)
{
    if( grid.LowerMetal == 0 && grid.UpperMetal == 0 )
        return false ;
    else
        return true ; 
}
void GlobalRouter::initGraph_SP()
{
    int Size = (int)(Grids[0].size() * Grids.size()) ;
    Graph_SP g(Size);
    int RowSize = (int)Grids.size() - 1;
    int ColSize = (int)Grids[0].size() - 1;
    for(int row = 0 ; row <= RowSize ; row++)
    {
        for( int col = 0 ; col <= ColSize ; col++ )
        {
            // 四周端點
            if( (row == 0 && col == 0) || (row == RowSize && col == 0 )  || ( row == 0 && col == ColSize ) || ( row == RowSize && col == ColSize ) )
            {
                // 左下
                if( row == 0 && col == 0 )
                {
                    int weight = ( isBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row + 1, col) , weight );
                    weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row , col+1) , weight );
                }
                //左上
                else if ( row == RowSize && col == 0  )
                {
                    int weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row - 1, col ), weight);
                    weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row , col + 1), weight);
                }
                //右下
                else if (row == 0 && col == ColSize)
                {
                    int weight = ( isBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row + 1, col ), weight);
                    weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row , col - 1), weight);
                }
                //右上
                else
                {
                    int weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row - 1, col ), weight);
                    weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row , col - 1), weight);
                }
            }
            // 四周
            else if( col == 0 || row == 0 || row == RowSize || col == ColSize )
            {
                // 下
                if( row == 0 )
                {
                    int weight = ( isBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row + 1, col ), weight);
                    weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row, col +1), weight);
                    weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row , col-1 ), weight);
                }
                // 左
                else if ( col == 0 )
                {
                    int weight = ( isBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row + 1, col ), weight);
                    weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row, col +1), weight);
                    weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row-1, col ), weight);
                }
                // 上
                else if ( row == RowSize )
                {
                    int weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row - 1, col ), weight);
                    weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row, col +1), weight);
                    weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row , col-1 ), weight);
                }
                // 右
                else
                {
                    int weight = ( isBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row + 1, col ), weight);
                    weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row, col-1), weight);
                    weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                    g.AddEdge(translate2D_1D(row, col), translate2D_1D(row-1, col ), weight);
                }
            }
            // 四周
            else
            {
                int weight = ( isBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
                g.AddEdge(translate2D_1D(row, col), translate2D_1D(row + 1, col ), weight);
                weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                g.AddEdge(translate2D_1D(row, col), translate2D_1D(row-1, col ), weight);
                weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                g.AddEdge(translate2D_1D(row, col), translate2D_1D(row, col-1), weight);
                weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                g.AddEdge(translate2D_1D(row, col), translate2D_1D(row, col +1), weight);
            }
        }
    }
    g.Dijkstra();
    g.getPath(8);
}
void GlobalRouter::Route()
{
    initGraph_SP();
}
int GlobalRouter::translate2D_1D(int row , int column)
{
    size_t size = Grids[0].size() ;
    return row * (int)size + column ;
}
pair<int, int> GlobalRouter::translate1D_2D(int index)
{
    size_t size = Grids[0].size();
    int row = index / (int)size ;
    int col = index % (int)size ;
    return make_pair(row, col);
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
