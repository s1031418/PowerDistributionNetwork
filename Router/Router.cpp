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
    
    InitGrids();
    InitBoundList();
    InitSpPoints();
    // Clear SpecialNet Map
    SpecialNetsMaps.clear();
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
void Router::InitSpPoints()
{
    for( auto it = Connection.begin() ; it != Connection.end() ; it = Connection.upper_bound(it->first))
    {
        auto ret = Connection.equal_range(it->first);
        for (auto i = ret.first; i != ret.second; ++i)
        {
            cout << i->first << endl;
            cout << i->second.BlockName << " " << i->second.BlockPinName << endl;
            Block powerPinCoordinate = RouterHelper.getPowerPinCoordinate(i->first);
            Block BlockPinCoordinate = RouterHelper.getBlock(i->second.BlockName, i->second.BlockPinName);
            int powerCandidate = getCandidates(powerPinCoordinate);
            auto powerCandidateCoordinate3D = translate1D_3D(powerCandidate);
            int blockPinCandidate = getCandidates(BlockPinCoordinate);
            auto blockPinCandidateCoordinate3D = translate1D_3D(blockPinCandidate);
            SpecialPoints.insert(make_pair(i->first,powerCandidateCoordinate3D));
            string key = i->second.BlockName + i->second.BlockPinName ;
            SpecialPoints.insert(make_pair(key,blockPinCandidateCoordinate3D));
        }
    }
}
// 測試用
void Router::updateGrids(vector<int> paths , Graph_SP & graph_sp )
{
    for( int i = 0 ; i < paths.size(); i++ )
    {
        auto coordinate3d = translate1D_3D(paths[i]);
        int x = coordinate3d.x ;
        int y = coordinate3d.y ;
        int z = coordinate3d.z ;
        bool isLeftEdgeLegal = false , isRightEdgeLegal = false, isUpEdgeLegal = false , isDownEdgeLegal = false ;
        isLeftEdgeLegal = (x - 1 < 0) ? false : true ;
        isRightEdgeLegal = (x + 1 > Grids[0].size()) ? false : true ;
        isUpEdgeLegal = (y + 1 > Grids.size()) ? false : true ;
        isDownEdgeLegal = (y - 1 < 0) ? false : true ;
        if( isLeftEdgeLegal )
        {
            int to = translate3D_1D(Coordinate3D(x-1,y,z));
            graph_sp.UpdateWeight(paths[i], to, Max_Distance);
            graph_sp.UpdateWeight(to, paths[i], Max_Distance);
            Grids[y][x].Edges[z].leftEdge = false ;
            Grids[y][x-1].Edges[z].rightEdge = false ;
        }
        if(isRightEdgeLegal)
        {
            int to = translate3D_1D(Coordinate3D(x+1,y,z));
            graph_sp.UpdateWeight(paths[i], to, Max_Distance);
            graph_sp.UpdateWeight(to, paths[i], Max_Distance);
            Grids[y][x].Edges[z].rightEdge = false ;
            Grids[y][x+1].Edges[z].leftEdge = false ;
        }
        if( isUpEdgeLegal )
        {
            int to = translate3D_1D(Coordinate3D(x,y+1,z));
            graph_sp.UpdateWeight(paths[i], to, Max_Distance);
            graph_sp.UpdateWeight(to, paths[i], Max_Distance);
            Grids[y][x].Edges[z].upEdge = false ;
            Grids[y+1][x].Edges[z].downEdge = false ;
        }
        if(isDownEdgeLegal)
        {
            int to = translate3D_1D(Coordinate3D(x,y-1,z));
            graph_sp.UpdateWeight(paths[i], to, Max_Distance);
            graph_sp.UpdateWeight(to, paths[i], Max_Distance);
            Grids[y][x].Edges[z].downEdge = false ;
            Grids[y-1][x].Edges[z].upEdge = false ;
        }
    }
}
int Router::getCandidates( Block block )
{
    DIRECTION direction = block.Direction ;
    
    int topLayer =  RouterHelper.translateMetalNameToInt( block.Metals[block.Metals.size() - 1] );
    int xLowerBound = block.LeftDown.x / DEFAULT_PITCH / UNITS_DISTANCE ;
    int xUpperBound = block.RightUp.x / DEFAULT_PITCH / UNITS_DISTANCE + 1 ;
    int yLowerBound = block.LeftDown.y / DEFAULT_PITCH / UNITS_DISTANCE ;
    int yUpperBound = block.RightUp.y / DEFAULT_PITCH / UNITS_DISTANCE + 1 ;
    cout << "xLowerBound:" << xLowerBound << " " << "xUpperBound:" << xUpperBound << " " << endl;
    cout << "yLowerBound:" << yLowerBound << " " << "yUpperBound:" << yUpperBound << " " << endl;
    int xCandidate = 0 , yCandidate = 0 ;
    if( direction == TOP )
    {
        yCandidate = yUpperBound ;
        xCandidate = (xLowerBound + xUpperBound ) / 2 ;
        while( !Grids[yCandidate-1][xCandidate].Edges[topLayer].upEdge )
            yCandidate++;
    }
    else if( direction == LEFT )
    {
        xCandidate = xLowerBound ;
        yCandidate = (yLowerBound + yUpperBound)/2 ;
        while( !Grids[yCandidate][xCandidate].Edges[topLayer].rightEdge )
            xCandidate--;
    }
    else if( direction == RIGHT )
    {
        xCandidate = xUpperBound ;
        yCandidate = (yLowerBound + yUpperBound)/2 ;
        while( !Grids[yCandidate][xCandidate-1].Edges[topLayer].rightEdge )
            xCandidate++;
    }
    else if( direction == DOWN )
    {
        yCandidate = yLowerBound ;
        xCandidate = (xLowerBound + xUpperBound ) / 2 ;
        while( !Grids[yCandidate][xCandidate].Edges[topLayer].upEdge )
            yCandidate--;
    }
    return translate3D_1D(Coordinate3D(xCandidate,yCandidate,topLayer));
}
RelativeDirection Router::getRelativeDirection( Block b1 , Block b2 )
{
    Point<int> CenterPointOfB1 , CenterPointOfB2;
    CenterPointOfB1.x = (b1.LeftDown.x + b1.RightUp.x)/2;
    CenterPointOfB1.y = (b1.LeftDown.y + b1.RightUp.y)/2;
    CenterPointOfB2.x = (b2.LeftDown.x + b2.RightUp.x)/2;
    CenterPointOfB2.y = (b2.LeftDown.y + b2.RightUp.y)/2;
    bool up = false, down = false, left = false , right = false ;
    if( CenterPointOfB1.x < CenterPointOfB2.x ) right = true ;
    if( CenterPointOfB1.x > CenterPointOfB2.x ) left = true ;
    if( CenterPointOfB1.y < CenterPointOfB2.y ) up = true ;
    if( CenterPointOfB1.y > CenterPointOfB2.y ) down = true ;
    
    
    if( up && left ) return NW ;
    else if( up && right) return NE ;
    else if( down && left ) return SW ;
    else if( down && right) return SE ;
    
    assert(0);
}
void Router::Route()
{
    cout << "Begin Routing ..." << endl;
    clock_t Start = clock();
    int YSize = (int)Grids.size() ;
    int XSize = (int)Grids[0].size() ;
//    int Size = XSize * YSize * highest ;
//    Graph_SP graph_sp(Size);
    GlobalRouter gr ;
    auto orders = gr.getNetOrdering();
    // 先不決定ordering
    for( auto order : orders)
    {
        SBox sbox ;
        sbox.lowestMetalLayer = lowest ;
        sbox.highestMetalLayer = highest ;
        sbox.LeftDown = Point<int>(0,0);
        sbox.RightUp = Point<int>(XSize-1,YSize-1);
        string source = order.second.source ;
        string targetBlock = order.second.target.first;
        string targetBlockPin = order.second.target.second ;
        set<int> updateGridSet;
        BlockInfo blockinfo ;
        blockinfo.BlockName = targetBlock ;
        blockinfo.BlockPinName = targetBlockPin ;
        if( Connection.count(source) > 1) continue ;
        Block powerPinCoordinate = RouterHelper.getPowerPinCoordinate(source);
        Block BlockPinCoordinate = RouterHelper.getBlock(targetBlock, targetBlockPin);
        auto relativeDirection = getRelativeDirection(powerPinCoordinate, BlockPinCoordinate);
        int powerCandidate = getCandidates(powerPinCoordinate);
        int blockPinCandidate = getCandidates(BlockPinCoordinate);
        vector<Coordinate3D> finalPaths ;
        if( relativeDirection == NW )
        {
            Graph_SP * graph_sp_W = InitGraph_SP(sbox,LEFT );
            graph_sp_W->Dijkstra(powerCandidate);
            auto solutionW= graph_sp_W->getPath(blockPinCandidate);
            delete [] graph_sp_W ;
            Graph_SP * graph_sp_N = InitGraph_SP(sbox, TOP);
            graph_sp_N->Dijkstra(powerCandidate);
            auto solutionN= graph_sp_N->getPath(blockPinCandidate);
            delete [] graph_sp_N ;
            if( getTurnCount(solutionW) < getTurnCount(solutionN) )
            {
                for(auto sol : solutionW)
                {
                    updateGridSet.insert(sol);
                    auto coordinate3d = translate1D_3D(sol);
                    finalPaths.push_back(coordinate3d);
                }
            }
            else
            {
                for(auto sol : solutionN)
                {
                    updateGridSet.insert(sol);
                    auto coordinate3d = translate1D_3D(sol);
                    finalPaths.push_back(coordinate3d);
                }
            }
        }
        else if( relativeDirection == NE )
        {
            
            Graph_SP * graph_sp_E = InitGraph_SP(sbox,RIGHT );
            graph_sp_E->Dijkstra(powerCandidate);
            auto solutionE= graph_sp_E->getPath(blockPinCandidate);
            delete [] graph_sp_E ;
            Graph_SP * graph_sp_N = InitGraph_SP(sbox, TOP);
            graph_sp_N->Dijkstra(powerCandidate);
            auto solutionN= graph_sp_N->getPath(blockPinCandidate);
            delete [] graph_sp_N ;
            cout << getTurnCount(solutionE) << endl;
            cout << getTurnCount(solutionN) << endl;
            if( getTurnCount(solutionE) < getTurnCount(solutionN) )
            {
                for(auto sol : solutionE)
                {
                    updateGridSet.insert(sol);
                    auto coordinate3d = translate1D_3D(sol);
                    finalPaths.push_back(coordinate3d);
                }
            }
            else
            {
                for(auto sol : solutionN)
                {
                    updateGridSet.insert(sol);
                    auto coordinate3d = translate1D_3D(sol);
                    finalPaths.push_back(coordinate3d);
                }
            }
        }
        else if( relativeDirection == SW )
        {
            Graph_SP * graph_sp_W = InitGraph_SP(sbox,LEFT );
            graph_sp_W->Dijkstra(powerCandidate);
            auto solutionW= graph_sp_W->getPath(blockPinCandidate);
            delete [] graph_sp_W ;
            Graph_SP * graph_sp_S = InitGraph_SP(sbox, DOWN);
            graph_sp_S->Dijkstra(powerCandidate);
            auto solutionS= graph_sp_S->getPath(blockPinCandidate);
            delete [] graph_sp_S ;
            if( getTurnCount(solutionW) < getTurnCount(solutionS) )
            {
                for(auto sol : solutionW)
                {
                    updateGridSet.insert(sol);
                    auto coordinate3d = translate1D_3D(sol);
                    finalPaths.push_back(coordinate3d);
                }
            }
            else
            {
                for(auto sol : solutionS)
                {
                    updateGridSet.insert(sol);
                    auto coordinate3d = translate1D_3D(sol);
                    finalPaths.push_back(coordinate3d);
                }
            }
        }
        else if( relativeDirection == SE )
        {
            Graph_SP * graph_sp_E = InitGraph_SP(sbox,RIGHT );
            graph_sp_E->Dijkstra(powerCandidate);
            auto solutionE= graph_sp_E->getPath(blockPinCandidate);
            delete [] graph_sp_E ;
            Graph_SP * graph_sp_S = InitGraph_SP(sbox, DOWN);
            graph_sp_S->Dijkstra(powerCandidate);
            auto solutionS= graph_sp_S->getPath(blockPinCandidate);
            delete [] graph_sp_S ;
            if( getTurnCount(solutionE) < getTurnCount(solutionS) )
            {
                for(auto sol : solutionE)
                {
                    updateGridSet.insert(sol);
                    auto coordinate3d = translate1D_3D(sol);
                    finalPaths.push_back(coordinate3d);
                }
            }
            else
            {
                for(auto sol : solutionS)
                {
                    updateGridSet.insert(sol);
                    auto coordinate3d = translate1D_3D(sol);
                    finalPaths.push_back(coordinate3d);
                }
            }
        }
        fillSpNetMaps(finalPaths, source, blockinfo);
        // Update Grids
        vector<int> updateIndex ;
        updateIndex.assign(updateGridSet.begin(), updateGridSet.end());
        updateGrids(updateIndex);
        Converter converter("TEST");
        converter.toOutputDef();
        // free graph_sp
    }
//    for( auto it = Connection.begin() ; it != Connection.end() ; it = Connection.upper_bound(it->first))
//    {
//        SBox sbox ;
//        sbox.lowestMetalLayer = lowest ;
//        sbox.highestMetalLayer = highest ;
//        sbox.LeftDown = Point<int>(0,0);
//        sbox.RightUp = Point<int>(XSize-1,YSize-1);
//
//        set<int> updateGridSet;
//        // Single Source Single Target
//        if( Connection.count(it->first) == 1 )
//        {
//            vector<BlockInfo> targetPinNames ;
//            BlockInfo blockinfo ;
//            blockinfo.BlockName = it->second.BlockName ;
//            blockinfo.BlockPinName = it->second.BlockPinName ;
//            cout << it->first << " " << it->second.BlockName << " " << it->second.BlockPinName << endl;
//            Block powerPinCoordinate = RouterHelper.getPowerPinCoordinate(it->first);
//            Block BlockPinCoordinate = RouterHelper.getBlock(it->second.BlockName, it->second.BlockPinName);
//            auto relativeDirection = getRelativeDirection(powerPinCoordinate, BlockPinCoordinate);
//            int powerCandidate = getCandidates(powerPinCoordinate);
//            int blockPinCandidate = getCandidates(BlockPinCoordinate);
//            vector<Coordinate3D> finalPaths ;
//            if( relativeDirection == NW )
//            {
//                Graph_SP * graph_sp_W = InitGraph_SP(sbox,LEFT );
//                graph_sp_W->Dijkstra(powerCandidate);
//                auto solutionW= graph_sp_W->getPath(blockPinCandidate);
//                delete [] graph_sp_W ;
//                Graph_SP * graph_sp_N = InitGraph_SP(sbox, TOP);
//                graph_sp_N->Dijkstra(powerCandidate);
//                auto solutionN= graph_sp_N->getPath(blockPinCandidate);
//                delete [] graph_sp_N ;
//                if( getTurnCount(solutionW) < getTurnCount(solutionN) )
//                {
//                    for(auto sol : solutionW)
//                    {
//                        updateGridSet.insert(sol);
//                        auto coordinate3d = translate1D_3D(sol);
//                        finalPaths.push_back(coordinate3d);
//                    }
//                }
//                else
//                {
//                    for(auto sol : solutionN)
//                    {
//                        updateGridSet.insert(sol);
//                        auto coordinate3d = translate1D_3D(sol);
//                        finalPaths.push_back(coordinate3d);
//                    }
//                }
//            }
//            else if( relativeDirection == NE )
//            {
//                
//                Graph_SP * graph_sp_E = InitGraph_SP(sbox,RIGHT );
//                graph_sp_E->Dijkstra(powerCandidate);
//                auto solutionE= graph_sp_E->getPath(blockPinCandidate);
//                delete [] graph_sp_E ;
//                Graph_SP * graph_sp_N = InitGraph_SP(sbox, TOP);
//                graph_sp_N->Dijkstra(powerCandidate);
//                auto solutionN= graph_sp_N->getPath(blockPinCandidate);
//                delete [] graph_sp_N ;
//                cout << getTurnCount(solutionE) << endl;
//                cout << getTurnCount(solutionN) << endl;
//                if( getTurnCount(solutionE) < getTurnCount(solutionN) )
//                {
//                    for(auto sol : solutionE)
//                    {
//                        updateGridSet.insert(sol);
//                        auto coordinate3d = translate1D_3D(sol);
//                        finalPaths.push_back(coordinate3d);
//                    }
//                }
//                else
//                {
//                    for(auto sol : solutionN)
//                    {
//                        updateGridSet.insert(sol);
//                        auto coordinate3d = translate1D_3D(sol);
//                        finalPaths.push_back(coordinate3d);
//                    }
//                }
//            }
//            else if( relativeDirection == SW )
//            {
//                Graph_SP * graph_sp_W = InitGraph_SP(sbox,LEFT );
//                graph_sp_W->Dijkstra(powerCandidate);
//                auto solutionW= graph_sp_W->getPath(blockPinCandidate);
//                delete [] graph_sp_W ;
//                Graph_SP * graph_sp_S = InitGraph_SP(sbox, DOWN);
//                graph_sp_S->Dijkstra(powerCandidate);
//                auto solutionS= graph_sp_S->getPath(blockPinCandidate);
//                delete [] graph_sp_S ;
//                if( getTurnCount(solutionW) < getTurnCount(solutionS) )
//                {
//                    for(auto sol : solutionW)
//                    {
//                        updateGridSet.insert(sol);
//                        auto coordinate3d = translate1D_3D(sol);
//                        finalPaths.push_back(coordinate3d);
//                    }
//                }
//                else
//                {
//                    for(auto sol : solutionS)
//                    {
//                        updateGridSet.insert(sol);
//                        auto coordinate3d = translate1D_3D(sol);
//                        finalPaths.push_back(coordinate3d);
//                    }
//                }
//            }
//            else if( relativeDirection == SE )
//            {
//                Graph_SP * graph_sp_E = InitGraph_SP(sbox,RIGHT );
//                graph_sp_E->Dijkstra(powerCandidate);
//                auto solutionE= graph_sp_E->getPath(blockPinCandidate);
//                delete [] graph_sp_E ;
//                Graph_SP * graph_sp_S = InitGraph_SP(sbox, DOWN);
//                graph_sp_S->Dijkstra(powerCandidate);
//                auto solutionS= graph_sp_S->getPath(blockPinCandidate);
//                delete [] graph_sp_S ;
//                if( getTurnCount(solutionE) < getTurnCount(solutionS) )
//                {
//                    for(auto sol : solutionE)
//                    {
//                        updateGridSet.insert(sol);
//                        auto coordinate3d = translate1D_3D(sol);
//                        finalPaths.push_back(coordinate3d);
//                    }
//                }
//                else
//                {
//                    for(auto sol : solutionS)
//                    {
//                        updateGridSet.insert(sol);
//                        auto coordinate3d = translate1D_3D(sol);
//                        finalPaths.push_back(coordinate3d);
//                    }
//                }
//            }
//            targetPinNames.push_back(blockinfo);
//            fillSpNetMaps(finalPaths, it->first, targetPinNames);
//        }
//        // Single Source Multiple Target
//        else
//        {
//            auto ret = Connection.equal_range(it->first);
//            for (auto i = ret.first; i != ret.second; ++i)
//            {
//                cout << i->first << " " << i->second.BlockName << " " << i->second.BlockPinName << endl;
//                Block powerPinCoordinate = RouterHelper.getPowerPinCoordinate(it->first);
//                Block BlockPinCoordinate = RouterHelper.getBlock(i->second.BlockName, i->second.BlockPinName);
//                
//            }
//            
//            
//        }
//        // Update Grids
//        vector<int> updateIndex ;
//        updateIndex.assign(updateGridSet.begin(), updateGridSet.end());
//        updateGrids(updateIndex);
//        // free graph_sp
//        
//    }
    
    clock_t End = clock();
    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
    cout << "Routing Done" << endl ;
    cout << "We cost " << duration << "(s)" << endl;
    
    
}
void Router::updateGrids(vector<int> paths)
{
    for( int i = 0 ; i < paths.size(); i++ )
    {
        auto coordinate3d = translate1D_3D(paths[i]);
        int x = coordinate3d.x ;
        int y = coordinate3d.y ;
        int z = coordinate3d.z ;
        bool isLeftEdgeLegal = false , isRightEdgeLegal = false, isUpEdgeLegal = false , isDownEdgeLegal = false ;
        isLeftEdgeLegal = (x - 1 < 0) ? false : true ;
        isRightEdgeLegal = (x + 1 > Grids[0].size()) ? false : true ;
        isUpEdgeLegal = (y + 1 > Grids.size()) ? false : true ;
        isDownEdgeLegal = (y - 1 < 0) ? false : true ;
        if( isLeftEdgeLegal )
        {
            Grids[y][x].Edges[z].leftEdge = false ;
            Grids[y][x-1].Edges[z].rightEdge = false ;
        }
        if(isRightEdgeLegal)
        {
            Grids[y][x].Edges[z].rightEdge = false ;
            Grids[y][x+1].Edges[z].leftEdge = false ;
        }
        if( isUpEdgeLegal )
        {
            Grids[y][x].Edges[z].upEdge = false ;
            Grids[y+1][x].Edges[z].downEdge = false ;
        }
        if(isDownEdgeLegal)
        {
            Grids[y][x].Edges[z].downEdge = false ;
            Grids[y-1][x].Edges[z].upEdge = false ;
        }
    }
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
    cout << endl;
    return paths;
}
int Router::getTurnCount(vector<int> paths)
{
    vector<Coordinate3D> path3D ;
    for(auto p : paths)
        path3D.push_back(translate1D_3D(p));
    
    auto friendlyForm = translateToFriendlyForm(path3D);
    int turnCount = 0 ;
    for( auto f : friendlyForm )
    {
        if(f.first == topOrient || f.first == bottomOrient ) continue ;
        turnCount++ ;
    }
    return turnCount ;
}
void Router::fillSpNetMaps( vector<Coordinate3D> & paths , string powerPinName , BlockInfo blockinfo )
{
    // find
    auto iter = SpecialNetsMaps.find(powerPinName) ;
    bool create = ( iter != SpecialNetsMaps.end() ) ? false : true ;
    SpecialNets specialnet ;
    Nets net ;
    // Fill SOURCENAME
    specialnet.SOURCENAME = powerPinName ;
    // Fill DESTINATIONMAPS
    

    
    if( create )
        specialnet.DESTINATIONMAPS.insert(make_pair(blockinfo.BlockName, blockinfo.BlockPinName));
    else
        iter->second.DESTINATIONMAPS.insert(make_pair(blockinfo.BlockName, blockinfo.BlockPinName));
    specialnet.USE = "POWER";
    
    
    auto source = paths[0];
    int startLayer = source.z ;
    int layer = startLayer ;
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
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = DEFAULT_WIDTH * UNITS_DISTANCE ;
            net.SHAPE = "STRIPE";
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            
            cout << layer << " " << startPoint << " " << targetPoint << endl;
        }
        else if( diection == downOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x , oringinTargetPoint.y - (progress * DEFAULT_PITCH * UNITS_DISTANCE));
            targetPoint = oringinTargetPoint ;
            targetPoint.y -= (DEFAULT_WIDTH * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = DEFAULT_WIDTH * UNITS_DISTANCE ;
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            net.SHAPE = "STRIPE";
            cout << layer << " " << startPoint << " " << targetPoint << endl;
        }
        else if( diection == leftOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x - (progress * DEFAULT_PITCH * UNITS_DISTANCE), oringinTargetPoint.y );
            targetPoint = oringinTargetPoint ;
            targetPoint.x -= (DEFAULT_WIDTH * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = DEFAULT_WIDTH * UNITS_DISTANCE ;
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            net.SHAPE = "STRIPE";
            cout << layer << " " << startPoint << " " << targetPoint << endl;
        }
        else if( diection == rightOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x + (progress * DEFAULT_PITCH * UNITS_DISTANCE), oringinTargetPoint.y );
            targetPoint = oringinTargetPoint ;
            targetPoint.x += (DEFAULT_WIDTH * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = DEFAULT_WIDTH * UNITS_DISTANCE ;
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            net.SHAPE = "STRIPE";
            cout << layer << " " << startPoint << " " << targetPoint << endl;
        }
        else if( diection == topOrient )
        {
            for( int i = 0 ; i < progress ; i++ )
            {
                net.METALNAME = RouterHelper.translateIntToMetalName(layer);
                net.ROUNTWIDTH = 0 ;
                net.SHAPE = "STRIPE";
                net.ABSOLUTE_POINT1 = oringinTargetPoint;
                if( layer == 1 ) net.VIANAME = "via1_C";
                if( layer == 2 ) net.VIANAME = "via2_C";
                if( layer == 3 ) net.VIANAME = "via3_C";
                if( layer == 4 ) net.VIANAME = "via4_C";
                if( layer == 5 ) net.VIANAME = "via5_C";
                cout << layer << " " << oringinTargetPoint << " " ;
                if( layer == 1 ) cout << "via1_C" << endl;
                if( layer == 2 ) cout << "via2_C" << endl;
                if( layer == 3 ) cout << "via3_C" << endl;
                if( layer == 4 ) cout << "via4_C" << endl;
                if( layer == 5 ) cout << "via5_C" << endl;
                layer++ ;
            }
            
        }
        else if( diection == bottomOrient )
        {
            for( int i = 0 ; i < progress ; i++ )
            {
                net.METALNAME = RouterHelper.translateIntToMetalName(layer);
                net.ROUNTWIDTH = 0 ;
                net.SHAPE = "STRIPE";
                net.ABSOLUTE_POINT1 = oringinTargetPoint;
                if( layer == 6 ) net.VIANAME = "via5_C";
                if( layer == 5 ) net.VIANAME = "via4_C";
                if( layer == 4 ) net.VIANAME = "via3_C";
                if( layer == 3 ) net.VIANAME = "via2_C";
                if( layer == 2 ) net.VIANAME = "via1_C";
                // hard code via5c 而且 第六層走到第五層要用查的，目前hardcode
                cout << layer << " " << oringinTargetPoint << " " ;
                
                if( layer == 6 ) cout << "via5_C" << endl;
                if( layer == 5 ) cout << "via4_C" << endl;
                if( layer == 4 ) cout << "via3_C" << endl;
                if( layer == 3 ) cout << "via2_C" << endl;
                if( layer == 2 ) cout << "via1_C" << endl;
                layer--;
            }
            
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
        if( create )
            specialnet.NETSMULTIMAPS.insert(make_pair(net.METALNAME, net));
        else
            iter->second.NETSMULTIMAPS.insert(make_pair(net.METALNAME, net));
        
    }
    fixSource();
    fixTarget();
    SpecialNetsMaps.insert(make_pair(powerPinName, specialnet));
}
void Router::fixSource()
{
    
}
void Router::fixTarget()
{
    
}
void Router::InitBoundList()
{
    int YSize = (int)Grids.size() ;
    int XSize = (int)Grids[0].size() ;
    int Up = XSize ;
    int Right = 1 ;
    int Top =  ( XSize - 1 ) * Right + ( YSize - 1 ) * Up + 1;
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
}
Graph_SP *  Router::InitGraph_SP( SBox sbox , DIRECTION preferDirection  )
{
    cout << "Begin Initialize 3D Shortest Path Graph ..." << endl;
    clock_t Start = clock();
    Graph_SP * graph_sp = new Graph_SP[1];
    
    int zLowerBound = sbox.lowestMetalLayer  ;
    int zUpperBound = sbox.highestMetalLayer + 1 ;
    int xLowerBound = sbox.LeftDown.x ;
    int xUpperBound = sbox.RightUp.x + 1 ;
    int yLowerBound = sbox.LeftDown.y ;
    int yUpperBound = sbox.RightUp.y + 1 ;
    int YSize = (int)Grids.size() ;
    int XSize = (int)Grids[0].size() ;
    int Right = 1 ;
    int Left = -1 * Right ;
    int Up = XSize ;
    int Down = -1 * Up ;
    int Top =  ( XSize - 1 ) * Right + ( YSize - 1 ) * Up + 1;
    int Bottom = -1 * Top ;
    graph_sp->resize(XSize * YSize * highest);
    
    for( int z = zLowerBound ; z < zUpperBound ; z++ )
    {
        for( int y = yLowerBound ; y < yUpperBound ; y++)
        {
            for( int x = xLowerBound ; x < xUpperBound ; x++ )
            {
                int index = translate3D_1D(Coordinate3D(x,y,z));
                int weights = RouterHelper.getWeights(z);
                // not boundary list
                if( boundList.find(index) == boundList.end() )
                {
                    // 因為不是boundary 所以不用check會不會超出index
                    // right
                    if( Grids[y][x].Edges[z].rightEdge )
                    {
                        if( preferDirection == RIGHT )
                        {
                            graph_sp->AddEdge(index, index+Right, weights-1);
                            graph_sp->AddEdge(index+Right, index, weights-1);
                        }
                        else
                        {
                            graph_sp->AddEdge(index, index+Right, weights);
                            graph_sp->AddEdge(index+Right, index, weights);
                        }
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Right, Max_Distance);
                        graph_sp->AddEdge(index+Right, index, Max_Distance);
                    }
                    // left
                    if( Grids[y][x-1].Edges[z].rightEdge )
                    {
                        if( preferDirection == LEFT )
                        {
                            graph_sp->AddEdge(index, index+Left, weights-1);
                            graph_sp->AddEdge(index+Left, index, weights-1);
                        }
                        else
                        {
                            graph_sp->AddEdge(index, index+Left, weights);
                            graph_sp->AddEdge(index+Left, index, weights);
                        }
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Left, Max_Distance);
                        graph_sp->AddEdge(index+Left, index, Max_Distance);
                    }
                    // up
                    if( Grids[y][x].Edges[z].upEdge )
                    {
                        if( preferDirection == TOP )
                        {
                            graph_sp->AddEdge(index, index+Up, weights-1);
                            graph_sp->AddEdge(index+Up, index, weights-1);
                        }
                        else
                        {
                            graph_sp->AddEdge(index, index+Up, weights);
                            graph_sp->AddEdge(index+Up, index, weights);
                        }
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Up, Max_Distance);
                        graph_sp->AddEdge(index+Up, index, Max_Distance);
                    }
                    // down
                    if( Grids[y-1][x].Edges[z].upEdge )
                    {
                        if( preferDirection == DOWN )
                        {
                            graph_sp->AddEdge(index, index+Down, weights-1);
                            graph_sp->AddEdge(index+Down, index, weights-1);
                        }
                        else
                        {
                            graph_sp->AddEdge(index, index+Down, weights);
                            graph_sp->AddEdge(index+Down, index, weights);
                        }
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Down, Max_Distance);
                        graph_sp->AddEdge(index+Down, index, Max_Distance);
                    }
                    // VIA WEIGHT 目前假設1
                    if( z == lowest )
                    {
                        graph_sp->AddEdge(index, index+Top, 1);
                    }
                    else if (z == highest)
                    {
                        graph_sp->AddEdge(index, index+Bottom, 1);
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Top, 1);
                        graph_sp->AddEdge(index, index+Bottom, 1);
                    }
                }
                for( auto sp : SpecialPoints )
                {
                    if(sp.second.x == x && sp.second.y == y)
                    {
                        for( int z = lowest ; z <= highest ; z++ )
                        {
                            int index = translate3D_1D(Coordinate3D(x,y,z));
                            if( Grids[y][x].Edges[z].rightEdge ) graph_sp->UpdateWeight(index+Right, index, Max_Distance - index);
                            if( Grids[y][x].Edges[z].upEdge ) graph_sp->UpdateWeight(index+Up, index, Max_Distance - index);
                            if( Grids[y][x-1].Edges[z].rightEdge ) graph_sp->UpdateWeight(index+Left, index, Max_Distance - index);
                            if( Grids[y-1][x].Edges[z].upEdge ) graph_sp->UpdateWeight(index+Down, index, Max_Distance - index);
                        }
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
    return graph_sp ; 
}
void Router::CutByUserDefine()
{
    int x_diff = DIEAREA.pt2.x - DIEAREA.pt1.x ;
    int y_diff = DIEAREA.pt2.y - DIEAREA.pt1.y ;
    
    int default_length = DEFAULT_PITCH * UNITS_DISTANCE ;
    //int default_length = 100 * UNITS_DISTANCE ;
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
void Router::CutGrid()
{
    auto powerInfos = RouterHelper.getPowerPinInfo() ;
    auto blockPinInfos = RouterHelper.getBlockPinInfo() ;
    
    for(auto powerinfo : powerInfos)
        cout << powerinfo.LeftDown << " " << powerinfo.RightUp << endl;
    cout << endl; 
    for(auto blockpininfo : blockPinInfos)
        cout << blockpininfo.LeftDown << " " << blockpininfo.RightUp << endl;
    
    cout << " ";
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
    
//    int x_diff = DIEAREA.pt2.x - DIEAREA.pt1.x ;
//    int y_diff = DIEAREA.pt2.y - DIEAREA.pt1.y ;
//    
//    int default_length = DEFAULT_PITCH * UNITS_DISTANCE ;
//    //int default_length = 100 * UNITS_DISTANCE ;
//    int scaling_length = default_length  ;
//    
//    // Set Vertical Line
//    for( int i = scaling_length ; i <= x_diff ; i += scaling_length )
//        Vertical.insert(i);
//    
//    
//    // Set Horizontal Line
//    for( int i = scaling_length ; i <= y_diff ; i += scaling_length )
//        Horizontal.insert(i);
//    
//    // DIEAREA is not align .
//    if( *Vertical.end() != DIEAREA.pt2.x ) Vertical.insert(DIEAREA.pt2.x);
//    if( *Horizontal.end() != DIEAREA.pt2.y ) Horizontal.insert(DIEAREA.pt2.y);
}
void Router::InitGrids()
{
    cout << "Begin Initialize  Grid Graph ..." << endl;
    clock_t Start = clock();
    //    CutByBlockBoundary();
//    CutByUserDefine();
    CutGrid();
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
    //printAllGrids();
    
}
