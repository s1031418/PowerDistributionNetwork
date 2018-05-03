////
////  Predictor.cpp
////  EDA_Contest2017(PDN)
////
////
//
//#include "Predictor.hpp"
//
//
//
//
//Predictor::Predictor()
//{
//    vector<string> metals ;
//    for( auto x : LayerMaps )
//    {
//        if( x.second.TYPE == "ROUTING" )
//            metals.push_back(x.first);
//    }
//    lowest = stoi(metals[0].substr(5));
//    highest = stoi(metals[metals.size()-1].substr(5));
//    InitGrids();
//}
//
//
//Predictor::~Predictor()
//{
//}
//
//
//void Predictor::InitGrids()
//{
//    cout << "Begin Initialize Global Grid Graph ..." << endl;
//    clock_t Start = clock();
//    CutByBlockBoundary();
//    //    CutByUserDefine();
//    Point<int> startpoint(0,0) ;
//    for( auto h : Horizontal )
//    {
//        vector<Grid> temp ;
//        for(auto v : Vertical)
//        {
//            Grid grid ;
//            grid.capacities.resize(highest+1);
//            Point<int> CrossPoint(v,h);
//            grid.width = CrossPoint.x - startpoint.x ;
//            grid.length = CrossPoint.y - startpoint.y ;
//            grid.startpoint = startpoint ;
//            pair<bool, string> result = RouterHelper.IsBlock(startpoint, Point<int>( startpoint.x + grid.width , startpoint.y + grid.length ) );
//            if( get<0>(result) )
//            {
//                auto begin = MacroMaps[get<1>(result)].obs.InnerMaps.begin();
//                auto end = --MacroMaps[get<1>(result)].obs.InnerMaps.end() ;
//                grid.lowermetal = stoi(begin->first.substr(5));
//                grid.uppermetal = stoi(end->first.substr(5));
//            }
//            // 代表非Block的區域
//            if( grid.lowermetal == 0 && grid.uppermetal  == 0)
//            {
//                for( int i = lowest ; i <= highest ; i++ )
//                {
//                    unsigned capacity = grid.width / UNITS_DISTANCE * grid.length  / UNITS_DISTANCE;
//                    grid.capacities[i] = capacity;
//                }
//            }
//            else
//            {
//                for( int i = lowest ; i <= highest ; i++ )
//                {
//                    if( i >= grid.lowermetal && i <= grid.uppermetal )
//                        grid.capacities[i] = 0 ;
//                    else
//                        grid.capacities[i] = grid.width / UNITS_DISTANCE * grid.length  / UNITS_DISTANCE ;
//                }
//            }
//            
//            startpoint.x += (CrossPoint.x - startpoint.x) ;
//            temp.push_back(grid);
//        }
//        startpoint.x = 0 ;
//        startpoint.y = h ;
//        Grids.push_back(temp);
//    }
//    clock_t End = clock();
//    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
//    cout << "Initialize Global Grid Graph Done" << endl ;
//    cout << "We cost " << duration << "(s)" << endl;
//    //    printAllGrids();
//    
//}
//void Predictor::CutByBlockBoundary()
//{
//    for (auto Conponent : ComponentMaps)
//    {
//        pair<Point<int> , Point<int>> BlockCoordinate = RouterHelper.getBlockCoordinate(Conponent.second.STARTPOINT.x, Conponent.second.STARTPOINT.y,MacroMaps[Conponent.second.MACROTYPE].WIDTH * UNITS_DISTANCE, MacroMaps[Conponent.second.MACROTYPE].LENGTH * UNITS_DISTANCE, Conponent.second.ORIENT);
//        Vertical.insert(get<0>(BlockCoordinate).x);
//        Vertical.insert(get<1>(BlockCoordinate).x);
//        Horizontal.insert(get<0>(BlockCoordinate).y);
//        Horizontal.insert(get<1>(BlockCoordinate).y);
//    }
//    Vertical.insert(DIEAREA.pt2.x);
//    Horizontal.insert(DIEAREA.pt2.y);
//    if( *Vertical.begin() == DIEAREA.pt1.x ) Vertical.erase(Vertical.begin());
//    if( *Horizontal.begin() == DIEAREA.pt1.y ) Horizontal.erase(Horizontal.begin());
//    
//}
//int Predictor::getApproximateManhattanDistance(Block block1 , Block block2 )
//{
//    Point<int> block1Center , block2Center;
//    // calculate center of block1
//    block1Center.x = (block1.LeftDown.x + block1.RightUp.x)/2;
//    block1Center.y = (block1.LeftDown.y + block1.RightUp.y)/2;
//    // calculate center of block2
//    block2Center.x = (block2.LeftDown.x + block2.RightUp.x)/2;
//    block2Center.y = (block2.LeftDown.y + block2.RightUp.y)/2;
//    return getManhattanDistance(block1Center,block2Center);
//}
//int Predictor::getManhattanDistance(Point<int> pt1 , Point<int> pt2)
//{
//    int xDistance = abs(pt1.x - pt2.x);
//    int yDistance = abs(pt1.y - pt2.y);
//    return xDistance + yDistance ;
//}
//void Predictor::estimate()
//{
//    for( auto it = Connection.begin() ; it != Connection.end() ; it = Connection.upper_bound(it->first))
//    {
//        auto ret = Connection.equal_range(it->first);
//        for (auto i = ret.first; i != ret.second; ++i)
//        {
//            string powerpin = i->first ;
//            string block = i->second.BlockName ;
//            string blockPin = i->second.BlockPinName ;
//            Block powerPinCoordinate = RouterHelper.getPowerPinCoordinate(powerpin);
//            Block blockPinCoordinate = RouterHelper.getBlock(block,blockPin);
//            int distance = getApproximateManhattanDistance(powerPinCoordinate,blockPinCoordinate);
//            double irConstaint = RouterHelper.getIRDropConstaint(block, blockPin) / 100 ;
//            double sourceV = RouterHelper.getSourceVoltage(powerpin);
//            double current = RouterHelper.getCurrent(block, blockPin);
//            double deltaV = sourceV * irConstaint ;
//            
//            
////            auto powerGrid = getGridCoordinate(powerPinCoordinate);
////            auto blockPinGrid = getGridCoordinate(blockPinCoordinate);
////            int currentIndex = translate3D_1D(Coordinate3D(powerGrid.x,powerGrid.y,powerGrid.z));
////            currentIndex = translate3D_1D(Coordinate3D(blockPinGrid.x,blockPinGrid.y,blockPinGrid.z));
////            if( powerpin != sourcePowerPin ) BlockGridCoordinate(graph_sp,powerPinCoordinate);
////            BlockGridCoordinate(graph_sp,blockPinCoordinate);
//        }
//    }
//    
//}
//unsigned Predictor::cost(int z , bool horizontal , Grid & grid)
//{
//    string key ;
//    if( z == 1 ) key = "M1" ;
//    if( z == 2 ) key = "M2" ;
//    if( z == 3 ) key = "M3" ;
//    if( z == 4 ) key = "M4" ;
//    if( z == 5 ) key = "M5" ;
//    if( z == 6 ) key = "M6" ;
//    if( z == 7 ) key = "M7" ;
//    if( z == 8 ) key = "M8" ;
//    if( z == 9 ) key = "M9" ;
//    if( z == 10 ) key = "M10" ;
//    if( z == 11 ) key = "M11" ;
//    if( z == 12 ) key = "M12" ;
//    if( z == 13 ) key = "M13" ;
//    if( z == 14 ) key = "M14" ;
//    if( z == 15 ) key = "M15" ;
//    
//    unsigned weights = stod(WeightsMaps[key])*100;
//    unsigned demand = ( horizontal == true ) ?   grid.width / UNITS_DISTANCE :   grid.length  / UNITS_DISTANCE ;
//    unsigned scalingDemand = demand * DEFAULT_WIDTH  ;
//    if( scalingDemand > grid.capacities[z] ) return Max_Distance ;
//    return weights * demand  ;
//}
//void Predictor::InitGraph_SP()
//{
//    int Size = (int)(Grids[0].size() * Grids.size() * highest ) ;
//    graph.resize(Size);
//    int YSize = (int)Grids.size() ;
//    int XSize = (int)Grids[0].size() ;
//    //    int weight = 0 ;
//    Edge edge ;
//    for( int z = lowest ; z <= highest ; z++ )
//    {
//        for( int y = 0 ; y < YSize ; y++)
//        {
//            for( int x = 0 ; x < XSize ; x++ )
//            {
//                if (Grids[y][x].capacities[z] == 0) continue;
//                int from = translate3D_1D(x, y, z);
//                vector<Edge> edges ;
//                auto location = getGridLocation(x, y, z);
//                
//                switch (location.first)
//                {
//                    case vertex_rightup:
//                        edge.to = translate3D_1D(x-1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x-1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y-1, z);
//                        edge.weight = cost(z, false, Grids[y-1][x]);
//                        edges.push_back(edge);
//                        break;
//                    case vertex_rightdown:
//                        edge.to = translate3D_1D(x-1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x-1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y+1, z);
//                        edge.weight = cost(z, false, Grids[y+1][x]);
//                        edges.push_back(edge);
//                        break;
//                    case vertex_leftup:
//                        edge.to = translate3D_1D(x+1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x+1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y-1, z);
//                        edge.weight = cost(z, false, Grids[y-1][x]);
//                        edges.push_back(edge);
//                        break;
//                    case vertex_leftdown:
//                        edge.to = translate3D_1D(x+1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x+1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y+1, z);
//                        edge.weight = cost(z, false, Grids[y+1][x]);
//                        edges.push_back(edge);
//                        break;
//                    case border_top:
//                        edge.to = translate3D_1D(x+1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x+1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x-1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x-1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y-1, z);
//                        edge.weight = cost(z, false, Grids[y-1][x]);
//                        edges.push_back(edge);
//                        break;
//                    case border_bottom :
//                        edge.to = translate3D_1D(x+1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x+1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x-1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x-1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y+1, z);
//                        edge.weight = cost(z, false, Grids[y+1][x]);
//                        edges.push_back(edge);
//                        break;
//                    case border_right :
//                        edge.to = translate3D_1D(x-1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x-1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y+1, z);
//                        edge.weight = cost(z, false, Grids[y+1][x]);
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y-1, z);
//                        edge.weight = cost(z, false, Grids[y-1][x]);
//                        edges.push_back(edge);
//                        break;
//                    case border_left:
//                        edge.to = translate3D_1D(x+1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x+1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y+1, z);
//                        edge.weight = cost(z, false, Grids[y+1][x]);
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y-1, z);
//                        edge.weight = cost(z, false, Grids[y-1][x]);
//                        edges.push_back(edge);
//                        break;
//                    case other_2D:
//                        edge.to = translate3D_1D(x+1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x+1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x-1 , y , z);
//                        edge.weight = cost(z, true, Grids[y][x-1]) ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y+1, z);
//                        edge.weight = cost(z, false, Grids[y+1][x]);
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x, y-1, z);
//                        edge.weight = cost(z, false, Grids[y-1][x]);
//                        edges.push_back(edge);
//                        break;
//                    default:
//                        break;
//                }
//                switch (location.second)
//                {
//                        // 目前假設走via 沒有 penalty
//                    case top:
//                        edge.to = translate3D_1D(x , y , z-1);
//                        edge.weight = 0 ;
//                        edges.push_back(edge);
//                        break;
//                    case bottom:
//                        edge.to = translate3D_1D(x , y , z+1);
//                        edge.weight = 0 ;
//                        edges.push_back(edge);
//                        break;
//                    case other_3D:
//                        edge.to = translate3D_1D(x , y , z+1);
//                        edge.weight = 0 ;
//                        edges.push_back(edge);
//                        
//                        edge.to = translate3D_1D(x , y , z-1);
//                        edge.weight = 0 ;
//                        edges.push_back(edge);
//                        break;
//                    default:
//                        break;
//                }
//                for( auto edge : edges )
//                {
//                    graph.AddEdge(from, edge.to, edge.weight);
//                }
//            }
//        }
//    }
//    
//}
//map<double,Path> Predictor::getNetOrdering()
//{
//    InitGraph_SP();
//    // key:PowerSourceName , value: criticalMap
//    // In order to for global routing , multiterminal must be consider together .
//    // if source is the same  , we use the same key to map different value
//    map<double,map<double,Path>> orders ;
//    
//    map<double,Path> criticalMap;
//    for( auto it = Connection.begin() ; it != Connection.end() ; it = Connection.upper_bound(it->first))
//    {
//        // key:critical value( the smaller is more critical ) , value : Path( Define in RouteComponents )
//        
//        int index = 0 ;
//        double sumCritical = 0 ;
//        Pin pin = PinMaps[it->first];
//        // source absoult coordinate
//        auto source = RouterHelper.getPowerPinCoordinate(pin.STARTPOINT.x, pin.STARTPOINT.y, pin.RELATIVE_POINT1, pin.RELATIVE_POINT2, pin.ORIENT);
//        auto sourceCenter = RouterHelper.getCenter(source.first, source.second);
//        // source grid coordinate
//        auto sourceGrid = getGridCoordinate(sourceCenter);
//        Coordinate3D SGridCoodinate(sourceGrid.first , sourceGrid.second , RouterHelper.translateMetalNameToInt(pin.METALNAME) );
//        auto ret = Connection.equal_range(it->first);
//        for (auto i = ret.first; i != ret.second; ++i)
//        {
//            index++;
//            string constraint ;
//            //                cout << i->first << " " << i->second.BlockName << " " << i->second.BlockPinName << endl;
//            auto target = RouterHelper.getBlock(i->second.BlockName, i->second.BlockPinName);
//            int topLayer = RouterHelper.translateMetalNameToInt( target.Metals[target.Metals.size()-1] );
//            auto targetCenter = RouterHelper.getCenter(target.LeftDown,target.RightUp);
//            auto targetGrid = getGridCoordinate(targetCenter);
//            if( target.Direction == LEFT ) get<0>(targetGrid) -= 1 ;
//            if( target.Direction == RIGHT ) get<0>(targetGrid) += 1 ;
//            if( target.Direction == TOP ) get<1>(targetGrid) += 1 ;
//            if( target.Direction == DOWN ) get<1>(targetGrid) -= 1 ;
//            Coordinate3D TGridCoodinate(targetGrid.first , targetGrid.second , topLayer );
//            graph.Dijkstra(translate3D_1D(SGridCoodinate.x, SGridCoodinate.y, SGridCoodinate.z));
//            vector<int> path = graph.getPath(translate3D_1D(targetGrid.first, targetGrid.second, topLayer));
//            int length = getLength(path);
//            auto iter = ConstraintMaps.find(i->second.BlockName) ;
//            auto retu = ConstraintMaps.equal_range(iter->first);
//            for (auto t = retu.first; t != retu.second; ++t)
//                if( t->second.NAME == i->second.BlockPinName ) constraint = t->second.CONSTRAINT;
//            double critical = stod(constraint) / length ;
//            sumCritical += critical ;
//            // initialize path object
//            Path p ;
//            p.source = i->first ;
//            p.target = make_pair(i->second.BlockName, i->second.BlockPinName);
//            p.sourceGrid = SGridCoodinate ;
//            p.targetGrid = TGridCoodinate ;
//            // sorting by critical
//            criticalMap.insert(make_pair(critical,p));
//        }
//        //            double averageCritical = sumCritical / index ;
//        //            orders.insert(make_pair(averageCritical, criticalMap));
//        //        }
//        // print crtical map
//        //        for( auto o : criticalMap )
//        //        {
//        //            cout << o.first << " " << o.second.source << " " << o.second.target.first << " " << o.second.target.second << endl;
//        //        }
//        // assign critical map to the value of orders
//        //        for( auto x : criticalMap )
//        //        {
//        //            if( orders.find(x.second.source) == orders.end())
//        //            {
//        //                orders.insert(make_pair(x.second.source, map<double,Path>()));
//        //            }
//        //            orders[x.second.source].insert(make_pair(x.first, x.second));
//        //        }
//        
//        // print orders
//        //        for( auto x : orders )
//        //        {
//        //            cout << x.first << " " ;
//        //            for( auto y : x.second )
//        //                cout << y.first << " " << y.second.source << " " << y.second.target.first << " " << y.second.target.second << endl;
//        //
//        //        }
//        //        return orders ;
//    }
//    return criticalMap;
//    assert(0);
//}
//
//pair<int, int> Predictor::getGridCoordinate( Point<int> pt )
//{
//    // find X coordinate of LeftDownX and RightUpX
//    int LastX = 0 , CurrentX = 0 ;
//    int X = 0 ;
//    for( int i = 0 ; i < Grids[0].size() ; i++)
//    {
//        CurrentX = LastX + Grids[0][i].width;
//        if( pt.x >= LastX && pt.x < CurrentX  )
//            X = i;
//        LastX = CurrentX ;
//    }
//    // find Y coordinate of LeftDownY and RightUpY
//    int LastY = 0 , CurrentY = 0 ;
//    int Y = 0  ;
//    
//    for( int i = 0 ; i < Grids.size() ; i++)
//    {
//        CurrentY = LastY + Grids[i][0].length;
//        if( pt.y >= LastY && pt.y < CurrentY  )
//            Y = i  ;
//        LastY = CurrentY ;
//    }
//    return make_pair(X, Y);
//}
//
//int Predictor::translate3D_1D(int x , int y , int z)
//{
//    return translate2D_1D(x, y) +  (int)((z - 1) * Grids[0].size() * Grids.size() );
//}
//tuple<int,int,int> Predictor::translate1D_3D(int index)
//{
//    int z = ( index / (int)(Grids[0].size() * Grids.size()) ) + 1 ;
//    index %= (int)(Grids[0].size() * Grids.size()) ;
//    auto xy = translate1D_2D(index);
//    return make_tuple(xy.first, xy.second , z);
//}
//
//int Predictor::translate2D_1D(int x , int y)
//{
//    size_t size = Grids[0].size() ;
//    return y * (int)size + x ;
//}
//pair<int, int> Predictor::translate1D_2D(int index)
//{
//    size_t size = Grids[0].size();
//    int y = index / (int)size ;
//    int x = index % (int)size ;
//    return make_pair(x, y);
//}
//int Predictor::getLength(vector<int> & path)
//{
//    unsigned sum = 0 ;
//    //    for( int i = 0 ; i < path.size() ; i++ )
//    //    {
//    //        auto currentCoorindate3d = translate1D_3D(path[i]);
//    //        cout << "x:" << get<0>(currentCoorindate3d) << ", y:" << get<1>(currentCoorindate3d) << ", z:" << get<2>(currentCoorindate3d) << endl;
//    //    }
//    for( int i = 0 ; i < path.size() - 1  ; i++ )
//    {
//        bool horizontal = false, vertical = false ;
//        auto currentCoorindate3d = translate1D_3D(path[i]);
//        auto nextCoorindate3d = translate1D_3D(path[i+1]);
//        // grid coordinate
//        int currentXCoordinate = get<0>(currentCoorindate3d) ;
//        int currentYCoordinate = get<1>(currentCoorindate3d) ;
//        int nextXCoordinate = get<0>(nextCoorindate3d) ;
//        int nextYCoordinate = get<1>(nextCoorindate3d) ;
//        // if x the same means this is vertical line
//        if( get<0>(currentCoorindate3d) == get<0>(nextCoorindate3d) ) vertical = true ;
//        // if y the same means this is horizontal line
//        if( get<1>(currentCoorindate3d) == get<1>(nextCoorindate3d) ) horizontal = true ;
//        
//        if( horizontal )
//        {
//            // absolute coordinate
//            int currentCenterX = Grids[currentYCoordinate][currentXCoordinate].startpoint.x + ( Grids[currentYCoordinate][currentXCoordinate].width / 2 ) ;
//            int nextCenterX = Grids[nextYCoordinate][nextXCoordinate].startpoint.x + ( Grids[nextYCoordinate][nextXCoordinate].width / 2 ) ;
//            int diff = nextCenterX - currentCenterX ;
//            sum += abs(diff) ;
//        }
//        if( vertical )
//        {
//            // absolute coordinate
//            int currentCenterY = Grids[currentYCoordinate][currentXCoordinate].startpoint.y + ( Grids[currentYCoordinate][currentXCoordinate].length / 2 ) ;
//            int nextCenterY = Grids[nextYCoordinate][nextXCoordinate].startpoint.y + ( Grids[nextYCoordinate][nextXCoordinate].length / 2 ) ;
//            int diff = nextCenterY - currentCenterY ;
//            sum += abs(diff) ;
//        }
//        
//    }
//    return sum ;
//}
//
//// correspond the index , calculate the 3D location of this point
//ThreeDGridLocation Predictor::get3DGridLocation(int z)
//{
//    if( z == lowest ) return bottom ;
//    else if (z == highest) return top ;
//    else return other_3D ;
//    assert(0);
//    return other_3D ;
//}
//
//// correspond the index , calculate the 2D location of this point
//TwoDGridLocation Predictor::get2DGridLocation(int x , int y)
//{
//    int x_lowest = 0 ;
//    int x_highest = (int)Grids[0].size() - 1;
//    int y_lowest = 0 ;
//    int y_highest = (int)Grids.size() -1 ;
//    
//    if( x == x_lowest && y == y_lowest ) return vertex_leftdown ;
//    else if( x == x_lowest && y == y_highest ) return vertex_leftup ;
//    else if( x == x_highest && y == y_lowest ) return vertex_rightdown ;
//    else if( x == x_highest && y == y_highest ) return vertex_rightup ;
//    else if ( x == x_lowest ) return border_left ;
//    else if( x == x_highest ) return border_right ;
//    else if( y == y_lowest ) return border_bottom ;
//    else if( y == y_highest ) return border_top ;
//    else return other_2D ;
//    assert(0);
//    return other_2D ;
//}
//
//// get 3D 2D location of this point
//pair<TwoDGridLocation, ThreeDGridLocation> Predictor::getGridLocation(int x , int y , int z )
//{
//    TwoDGridLocation TwoD = get2DGridLocation(x, y);
//    ThreeDGridLocation ThreeD = get3DGridLocation(z);
//    return make_pair(TwoD, ThreeD);
//}
