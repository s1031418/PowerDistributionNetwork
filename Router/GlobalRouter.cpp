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
    graph.resize(Size);
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
                    graph.AddEdge(translate2D_1D(col , row), translate2D_1D(col, row + 1) , weight );
                    weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D(col , row), translate2D_1D(col+1 , row) , weight );
                }
                //左上
                else if ( row == RowSize && col == 0  )
                {
                    int weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D(col , row), translate2D_1D(col , row -1 ), weight);
                    weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D(col , row), translate2D_1D( col + 1 , row ), weight);
                }
                //右下
                else if (row == 0 && col == ColSize)
                {
                    int weight = ( isBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D(col , row ), translate2D_1D( col  , row + 1 ), weight);
                    weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col - 1 , row ), weight);
                }
                //右上
                else
                {
                    int weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col  , row - 1), weight);
                    weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D(col , row ), translate2D_1D( col - 1 , row ), weight);
                }
            }
            // 四周
            else if( col == 0 || row == 0 || row == RowSize || col == ColSize )
            {
                // 下
                if( row == 0 )
                {
                    int weight = ( isBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row + 1 ), weight);
                    weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col +1 , row ), weight);
                    weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col-1 , row ), weight);
                }
                // 左
                else if ( col == 0 )
                {
                    int weight = ( isBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row + 1 ), weight);
                    weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col +1 , row ), weight);
                    weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row - 1), weight);
                }
                // 上
                else if ( row == RowSize )
                {
                    int weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row - 1), weight);
                    weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col +1 , row ), weight);
                    weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col-1 , row ), weight);
                }
                // 右
                else
                {
                    int weight = ( isBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row + 1 ), weight);
                    weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col-1 , row ), weight);
                    weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                    graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row - 1), weight);
                }
            }
            // 四周
            else
            {
                int weight = ( isBlock(Grids[row+1][col]) ) ? Max_Distance : 1 ;
                graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row + 1 ), weight);
                weight = ( isBlock(Grids[row-1][col]) ) ? Max_Distance : 1 ;
                graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col , row - 1), weight);
                weight = ( isBlock(Grids[row][col-1]) ) ? Max_Distance : 1 ;
                graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col-1 , row ), weight);
                weight = ( isBlock(Grids[row][col+1]) ) ? Max_Distance : 1 ;
                graph.AddEdge(translate2D_1D( col , row ), translate2D_1D(col +1 , row ), weight);
            }
        }
    }
    
}
pair<int, int> GlobalRouter::getGridCoordinate( Point<int> LeftDown , Point<int> RightUp )
{
    // find X coordinate of LeftDownX and RightUpX
    int LastX = 0 , CurrentX = 0 ;
    int LeftX = 0 , RightX = 0 ;
    for( int i = 0 ; i < Grids[0].size() ; i++)
    {
        CurrentX = LastX + Grids[0][i].Width;
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
        CurrentY = LastY + Grids[i][0].Length;
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
            int Right_Boundary = Grids[0][LeftX].StartPoint.x + Grids[0][LeftX].Width ;
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
            int Up_Boundary = Grids[0][BottomY].StartPoint.y + Grids[0][BottomY].Length ;
            if( Up_Boundary - LeftDown.y >= RightUp.y - Up_Boundary )
                return make_pair(LeftX , BottomY);
            else
                return make_pair(LeftX , TopY);
        }
    }
    // 不預期會有Pin穿過2d的AREA
    assert(0);
}
void GlobalRouter::Route()
{
    initGraph_SP();
    flute_net flute;
    cout << "Global Routing:" << endl;
    for( auto it = Connection.begin(), end = Connection.end(); it != end;it = Connection.upper_bound(it->first))
    {
        cout << "PowerPin( Start ) : " << it->first ;
        // PowerPin( Start )
        pair< Point<int>, Point<int> > SCoordinate = helper.getPowerPinCoordinate(PinMaps[it->first].STARTPOINT.x, PinMaps[it->first].STARTPOINT.y, PinMaps[it->first].RELATIVE_POINT1, PinMaps[it->first].RELATIVE_POINT2, PinMaps[it->first].ORIENT);
        pair<int, int> SGridCoordinate = getGridCoordinate(get<0>(SCoordinate), get<1>(SCoordinate));
        cout << "( " << get<0>(SGridCoordinate) << " , " << get<1>(SGridCoordinate) << " )" << endl;
        
        vector<Point<int>> Points ;
        Points.push_back(Point<int>(get<0>(SGridCoordinate) , get<1>(SGridCoordinate) ));
        auto beginning = Connection.lower_bound(it->first);
        auto endding = Connection.upper_bound(it->first);
        // 儲存所有Source Target的Point，轉換成Grid座標儲存
        while (beginning != endding)
        {
            // BlockPin(Target)
            Block block = helper.getBlock(beginning->second.BlockName, beginning->second.BlockPinName);
            pair<int, int> TGridCoordinate = getGridCoordinate(block.LeftDown, block.RightUp);
            if( block.Direction == LEFT ) get<0>(TGridCoordinate) -= 1  ;
            if( block.Direction == RIGHT ) get<0>(TGridCoordinate) += 1 ;
            if( block.Direction == TOP ) get<1>(TGridCoordinate) += 1;
            if( block.Direction == DOWN ) get<1>(TGridCoordinate) -= 1;
            cout << "BlockPin( Target ) : " << beginning->second.BlockName << "_" << beginning->second.BlockPinName;
            cout << "( " << get<0>(TGridCoordinate) << " , " << get<1>(TGridCoordinate) << " )" << endl;
            Points.push_back(Point<int>( get<0>(TGridCoordinate) , get<1>(TGridCoordinate) )  );
            beginning++;
        }
        // 一個Source一個Target
        if( Points.size() == 2 )
        {
            graph.Dijkstra(translate2D_1D(Points[0].x , Points[0].y ));
            vector<int> Path = graph.getPath(translate2D_1D(Points[1].x , Points[1].y ));
            printPath(Path);
            cout << endl;
        }
        // 一個Source多個Target，用Flute
        else
        {
            flute.getSteinerTree(Points);
            for( int x = 0 ; x < flute.SteinerTree.size() ; x++ )
            {
                int X = flute.SteinerTree[x].x ;
                int Y = flute.SteinerTree[x].y ;
                int Target = flute.SteinerTree[x].target ;
                cout << x << ":" << "x=" << X << " y=" << Y ;
                cout << " e=" << Target << " ";
//                cout << Grids[ flute.SteinerTree[x].y ][ flute.SteinerTree[x].x ].StartPoint ;
                if( flute.SteinerTree[x].isSteiner &&  isBlock (Grids[Y][X] ))
                {
                    // Steiner Point 在 Block 上
                    cout << "Steiner Point is invalid " << endl;
                    // 還沒處理
                    assert(0);
                }
                graph.Dijkstra( translate2D_1D(X, Y));
//                if( flute.SteinerTree[ Target ].x == X &&  flute.SteinerTree[ Target ].y == Y )
//                {
//                    cout << " go myself " << endl;
//                    continue;
//                }
                Point<int> terminal = getTerminalGridCoordinate(X, Y, Target, flute.SteinerTree);
                if( terminal.x == X && terminal.y == Y )
                {
                    cout << "(" << flute.SteinerTree[x].x << "," << flute.SteinerTree[x].y << ")->";
                    cout << "(" << flute.SteinerTree[x].x << "," << flute.SteinerTree[x].y << ")"<< endl;
                    continue;
                }
                vector<int> Path  = graph.getPath( translate2D_1D ( terminal.x ,terminal.y )  );
//                vector<int> Path  = graph.getPath( translate2D_1D ( flute.SteinerTree[ Target ].x  , flute.SteinerTree[ Target ].y )  );
                printPath(Path);
                cout << endl;
            }
            
        }
    }
    
}
Point<int> GlobalRouter::getTerminalGridCoordinate(int x , int y , int target , vector<node> & SteierTree)
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
    if( *Vertical.begin() == DIEAREA.pt1.x ) Vertical.erase(Vertical.begin());
    if( *Horizontal.begin() == DIEAREA.pt1.y ) Horizontal.erase(Horizontal.begin());
}
