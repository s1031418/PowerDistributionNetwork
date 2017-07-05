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
                {
                    unsigned capacity = grid.width / UNITS_DISTANCE * grid.length  / UNITS_DISTANCE;
                    grid.capacities[i] = capacity;
                }
            }
            else
            {
                for( int i = lowest ; i <= highest ; i++ )
                {
                    if( i >= grid.lowermetal && i <= grid.uppermetal )
                        grid.capacities[i] = 0 ;
                    else
                        grid.capacities[i] = grid.width / UNITS_DISTANCE * grid.length  / UNITS_DISTANCE ;
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
    
    unsigned weights = stod(WeightsMaps[key])*100;
    unsigned demand = ( horizontal == true ) ?   grid.width / UNITS_DISTANCE :   grid.length  / UNITS_DISTANCE ;
    unsigned scalingDemand = demand * DEFAULT_WIDTH  ;
    if( scalingDemand > grid.capacities[z] ) return Max_Distance ;
    return weights * demand  ;
}
void GlobalRouter::InitGraph_SP()
{
    cout << "Begin Initialize 3D Shortest Path Graph ..." << endl;
    clock_t Start = clock();
    int Size = (int)(Grids[0].size() * Grids.size() * highest ) ;
    graph.resize(Size);
    int YSize = (int)Grids.size() ;
    int XSize = (int)Grids[0].size() ;
//    int weight = 0 ;
    Edge edge ;
    for( int z = lowest ; z <= highest ; z++ )
    {
        for( int y = 0 ; y < YSize ; y++)
        {
            for( int x = 0 ; x < XSize ; x++ )
            {
                if (Grids[y][x].capacities[z] == 0) continue;
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
    
    clock_t End = clock();
    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
//    printAllGrids();
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
        if( pt.x >= LastX && pt.x < CurrentX  )
            X = i;
        LastX = CurrentX ;
    }
    // find Y coordinate of LeftDownY and RightUpY
    int LastY = 0 , CurrentY = 0 ;
    int Y = 0  ;

    for( int i = 0 ; i < Grids.size() ; i++)
    {
        CurrentY = LastY + Grids[i][0].length;
        if( pt.y >= LastY && pt.y < CurrentY  )
            Y = i  ;
        LastY = CurrentY ;
    }
    return make_pair(X, Y);
}
pair<int, int> GlobalRouter::ripple(int X , int Y)
{
    return make_pair(0, 0);
}
int GlobalRouter::getLength(vector<int> & path)
{
    unsigned sum = 0 ;
//    for( int i = 0 ; i < path.size() ; i++ )
//    {
//        auto currentCoorindate3d = translate1D_3D(path[i]);
//        cout << "x:" << get<0>(currentCoorindate3d) << ", y:" << get<1>(currentCoorindate3d) << ", z:" << get<2>(currentCoorindate3d) << endl;
//    }
    for( int i = 0 ; i < path.size() - 1  ; i++ )
    {
        bool horizontal = false, vertical = false ;
        auto currentCoorindate3d = translate1D_3D(path[i]);
        auto nextCoorindate3d = translate1D_3D(path[i+1]);
        // grid coordinate
        int currentXCoordinate = get<0>(currentCoorindate3d) ;
        int currentYCoordinate = get<1>(currentCoorindate3d) ;
        int nextXCoordinate = get<0>(nextCoorindate3d) ;
        int nextYCoordinate = get<1>(nextCoorindate3d) ;
        // if x the same means this is vertical line
        if( get<0>(currentCoorindate3d) == get<0>(nextCoorindate3d) ) vertical = true ;
        // if y the same means this is horizontal line
        if( get<1>(currentCoorindate3d) == get<1>(nextCoorindate3d) ) horizontal = true ;
        
        if( horizontal )
        {
            // absolute coordinate
            int currentCenterX = Grids[currentYCoordinate][currentXCoordinate].startpoint.x + ( Grids[currentYCoordinate][currentXCoordinate].width / 2 ) ;
            int nextCenterX = Grids[nextYCoordinate][nextXCoordinate].startpoint.x + ( Grids[nextYCoordinate][nextXCoordinate].width / 2 ) ;
            int diff = nextCenterX - currentCenterX ;
            sum += abs(diff) ;
        }
        if( vertical )
        {
            // absolute coordinate
            int currentCenterY = Grids[currentYCoordinate][currentXCoordinate].startpoint.y + ( Grids[currentYCoordinate][currentXCoordinate].length / 2 ) ;
            int nextCenterY = Grids[nextYCoordinate][nextXCoordinate].startpoint.y + ( Grids[nextYCoordinate][nextXCoordinate].length / 2 ) ;
            int diff = nextCenterY - currentCenterY ;
            sum += abs(diff) ;
        }

    }
    return sum ;
}
map<double,Path> GlobalRouter::getNetOrdering()
{
    InitGraph_SP();
//    if( netOrder == SHORTESTPATH )
//    {
//        
//    }
//    else if ( netOrder == IR_DROP )
//    {
    
        // key:PowerSourceName , value: criticalMap
        // In order to for global routing , multiterminal must be consider together .
        // if source is the same  , we use the same key to map different value 
        map<double,map<double,Path>> orders ;
        
        map<double,Path> criticalMap;
        for( auto it = Connection.begin() ; it != Connection.end() ; it = Connection.upper_bound(it->first))
        {
            // key:critical value( the smaller is more critical ) , value : Path( Define in RouteComponents )
            
            int index = 0 ;
            double sumCritical = 0 ;
            Pin pin = PinMaps[it->first];
            // source absoult coordinate
            auto source = RouterHelper.getPowerPinCoordinate(pin.STARTPOINT.x, pin.STARTPOINT.y, pin.RELATIVE_POINT1, pin.RELATIVE_POINT2, pin.ORIENT);
            auto sourceCenter = RouterHelper.getCenter(source.first, source.second);
            // source grid coordinate 
            auto sourceGrid = getGridCoordinate(sourceCenter);
            Coordinate3D SGridCoodinate(sourceGrid.first , sourceGrid.second , RouterHelper.translateMetalNameToInt(pin.METALNAME) );
            auto ret = Connection.equal_range(it->first);
            for (auto i = ret.first; i != ret.second; ++i)
            {
                index++;
                string constraint ;
//                cout << i->first << " " << i->second.BlockName << " " << i->second.BlockPinName << endl;
                auto target = RouterHelper.getBlock(i->second.BlockName, i->second.BlockPinName);
                int topLayer = RouterHelper.translateMetalNameToInt( target.Metals[target.Metals.size()-1] );
                auto targetCenter = RouterHelper.getCenter(target.LeftDown,target.RightUp);
                auto targetGrid = getGridCoordinate(targetCenter);
                if( target.Direction == LEFT ) get<0>(targetGrid) -= 1 ;
                if( target.Direction == RIGHT ) get<0>(targetGrid) += 1 ;
                if( target.Direction == TOP ) get<1>(targetGrid) += 1 ;
                if( target.Direction == DOWN ) get<1>(targetGrid) -= 1 ;
                Coordinate3D TGridCoodinate(targetGrid.first , targetGrid.second , topLayer );
                graph.Dijkstra(translate3D_1D(SGridCoodinate.x, SGridCoodinate.y, SGridCoodinate.z));
                vector<int> path = graph.getPath(translate3D_1D(targetGrid.first, targetGrid.second, topLayer));
                int length = getLength(path);
                auto iter = ConstraintMaps.find(i->second.BlockName) ;
                auto retu = ConstraintMaps.equal_range(iter->first);
                for (auto t = retu.first; t != retu.second; ++t)
                    if( t->second.NAME == i->second.BlockPinName ) constraint = t->second.CONSTRAINT;
                double critical = stod(constraint) / length ;
                sumCritical += critical ;
                // initialize path object
                Path p ;
                p.source = i->first ;
                p.target = make_pair(i->second.BlockName, i->second.BlockPinName);
                p.sourceGrid = SGridCoodinate ;
                p.targetGrid = TGridCoodinate ;
                // sorting by critical
                criticalMap.insert(make_pair(critical,p));
            }
//            double averageCritical = sumCritical / index ;
//            orders.insert(make_pair(averageCritical, criticalMap));
//        }
        // print crtical map
//        for( auto o : criticalMap )
//        {
//            cout << o.first << " " << o.second.source << " " << o.second.target.first << " " << o.second.target.second << endl;
//        }
        // assign critical map to the value of orders
//        for( auto x : criticalMap )
//        {
//            if( orders.find(x.second.source) == orders.end())
//            {
//                orders.insert(make_pair(x.second.source, map<double,Path>()));
//            }
//            orders[x.second.source].insert(make_pair(x.first, x.second));
//        }
        
        // print orders
//        for( auto x : orders )
//        {
//            cout << x.first << " " ;
//            for( auto y : x.second )
//                cout << y.first << " " << y.second.source << " " << y.second.target.first << " " << y.second.target.second << endl;
//            
//        }
//        return orders ;
    }
    return criticalMap;
    assert(0);
//    return map<double,map<double,Path>>();
}
//vector<Path> GlobalRouter::getNetOrdering()
//{
//    map<double,Path> orders;
//    vector<Path> criticalOrder;
//    // foreach powerpin
//    for( auto it = Connection.begin() ; it != Connection.end() ; it = Connection.upper_bound(it->first))
//    {
//        vector<Point<int>> points ;
//        vector<pair<string, string>> targets ;
//        Pin pin = PinMaps[it->first];
//        auto source = RouterHelper.getPowerPinCoordinate(pin.STARTPOINT.x, pin.STARTPOINT.y, pin.RELATIVE_POINT1, pin.RELATIVE_POINT2, pin.ORIENT);
//        Point<int> sourcePoint;
//        auto sourceCenter = RouterHelper.getCenter(source.first, source.second);
//        points.push_back(sourceCenter);
//        auto ret = Connection.equal_range(it->first);
//        for (auto i = ret.first; i != ret.second; ++i)
//        {
//            auto block = RouterHelper.getBlock(i->second.BlockName, i->second.BlockPinName);
//            targets.push_back(make_pair(i->second.BlockName, i->second.BlockPinName));
//            points.push_back(RouterHelper.getCenter(block.LeftDown, block.RightUp));
//        }
//
//        flute_net flute;
//        flute.getSteinerTree(points);
//        for( auto target : targets )
//        {
//            string constraint ;
//            Path path ;
//            auto block = RouterHelper.getBlock(target.first, target.second);
//            auto targetCenter = RouterHelper.getCenter(block.LeftDown, block.RightUp);
//            vector<Point<int>> pathOrder = flute.getShortestPathOrder(sourceCenter, targetCenter);
//            unsigned shortestLength = flute.getShortestPathLength(pathOrder);
//            auto iter = ConstraintMaps.find(target.first) ;
//            auto retu = ConstraintMaps.equal_range(iter->first);
//            for (auto i = retu.first; i != retu.second; ++i)
//                if( i->second.NAME == target.second ) constraint = i->second.CONSTRAINT;
//            double critical = stod(constraint) / shortestLength ;
//            path.source = it->first ;
//            path.target = make_pair(it->second.BlockName, it->second.BlockPinName);
//            orders.insert(make_pair(critical, path));
//        }
//    }
//    
//    for( auto x : orders )
//        criticalOrder.push_back(x.second);
//    return criticalOrder ;
//    
//}
unsigned GlobalRouter::estimateCritical(vector<Point<int>> & Points)
{
    return 0 ;
}

void GlobalRouter::updateGraph_SP(set<int> & UpdateGrids , map<int,int> & hvTable)
{
    int x , y , z ;
    for( auto index : UpdateGrids )
    {
        auto index3D = translate1D_3D(index);
        x = get<0>(index3D);
        y = get<1>(index3D);
        z = get<2>(index3D);
        int demand = 0 ;
        double spacing = LayerMaps[RouterHelper.translateIntToMetalName(z)].SPACING ;
        if( hvTable[index] == 0 ) demand = ( DEFAULT_WIDTH + 2 * spacing ) * Grids[y][x].length / UNITS_DISTANCE ;
        if( hvTable[index] == 1 ) demand = ( DEFAULT_WIDTH + 2 * spacing )  * Grids[y][x].width / UNITS_DISTANCE ;
        if( hvTable[index] == 2 ) demand = ( Grids[y][x].length > Grids[y][x].width ) ? Grids[y][x].length * ( DEFAULT_WIDTH + 2 * spacing )  / UNITS_DISTANCE : Grids[y][x].width * ( DEFAULT_WIDTH + 2 * spacing )  / UNITS_DISTANCE ;
        Grids[y][x].capacities[z] -= demand ;
        int up = ( y + 1 < Grids.size() ) ? y + 1 : -1 ;
        int down = ( y - 1 > 0 ) ? y - 1 : -1 ;
        int right = ( x + 1 < Grids[0].size() ) ? x + 1 : -1 ;
        int left = ( x - 1 > 0 ) ? x - 1 : -1 ;
        if( up != -1 )
        {
            int demand = DEFAULT_WIDTH * Grids[y][x].length / UNITS_DISTANCE ;
            if( demand > Grids[y][x].capacities[z] )
                graph.UpdateWeight(translate3D_1D(x, up, z), index, Max_Distance);
        }
        if( right != -1 )
        {
            int demand = DEFAULT_WIDTH * Grids[y][x].width / UNITS_DISTANCE ;
            if( demand > Grids[y][x].capacities[z] )
                graph.UpdateWeight(translate3D_1D(right, y, z), index, Max_Distance);
        }
        if( left != -1 )
        {
            int demand = DEFAULT_WIDTH * Grids[y][x].width / UNITS_DISTANCE ;
            if( demand > Grids[y][x].capacities[z] )
                graph.UpdateWeight(translate3D_1D(left, y, z), index, Max_Distance);
        }
        if( down != -1 )
        {
            int demand = DEFAULT_WIDTH * Grids[y][x].length / UNITS_DISTANCE ;
            if( demand > Grids[y][x].capacities[z] )
                graph.UpdateWeight(translate3D_1D(x, down, z), index, Max_Distance);
        }
    }
    
}

void GlobalRouter::Route()
{
    cout << "Begin Global Routing ..." << endl;
    clock_t Start = clock();
    
    
    InitGraph_SP();
    cout << "Global Routing:" << endl;
    // Decide Net Ordering
    // Net order 是用 PowerSoure crtical 的平均 做排序 , 以 PowerSource 為單位
    // 將來或許可以以線為單位，但是不好實作（所以目前先以VDD為考量）
//    auto orders = getNetOrdering(IR_DROP) ;
    
    
//    for(auto o : orders)
//    {
//        cout << o.first << " " << o.second.source << " " << o.second.target.first << " " << o.second.target.second << endl;
//    }
//    cout << "Net Order List " << endl;
//    for( auto o : orders )
//    {
//        cout << "Average Critical:" << o.first * 1000000 << endl ;
//        for( auto p : o.second )
//        {
//            cout << p.second.source << " " << p.second.target.first << " " << p.second.target.second << " " << p.first * 1000000 << endl;
//        }
//    }
//    cout << "-----------------------------------------------------" << endl;
//    for( auto source : orders )
//    {
//
//        set<int> updateGrids ;
//        // vertical 0  , horizontal 1 , crossroad 2
//        map<int,int> hvTable ; // look for the grid is horizontal or vertical
//        
//        for( auto target : source.second )
//        {
//            cout << target.second.source << " " << target.second.target.first << " " << target.second.target.second << " " << target.first * 1000000 << endl;
//            auto sourceGridCoordinate = target.second.sourceGrid ;
//            auto targetGridCoordinate = target.second.targetGrid ;
//            GlobalSolution gsol ;
//            gsol.PowerSourceName = target.second.source ;
//            gsol.TargetName = make_pair(target.second.target.first, target.second.target.second);
//            gsol.source = sourceGridCoordinate ;
//            gsol.targets.push_back(targetGridCoordinate);
//            graph.Dijkstra(translate3D_1D(sourceGridCoordinate.x, sourceGridCoordinate.y, sourceGridCoordinate.z));
//            vector<int> paths = graph.getPath(translate3D_1D(targetGridCoordinate.x,targetGridCoordinate.y,targetGridCoordinate.z));
//            for(int i = (int)paths.size() -1 ; i >= 0   ; i--)
//            {
//                bool horizontal = false, vertical = false ;
//                auto currentCoorindate3d = translate1D_3D(paths[i]);
//                auto nextCoorindate3d = translate1D_3D(paths[i+1]);
//                // if x the same means this is vertical line
//                if( get<0>(currentCoorindate3d) == get<0>(nextCoorindate3d) ) vertical = true ;
//                // if y the same means this is horizontal line
//                if( get<1>(currentCoorindate3d) == get<1>(nextCoorindate3d) ) horizontal = true ;
//                // if find
//                if( hvTable.find(paths[i]) != hvTable.end() )
//                {
//                    if( vertical && hvTable[paths[i]] == 1)
//                        hvTable[paths[i]] = 2 ;
//                    if( horizontal && hvTable[paths[i]] == 0)
//                        hvTable[paths[i]] = 2 ;
//                }
//                else
//                {
//                    if( vertical ) hvTable.insert(make_pair(paths[i], 0));
//                    if( horizontal ) hvTable.insert(make_pair(paths[i], 1));
//                }
//                updateGrids.insert(paths[i]);
//            }
//            for( auto grid : updateGrids)
//            {
//                auto index3D = translate1D_3D(grid);
//                Coordinate3D coordinate3d(get<0>(index3D) ,get<1>(index3D),get<2>(index3D));
//                gsol.paths.push_back(coordinate3d);
//            }
//            //gsol.paths.assign(updateGrids.begin(), updateGrids.end());
//            GlobalSolutions.push_back(gsol);
////            for( auto s : sol )
////                cout << s.x << "," << s.y << ","<< s.z << endl;
//        }
//        updateGraph_SP(updateGrids, hvTable);
//    }
    
    
//    for( auto it = Connection.begin(), end = Connection.end(); it != end;it = Connection.upper_bound(it->first))
//    {
//        cout << "PowerPin( Start ) : " << it->first ;
//        // PowerPin( Start )
//        pair< Point<int>, Point<int> > SCoordinate = RouterHelper.getPowerPinCoordinate(PinMaps[it->first].STARTPOINT.x, PinMaps[it->first].STARTPOINT.y, PinMaps[it->first].RELATIVE_POINT1, PinMaps[it->first].RELATIVE_POINT2, PinMaps[it->first].ORIENT);
//        pair<int, int> SGridCoordinate = getGridCoordinate(get<0>(SCoordinate), get<1>(SCoordinate));
//        cout << "( " << get<0>(SGridCoordinate) << " , " << get<1>(SGridCoordinate) << " )" << endl;
//        
//        vector<Point<int>> Points ;
//        Grid SGrid = Grids[SGridCoordinate.second][SGridCoordinate.first] ;
//        Point<int> Middle ( SGrid.startpoint.x +  SGrid.width / 2  , SGrid.startpoint.y + SGrid.length / 2);
//        Points.push_back( Middle);
//        auto beginning = Connection.lower_bound(it->first);
//        auto endding = Connection.upper_bound(it->first);
//        // 儲存所有Source Target的Point，轉換成Grid座標儲存
//        while (beginning != endding)
//        {
//            // BlockPin(Target)
//            Block block = RouterHelper.getBlock(beginning->second.BlockName, beginning->second.BlockPinName);
//            pair<int, int> TGridCoordinate = getGridCoordinate(block.LeftDown, block.RightUp);
//            if( block.Direction == LEFT ) get<0>(TGridCoordinate) -= 1  ;
//            if( block.Direction == RIGHT ) get<0>(TGridCoordinate) += 1 ;
//            if( block.Direction == TOP ) get<1>(TGridCoordinate) += 1;
//            if( block.Direction == DOWN ) get<1>(TGridCoordinate) -= 1;
//            cout << "BlockPin( Target ) : " << beginning->second.BlockName << "_" << beginning->second.BlockPinName;
//            cout << "( " << get<0>(TGridCoordinate) << " , " << get<1>(TGridCoordinate) << " )" << endl;
//            Grid TGrid = Grids[TGridCoordinate.second][TGridCoordinate.first] ;
//            Point<int> Middle ( TGrid.startpoint.x +  TGrid.width / 2  , TGrid.startpoint.y + TGrid.length / 2);
//            Points.push_back(Middle);
//            beginning++;
//        }
//        flute.getSteinerTree(Points);
//        for( int x = 0 ; x < flute.SteinerTree.size() ; x++ )
//        {
//            int X = flute.SteinerTree[x].x ;
//            int Y = flute.SteinerTree[x].y ;
//            int Target = flute.SteinerTree[x].target ;
//            cout << x << ":" << "x=" << X << " y=" << Y ;
//            cout << " e=" << Target << " ";
//            cout << endl;
//        }
//        for( int x = 0 ; x < flute.SteinerTree.size() ; x++ )
//        {
//            int X = flute.SteinerTree[x].x ;
//            int Y = flute.SteinerTree[x].y ;
//            int Target = flute.SteinerTree[x].target ;
//            
//
//            pair<int,int> SGridCoordinate = getGridCoordinate(Point<int>(X,Y));
//            Point<int> terminal = getTerminalCoordinate(X, Y, Target, flute.SteinerTree);
//            pair<int,int> TGridCoordinate = getGridCoordinate(terminal);
//            Grid SGrid = Grids[SGridCoordinate.second][SGridCoordinate.first];
//            if( RouterHelper.IsBlock(SGrid))
//            {
//                //ripple(<#int X#>, <#int Y#>);
//                cout << "Steiner Point is invalid " << endl;
//                assert(0);
//            }
//            if( SGridCoordinate.first == TGridCoordinate.first && SGridCoordinate.second == TGridCoordinate.second ) continue;
//            cout << " START:(" << SGridCoordinate.first << "," << SGridCoordinate.second << ")";
//            cout << " TERMINAL:(" << TGridCoordinate.first << "," << TGridCoordinate.second << ")";
//            cout << endl;
//            graph.Dijkstra( translate2D_1D(SGridCoordinate.first, SGridCoordinate.second));
//            vector<int> Path  = graph.getPath( translate2D_1D ( TGridCoordinate.first ,TGridCoordinate.second ) );
//            
//            printPath(Path);
//            cout << endl;
////            cout << "Grid:" ;
////            cout << gridCoordinate.first << "," << gridCoordinate.second << " " ;
////            if( flute.SteinerTree[x].isSteiner ) cout << "s";
////            cout << x << ":" << "x=" << X << " y=" << Y ;
////            cout << " e=" << Target << " ";
////            cout << endl;
//        }
//        cout << "------------------------------------end------------------------------------" << endl;
//    }
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
    int default_length = 12 * UNITS_DISTANCE ;
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
