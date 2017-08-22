//
//  RouterV4.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 05/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "RouterV4.hpp"

// bug list:
// 溝通要用絕對座標
// 優化何時要問ngspice
// source target selection
// spice 打錯 (應該是失真問題)




RouterV4::RouterV4(string spice, string def , string output)
{
    spiceName = spice ;
    defName = def ;
    outputfilesName = output ;
    def_gen.setDefName(def);
    sp_gen.setSpiceName(spice);
    InitState();
    
}
RouterV4::~RouterV4()
{
    
}

void RouterV4::InitState()
{
    // Init lowest and highest
    vector<string> metals ;
    for( auto x : LayerMaps )
    {
        if( x.second.TYPE == "ROUTING" )
            metals.push_back(x.first);
    }
    lowestMetal = stoi(metals[0].substr(5));
    highestMetal = stoi(metals[metals.size()-1].substr(5));
    SpecialNetsMaps.clear();
}
int RouterV4::translate3D_1D(Coordinate3D coordinate3d)
{
    int YSize = (int)Grids.size()+1;
    int XSize = (int)Grids[0].size()+1;
    int total = (XSize)*(YSize);
    int x = coordinate3d.x ;
    int y = coordinate3d.y ;
    int z = coordinate3d.z ;
    return (z-1)*total + x + y * XSize ;
}
Coordinate3D RouterV4::translate1D_3D(int index)
{
    int YSize = (int)Grids.size()+1;
    int XSize = (int)Grids[0].size()+1;
    int total = XSize*YSize;
    int z = index / total ;
    int coordinate2d = index % total ;
    int x = coordinate2d % XSize ;
    int y = coordinate2d / XSize ;
    return Coordinate3D(x,y,z+1);
}
void RouterV4::InitBoundList()
{
    boundList.clear();
    int YSize = (int)Grids.size()+1 ;
    int XSize = (int)Grids[0].size()+1;
    //( XSize - 1 ) * Right + ( YSize - 1 ) * Up + 1;
    for( int z = lowestMetal ; z <= highestMetal ; z++)
    {
        for( int x = 0 ; x < XSize ; x++ )
        {
            int downBoundary = translate3D_1D(Coordinate3D(x,0,z));
            boundList.insert(downBoundary);
            int upBoundary = translate3D_1D(Coordinate3D(x,(int)Grids.size(),z));
            boundList.insert(upBoundary);
        }
        for( int y = 0 ; y < YSize ; y++ )
        {
            int leftBoundary = translate3D_1D(Coordinate3D(0,y,z));
            boundList.insert(leftBoundary);
            int rightBoundary = translate3D_1D(Coordinate3D((int)Grids[0].size(),y,z));
            boundList.insert(rightBoundary);
        }
    }
}
Graph_SP * RouterV4::InitGraph_SP(double width , double spacing )
{
//    cout << "Begin Initialize 3D Shortest Path Graph ..." << endl;
//    clock_t Start = clock();
    InitBoundList();
    Graph_SP * graph_sp = new Graph_SP[1];
    int YSize = (int)Grids.size()+1 ;
    int XSize = (int)Grids[0].size()+1;
    int Up = XSize ;
    int Right = 1 ;
    int Top = (XSize) * (YSize);
    int Left = -1 * Right ;
    int Down = -1 * Up ;
    int Bottom = -1 * Top ;
    
    graph_sp->resize(XSize * YSize * highestMetal);
   
    
    for( int z = lowestMetal ; z <= highestMetal ; z++ )
    {
        for( int y = 0 ; y < YSize ; y++ )
        {
            for( int x = 0 ; x < XSize ; x++ )
            {
                
                int index = translate3D_1D(Coordinate3D(x,y,z));
                if( boundList.find(index) == boundList.end() )
                {
                    if( Grids[y][x].Edges[z].downEdge )
                    {
                        graph_sp->AddEdge(index, index+Right, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x].width) * 10000  );
                        graph_sp->AddEdge(index+Right, index, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x].width) * 10000 ) ;
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Right, Max_Distance);
                        graph_sp->AddEdge(index+Right, index, Max_Distance);
                    }
                    if( Grids[y][x-1].Edges[z].downEdge )
                    {
                        
                        graph_sp->AddEdge(index, index+Left, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x-1].width)* 10000 );
                        graph_sp->AddEdge(index+Left, index, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x-1].width)* 10000 );
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Left, Max_Distance);
                        graph_sp->AddEdge(index+Left, index, Max_Distance);
                    }
                    if( Grids[y][x].Edges[z].leftEdge )
                    {
                        graph_sp->AddEdge(index, index+Up, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x].length) * 10000);
                        graph_sp->AddEdge(index+Up, index, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x].length) * 10000);
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Up, Max_Distance);
                        graph_sp->AddEdge(index+Up, index, Max_Distance);
                    }
                    if( Grids[y-1][x].Edges[z].leftEdge )
                    {
                        
                        graph_sp->AddEdge(index, index+Down, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y-1][x].length)* 10000 );
                        graph_sp->AddEdge(index+Down, index, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y-1][x].length) * 10000);
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Down, Max_Distance);
                        graph_sp->AddEdge(index+Down, index, Max_Distance);
                    }
                    
                    
                    if( z == lowestMetal)
                    {
                        int viaWeight = RouterHelper.getViaWeight(width * width , z ) ;
                        
                        if( Grids[y][x].verticalEdges[z].topEdge )
                        {
                            graph_sp->AddEdge(index, index+Top, viaWeight);
                            graph_sp->AddEdge(index+Top, index, viaWeight);
                        }
                        else
                        {
                            graph_sp->AddEdge(index, index+Top, Max_Distance);
                            graph_sp->AddEdge(index+Top, index, Max_Distance);
                        }
                    }
                    else if (z == highestMetal)
                    {
                        int viaWeight = RouterHelper.getViaWeight(width * width , z-1 ) ;
                        
                        if( Grids[y][x].verticalEdges[z].bottomEdge )
                        {
                            graph_sp->AddEdge(index, index+Bottom, viaWeight);
                            graph_sp->AddEdge(index+Bottom, index, viaWeight);
                        }
                        else
                        {
                            graph_sp->AddEdge(index, index+Bottom, Max_Distance);
                            graph_sp->AddEdge(index+Bottom, index, Max_Distance);
                        }
                    }
                    else
                    {
                        int viaWeight = RouterHelper.getViaWeight(width * width , z ) ;
                        
                        if( Grids[y][x].verticalEdges[z].topEdge )
                        {
                            graph_sp->AddEdge(index, index+Top, viaWeight);
                            graph_sp->AddEdge(index+Top, index, viaWeight);
                        }
                        else
                        {
                            graph_sp->AddEdge(index, index+Top, Max_Distance);
                            graph_sp->AddEdge(index+Top, index, Max_Distance);
                        }
                        viaWeight = RouterHelper.getViaWeight(width * width , z -1) ;
                        
                        if( Grids[y][x].verticalEdges[z].bottomEdge )
                        {
                            graph_sp->AddEdge(index, index+Bottom, viaWeight);
                            graph_sp->AddEdge(index+Bottom, index, viaWeight);
                        }
                        else
                        {
                            graph_sp->AddEdge(index, index+Bottom, Max_Distance);
                            graph_sp->AddEdge(index+Bottom, index, Max_Distance);
                        }
                    }
                }
            }
        }
    }
    clock_t End = clock();
//    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
    //    printAllGrids();
//    cout << "3D Shortest Path Graph Done" << endl ;
//    cout << "We cost " << duration << "(s)" << endl;
    return graph_sp ;
}
Point<int> RouterV4::getAbsolutePoint( Coordinate3D coordinate3d )
{
    int x = coordinate3d.x ;
    int y = coordinate3d.y ;
    int X = ( x != 0 ) ? *std::next(Vertical.begin(), x-1) : 0 ;
    int Y = ( y != 0 ) ? *std::next(Horizontal.begin(), y-1) : 0 ;
    return Point<int>(X,Y);
}
Coordinate3D RouterV4::LegalizeTargetEdge(Block block ,Graph_SP * graph_sp , double width , double spacing )
{
    vector<Coordinate3D> paths ;
    auto sourceGrid = getGridCoordinate(block);
    Coordinate3D targetGrid = sourceGrid ;
    if( block.Direction == TOP )
    {
        int blockLength = block.RightUp.y - block.LeftDown.y ;
        
        int currentY = getAbsolutePoint(sourceGrid).y ;
        paths.push_back(sourceGrid);
        targetGrid.y += 1 ;
        int nextY = getAbsolutePoint(targetGrid).y ;
        paths.push_back(targetGrid);
        // get how many counts should be calcaulated.
        while( nextY - currentY !=  (0.5 * width + spacing)*UNITS_DISTANCE + blockLength )
        {
            targetGrid.y += 1 ;
            paths.push_back(targetGrid);
            nextY = getAbsolutePoint(targetGrid).y ;
        }
        for(int i = 0 ; i < paths.size() - 1 ; i++)
        {
            int current = translate3D_1D(paths[i]);
            int next = translate3D_1D(paths[i+1]);
            graph_sp->UpdateWeight(next, current, Grids[paths[i].y][paths[i].x].length);
        }
    }
    else if(block.Direction == DOWN)
    {
        int blockLength = block.RightUp.y - block.LeftDown.y ;
        int currentY = getAbsolutePoint(sourceGrid).y ;
        paths.push_back(sourceGrid);
        targetGrid.y -= 1 ;
        paths.push_back(targetGrid);
        int nextY = getAbsolutePoint(targetGrid).y ;
        // get how many counts should be calcaulated.
        while( currentY - nextY !=  (0.5 * width + spacing)*UNITS_DISTANCE + blockLength )
        {
            targetGrid.y -= 1 ;
            paths.push_back(targetGrid);
            nextY = getAbsolutePoint(targetGrid).y ;
        }
        for(int i = 0 ; i < paths.size() - 1 ; i++)
        {
            int current = translate3D_1D(paths[i]);
            int next = translate3D_1D(paths[i+1]);
            graph_sp->UpdateWeight(next, current, Grids[paths[i].y-1][paths[i].x].length);
        }
    }
    else if(block.Direction == RIGHT )
    {
        int blockWidth = block.RightUp.x - block.LeftDown.x ;
        int currentX = getAbsolutePoint(sourceGrid).x ;
        paths.push_back(sourceGrid);
        targetGrid.x += 1 ;
        paths.push_back(targetGrid);
        int nextX = getAbsolutePoint(targetGrid).x ;
        // get how many counts should be calcaulated.
        while( nextX - currentX !=  (0.5 * width + spacing)*UNITS_DISTANCE + blockWidth )
        {
            targetGrid.x += 1 ;
            paths.push_back(targetGrid);
            nextX = getAbsolutePoint(targetGrid).x ;
        }
        for(int i = 0 ; i < paths.size() - 1 ; i++)
        {
            int current = translate3D_1D(paths[i]);
            int next = translate3D_1D(paths[i+1]);
            graph_sp->UpdateWeight(next, current, Grids[paths[i].y][paths[i].x].width);
        }
        
    }
    else if(block.Direction == LEFT)
    {
        int blockWidth = block.RightUp.x - block.LeftDown.x ;
        paths.push_back(sourceGrid);
        int currentX = getAbsolutePoint(sourceGrid).x ;
        targetGrid.x -= 1 ;
        paths.push_back(targetGrid);
        int nextX = getAbsolutePoint(targetGrid).x ;
        // get how many counts should be calcaulated.
        while( currentX - nextX !=  (0.5 * width + spacing)*UNITS_DISTANCE + blockWidth )
        {
            targetGrid.x -= 1 ;
            paths.push_back(targetGrid);
            nextX = getAbsolutePoint(targetGrid).x ;
        }
        for(int i = 0 ; i < paths.size() - 1 ; i++)
        {
            int current = translate3D_1D(paths[i]);
            int next = translate3D_1D(paths[i+1]);
            graph_sp->UpdateWeight(next, current, Grids[paths[i].y][paths[i].x-1].width);
        }
    }
    return targetGrid ;
}
pair<string,vector<Point<int>>>  RouterV4::getViaLocation(Nets & net , Point<int> & orginTarget , bool top , double width)
{
    ViaInfo viaInfo ;
    vector<Point<int>> viaLocation ;
    double minResistace = INT_MAX ;
    int crossArea = width * width ;
    Point<int> LeftUp = orginTarget ;
    Point<int> RightDown = orginTarget ;
    LeftUp.x -= (width * UNITS_DISTANCE / 2 );
    LeftUp.y += (width * UNITS_DISTANCE / 2 );
    RightDown.x += (width * UNITS_DISTANCE / 2 );
    RightDown.y -= (width * UNITS_DISTANCE / 2 );
    string key = ( top ) ? RouterHelper.translateIntToMetalName( RouterHelper.translateMetalNameToInt(net.METALNAME) - 1 ): net.METALNAME ;
    // select best via
    for( auto via : RouterHelper.ViaInfos[key] )
    {
        int width = via.width;
        int length = via.length ;
        int area = width * length ;
        int cnt = crossArea / area ;
        double resistance = via.resistance / cnt ;
        if( minResistace > resistance )
        {
            minResistace = resistance ;
            viaInfo.name = via.name ;
            viaInfo.width = via.width ;
            viaInfo.length = via.length;
        }
    }
    int X = LeftUp.x + (viaInfo.width * UNITS_DISTANCE / 2) ;
    int Y = LeftUp.y - (viaInfo.length * UNITS_DISTANCE / 2) ;
    Point<int> first(X,Y);
    X = RightDown.x - (viaInfo.width * UNITS_DISTANCE / 2) ;
    Y = RightDown.y + (viaInfo.length * UNITS_DISTANCE / 2 ) ;
    Point<int> last(X,Y);
    if( first == last ) viaLocation.push_back(first);
    for( int x = first.x ; x <= last.x ; x += (viaInfo.width * UNITS_DISTANCE ) )
    {
        for( int y = first.y ; y >= last.y ; y -= (viaInfo.length * UNITS_DISTANCE) )
        {
            viaLocation.push_back(Point<int>(x,y));
        }
    }
    return make_pair(viaInfo.name, viaLocation);
}
void RouterV4::fillSpNetMaps( vector<Coordinate3D> & paths , string powerPinName , string blockName , string blockPinName , double width ,  bool peek )
{
//    for(auto p : paths)
//        cout << p.x << " " << p.y << " " << p.z << endl;
//    cout << "--------------------------------" << endl;
    // find
    auto iter = SpecialNetsMaps.find(powerPinName) ;
    bool create = ( iter != SpecialNetsMaps.end() ) ? false : true ;
    SpecialNets specialnet ;
    Nets net ;
    // Fill SOURCENAME
    specialnet.SOURCENAME = powerPinName ;
    // Fill DESTINATIONMAPS
    
    
    
    if( create )
        specialnet.DESTINATIONMAPS.insert(make_pair(blockName, blockPinName));
    else
        iter->second.DESTINATIONMAPS.insert(make_pair(blockName, blockPinName));
    specialnet.USE = "POWER";
    
    
    auto source = paths[0];
    int startLayer = source.z ;
    int layer = startLayer ;
//    Graph g ;
//    for(auto p : paths)
//    {
//        auto abs = gridToAbsolute(p);
//        widthTable.insert(make_pair(g.encode(abs), width));
//    }
    auto friendlyForm = translateToFriendlyForm(paths);
    
    Point<int> startPoint = getAbsolutePoint(source);
    Point<int> oringinTargetPoint = startPoint;
    Point<int> targetPoint = startPoint;
    Direction3D nextDirection = friendlyForm[0].first ;
    if(peek)
    {
        if( nextDirection == upOrient )
        {
            startPoint.y -= (width * UNITS_DISTANCE / 2) ;
        }
        else if (nextDirection == downOrient)
        {
            startPoint.y += (width * UNITS_DISTANCE / 2) ;
        }
        else if (nextDirection == leftOrient)
        {
            startPoint.x += (width * UNITS_DISTANCE / 2) ;
        }
        else if (nextDirection == rightOrient)
        {
            startPoint.x -= (width * UNITS_DISTANCE / 2) ;
        }
    }
    
    
//    for( auto p : friendlyForm )
//    {
//        if( p.first == 0 )  cout << "UP";
//        if( p.first == 1 )  cout << "DOWN";
//        if( p.first == 2 )  cout << "LEFT";
//        if( p.first == 3 )  cout << "RIGHT";
//        if( p.first == 4 )  cout << "TOP";
//        if( p.first == 5 )  cout << "BOTTOM";
//        cout << " " << p.second << endl;
//    }
//    cout << endl;
    
    auto Distance = getAbsoluteDistance(friendlyForm, Point<int>(source.x,source.y));
//    for(auto d : Distance)
//        cout << d << endl;
    for( int i = 0 ; i < friendlyForm.size() ; i++ )
    {
        vector<Nets> nets;
        Direction3D diection = friendlyForm[i].first ;
        Direction3D nextDirection = friendlyForm[i+1].first ;
        int progress = friendlyForm[i].second ;
        if( diection == upOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x , oringinTargetPoint.y + Distance[i]);
            targetPoint = oringinTargetPoint ;
//            if( i != friendlyForm.size() -1 )targetPoint.y += (WIDTH * UNITS_DISTANCE / 2) ;
            targetPoint.y += (width * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = width * UNITS_DISTANCE ;
            net.SHAPE = "STRIPE";
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            nets.push_back(net);
            BlockCoordinate blockCoordinate ;
            blockCoordinate.LeftDown = startPoint ;
            blockCoordinate.LeftDown.x -= net.ROUNTWIDTH / 2 ;
            blockCoordinate.RightUp = targetPoint ;
            blockCoordinate.RightUp.x += net.ROUNTWIDTH / 2 ;
            blockCoordinate.lowerMetal = layer ;
            blockCoordinate.upperMetal = layer ;
            if( obstacles.find(powerPinName) == obstacles.end() ) obstacles.insert(make_pair(powerPinName, vector<BlockCoordinate>()));
            obstacles[powerPinName].push_back(blockCoordinate);
//            cout << layer << " " << startPoint << " " << targetPoint << endl;
        }
        else if( diection == downOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x , oringinTargetPoint.y - Distance[i]);
            targetPoint = oringinTargetPoint ;
//            if( i != friendlyForm.size() -1 )targetPoint.y -= (WIDTH * UNITS_DISTANCE / 2) ;
            targetPoint.y -= (width * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = width * UNITS_DISTANCE ;
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            net.SHAPE = "STRIPE";
            nets.push_back(net);
            BlockCoordinate blockCoordinate ;
            blockCoordinate.RightUp = startPoint ;
            blockCoordinate.RightUp.x += net.ROUNTWIDTH / 2 ;
            blockCoordinate.LeftDown = targetPoint ;
            blockCoordinate.LeftDown.x -= net.ROUNTWIDTH / 2 ;
            blockCoordinate.lowerMetal = layer ;
            blockCoordinate.upperMetal = layer ;
            if( obstacles.find(powerPinName) == obstacles.end() ) obstacles.insert(make_pair(powerPinName, vector<BlockCoordinate>()));
            obstacles[powerPinName].push_back(blockCoordinate);
//            cout << layer << " " << startPoint << " " << targetPoint << endl;
        }
        else if( diection == leftOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x - Distance[i], oringinTargetPoint.y );
            targetPoint = oringinTargetPoint ;
//            if( i != friendlyForm.size() -1 ) targetPoint.x -= (WIDTH * UNITS_DISTANCE / 2) ;
            targetPoint.x -= (width * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = width * UNITS_DISTANCE ;
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            net.SHAPE = "STRIPE";
            nets.push_back(net);
            BlockCoordinate blockCoordinate ;
            blockCoordinate.RightUp = startPoint ;
            blockCoordinate.RightUp.y += net.ROUNTWIDTH / 2 ;
            blockCoordinate.LeftDown = targetPoint ;
            blockCoordinate.LeftDown.y -= net.ROUNTWIDTH / 2 ;
            blockCoordinate.lowerMetal = layer ;
            blockCoordinate.upperMetal = layer ;
            if( obstacles.find(powerPinName) == obstacles.end() ) obstacles.insert(make_pair(powerPinName, vector<BlockCoordinate>()));
            obstacles[powerPinName].push_back(blockCoordinate);
//            cout << layer << " " << startPoint << " " << targetPoint << endl;
        }
        else if( diection == rightOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x + Distance[i], oringinTargetPoint.y );
            targetPoint = oringinTargetPoint ;
//            if( i != friendlyForm.size() -1 )targetPoint.x += (WIDTH * UNITS_DISTANCE / 2) ;
            targetPoint.x += (width * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = width * UNITS_DISTANCE ;
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            net.SHAPE = "STRIPE";
            nets.push_back(net);
            BlockCoordinate blockCoordinate ;
            blockCoordinate.LeftDown = startPoint ;
            blockCoordinate.LeftDown.y -= net.ROUNTWIDTH / 2 ;
            blockCoordinate.RightUp = targetPoint ;
            blockCoordinate.RightUp.y += net.ROUNTWIDTH / 2 ;
            blockCoordinate.lowerMetal = layer ;
            blockCoordinate.upperMetal = layer ;
            if( obstacles.find(powerPinName) == obstacles.end() ) obstacles.insert(make_pair(powerPinName, vector<BlockCoordinate>()));
            obstacles[powerPinName].push_back(blockCoordinate);
//            cout << layer << " " << startPoint << " " << targetPoint << endl;
        }
        else if( diection == topOrient )
        {
            for( int i = 0 ; i < progress ; i++ )
            {
                net.METALNAME = RouterHelper.translateIntToMetalName(layer);
                net.ROUNTWIDTH = 0 ;
                net.SHAPE = "STRIPE";
                auto viaSols = getViaLocation(net,oringinTargetPoint , false , width);
                net.VIANAME = viaSols.first;
                for( auto sol : viaSols.second )
                {
                    net.ABSOLUTE_POINT1 = sol;
                    nets.push_back(net);
                }
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
                auto viaSols = getViaLocation(net,oringinTargetPoint , true ,width );
                net.VIANAME = viaSols.first;
                for( auto sol : viaSols.second )
                {
                    net.ABSOLUTE_POINT1 = sol;
                    nets.push_back(net);
                }
                layer--;
            }
            
        }
        //-------------------------------//
        //update source point
        startPoint = oringinTargetPoint ;
        if( nextDirection == upOrient )
        {
            startPoint.y -= width / 2 * UNITS_DISTANCE;
            
        }
        if( nextDirection == downOrient )
        {
            startPoint.y += width / 2 * UNITS_DISTANCE;
        }
        if( nextDirection == leftOrient )
        {
            startPoint.x += width / 2 * UNITS_DISTANCE;
        }
        if( nextDirection == rightOrient )
        {
            startPoint.x -= width / 2 * UNITS_DISTANCE;
        }
        if( create )
        {
            for(auto net : nets)
                specialnet.NETSMULTIMAPS.insert(make_pair(net.METALNAME, net));
        }
        else
        {
            for(auto net : nets)
                iter->second.NETSMULTIMAPS.insert(make_pair(net.METALNAME, net));
        }
        
    }
    
    SpecialNetsMaps.insert(make_pair(powerPinName, specialnet));
}
// x y are absolute coordinate 
string RouterV4::gridToString(Coordinate3D coordinate , bool translate )
{
    int x = (!translate) ? coordinate.x : getAbsolutePoint(coordinate).x;
    int y = (!translate) ? coordinate.y : getAbsolutePoint(coordinate).y;
    string metalName = RouterHelper.translateIntToMetalName(coordinate.z);
    string result = metalName;
    result.append("_").append(to_string(x)).append("_").append(to_string(y));
    return result ;
}
void RouterV4::getInitSolution(Block block  , string powerpin, string blockName , string BlockPinName , double width  , double spacing ,  bool source)
{
    // Virtual Obstacles 還沒存入multipinCandidate
    vector<BlockCoordinate> virtualObstacles ;
    vector<Coordinate3D> paths ;
    auto sourceGrid = getGridCoordinate(block);
    string key ;
    key.append(blockName).append(BlockPinName);
    if( powerpin == "VDD_111b" && blockName == "B6_05" )
        cout << "";
    if( block.Direction == TOP )
    {
        int blockLength = block.RightUp.y - block.LeftDown.y ;
        Coordinate3D targetGrid = sourceGrid ;
        int currentY = getAbsolutePoint(sourceGrid).y ;
        targetGrid.y += 1 ;
        int nextY = getAbsolutePoint(targetGrid).y ;
        // get how many counts should be calcaulated.
        while( nextY - currentY !=  (0.5 * width + spacing)*UNITS_DISTANCE + blockLength )
        {
            targetGrid.y += 1 ;
            nextY = getAbsolutePoint(targetGrid).y ;
        }
        for(int y = sourceGrid.y ; y <= targetGrid.y ; y++)
        {
            Coordinate3D solution ;
            solution.x = sourceGrid.x ;
            solution.y = y ;
            solution.z = sourceGrid.z ;
            paths.push_back(solution);
        }
        fillSpNetMaps(paths, powerpin, blockName , BlockPinName ,width ,  false );
        vector<Coordinate3D> absolutePoints ;
        for(auto p : paths)
        {
            auto pt = getAbsolutePoint(p);
            Coordinate3D coordinate3D(pt.x , pt.y , p.z);
            absolutePoints.push_back(coordinate3D);
        }
        if(source) sourceTargetInitPath.insert(make_pair(powerpin, absolutePoints));
        else sourceTargetInitPath.insert(make_pair(key, absolutePoints));
        
        for(int i = lowestMetal ; i <= highestMetal ; i++)
        {
            BlockCoordinate virtualObstacle ;
            int z = sourceGrid.z ;
            Point<int> Source = getAbsolutePoint(sourceGrid);
            Point<int> Target = getAbsolutePoint(targetGrid);
            virtualObstacle.LeftDown = Source;
            virtualObstacle.RightUp = Target;
            virtualObstacle.RightUp.y += (0.5 * DEFAULTWIDTH) * UNITS_DISTANCE ;
            virtualObstacle.LeftDown.x -= width / 2 * UNITS_DISTANCE ;
            virtualObstacle.RightUp.x += width / 2 * UNITS_DISTANCE ;
            if( i != z )
            {
                virtualObstacle.lowerMetal = i ;
                virtualObstacle.upperMetal = i ;
            }
            virtualObstacles.push_back(virtualObstacle);
        }
    }
    else if(block.Direction == DOWN)
    {
        int blockLength = block.RightUp.y - block.LeftDown.y ;
        Coordinate3D targetGrid = sourceGrid ;
        int currentY = getAbsolutePoint(sourceGrid).y ;
        targetGrid.y -= 1 ;
        int nextY = getAbsolutePoint(targetGrid).y ;
        // get how many counts should be calcaulated.
        while( currentY - nextY !=  (0.5 * width + spacing)*UNITS_DISTANCE + blockLength )
        {
            targetGrid.y -= 1 ;
            nextY = getAbsolutePoint(targetGrid).y ;
        }
        for( int y = sourceGrid.y ; y >= targetGrid.y ; y-- )
        {
            Coordinate3D solution ;
            solution.x = sourceGrid.x ;
            solution.y = y ;
            solution.z = sourceGrid.z ;
            paths.push_back(solution);
        }
        fillSpNetMaps(paths, powerpin, blockName , BlockPinName ,width ,  false );
        vector<Coordinate3D> absolutePoints ;
        for(auto p : paths)
        {
            auto pt = getAbsolutePoint(p);
            Coordinate3D coordinate3D(pt.x , pt.y , p.z);
            absolutePoints.push_back(coordinate3D);
        }
        if(source) sourceTargetInitPath.insert(make_pair(powerpin, absolutePoints));
        else sourceTargetInitPath.insert(make_pair(key, absolutePoints));
        for(int i = lowestMetal ; i <= highestMetal ; i++)
        {
            BlockCoordinate virtualObstacle ;
            int z = sourceGrid.z ;
            Point<int> Source = getAbsolutePoint(sourceGrid);
            Point<int> Target = getAbsolutePoint(targetGrid);
            virtualObstacle.RightUp = Source;
            virtualObstacle.LeftDown = Target;
            virtualObstacle.LeftDown.y -= (0.5 * DEFAULTWIDTH) * UNITS_DISTANCE ;
            virtualObstacle.LeftDown.x -= width / 2 * UNITS_DISTANCE ;
            virtualObstacle.RightUp.x += width / 2 * UNITS_DISTANCE ;
            if( i != z )
            {
                virtualObstacle.lowerMetal = i ;
                virtualObstacle.upperMetal = i ;
            }
            virtualObstacles.push_back(virtualObstacle);
        }
    }
    else if(block.Direction == RIGHT )
    {
        int blockWidth = block.RightUp.x - block.LeftDown.x ;
        Coordinate3D targetGrid = sourceGrid ;
        int currentX = getAbsolutePoint(sourceGrid).x ;
        targetGrid.x += 1 ;
        int nextX = getAbsolutePoint(targetGrid).x ;
        // get how many counts should be calcaulated.
        while( nextX - currentX !=  (0.5 * width + spacing)*UNITS_DISTANCE + blockWidth )
        {
            targetGrid.x += 1 ;
            nextX = getAbsolutePoint(targetGrid).x ;
        }
        for( int x = sourceGrid.x ; x <= targetGrid.x ; x++ )
        {
            Coordinate3D solution ;
            solution.x = x ;
            solution.y = sourceGrid.y ;
            solution.z = sourceGrid.z ;
            paths.push_back(solution);
        }
        fillSpNetMaps(paths, powerpin, blockName , BlockPinName ,width ,  false );
        vector<Coordinate3D> absolutePoints ;
        for(auto p : paths)
        {
            auto pt = getAbsolutePoint(p);
            Coordinate3D coordinate3D(pt.x , pt.y , p.z);
            absolutePoints.push_back(coordinate3D);
        }
        if(source) sourceTargetInitPath.insert(make_pair(powerpin, absolutePoints));
        else sourceTargetInitPath.insert(make_pair(key, absolutePoints));
        for(int i = lowestMetal ; i <= highestMetal ; i++)
        {
            BlockCoordinate virtualObstacle ;
            int z = sourceGrid.z ;
            Point<int> Source = getAbsolutePoint(sourceGrid);
            Point<int> Target = getAbsolutePoint(targetGrid);
            virtualObstacle.LeftDown = Source;
            virtualObstacle.RightUp = Target;
            virtualObstacle.RightUp.x += (0.5 * DEFAULTWIDTH) * UNITS_DISTANCE ;
            virtualObstacle.LeftDown.y -= width / 2 * UNITS_DISTANCE ;
            virtualObstacle.RightUp.y += width / 2 * UNITS_DISTANCE ;
            if( i != z )
            {
                virtualObstacle.lowerMetal = i ;
                virtualObstacle.upperMetal = i ;
            }
            virtualObstacles.push_back(virtualObstacle);
        }
    }
    else if(block.Direction == LEFT)
    {
        int blockWidth = block.RightUp.x - block.LeftDown.x ;
        Coordinate3D targetGrid = sourceGrid ;
        int currentX = getAbsolutePoint(sourceGrid).x ;
        targetGrid.x -= 1 ;
        int nextX = getAbsolutePoint(targetGrid).x ;
        // get how many counts should be calcaulated.
        while( currentX - nextX !=  (0.5 * width + spacing)*UNITS_DISTANCE + blockWidth )
        {
            targetGrid.x -= 1 ;
            nextX = getAbsolutePoint(targetGrid).x ;
        }
        for( int x = sourceGrid.x ; x >= targetGrid.x ; x-- )
        {
            Coordinate3D solution ;
            solution.x = x ;
            solution.y = sourceGrid.y ;
            solution.z = sourceGrid.z ;
            paths.push_back(solution);
        }
        SpecialNetsMaps.begin();
        fillSpNetMaps(paths, powerpin, blockName , BlockPinName ,width ,  false );
        vector<Coordinate3D> absolutePoints ;
        for(auto p : paths)
        {
            auto pt = getAbsolutePoint(p);
            Coordinate3D coordinate3D(pt.x , pt.y , p.z);
            absolutePoints.push_back(coordinate3D);
        }
        if(source) sourceTargetInitPath.insert(make_pair(powerpin, absolutePoints));
        else sourceTargetInitPath.insert(make_pair(key, absolutePoints));
        for(int i = lowestMetal ; i <= highestMetal ; i++)
        {
            BlockCoordinate virtualObstacle ;
            int z = sourceGrid.z ;
            Point<int> Source = getAbsolutePoint(sourceGrid);
            Point<int> Target = getAbsolutePoint(targetGrid);
            virtualObstacle.RightUp = Source;
            virtualObstacle.LeftDown = Target;
            virtualObstacle.LeftDown.x -= (0.5 * DEFAULTWIDTH) * UNITS_DISTANCE ;
            virtualObstacle.LeftDown.y -= width / 2 * UNITS_DISTANCE ;
            virtualObstacle.RightUp.y += width / 2 * UNITS_DISTANCE ;
            if( i != z )
            {
                virtualObstacle.lowerMetal = i ;
                virtualObstacle.upperMetal = i ;
            }
            virtualObstacles.push_back(virtualObstacle);
        }
    }
    for( auto virtualObstacle : virtualObstacles )
        obstacles[powerpin].push_back(virtualObstacle);
}
void RouterV4::InitPowerPinAndBlockPin(double width , double spacing )
{
    InitGrids("", width , spacing );

    for( auto it = Connection.begin() ; it != Connection.end() ; it = Connection.upper_bound(it->first))
    {
        auto ret = Connection.equal_range(it->first);
        for (auto i = ret.first; i != ret.second; ++i)
        {
            string powerpin = i->first ;
            string block = i->second.BlockName ;
            string blockPin = i->second.BlockPinName ;
            vector<Block> powerPinCoordinates = RouterHelper.getPowerPinCoordinate(powerpin);
            // 如果有multiSource 目前hardcode第一個
//            Block powerPinCoordinate = powerPinCoordinates[0];
            Block blockPinCoordinate = RouterHelper.getBlock(block,blockPin);
            for(auto powerPinCoordinate : powerPinCoordinates)
            {
                if( i == ret.first ) getInitSolution(powerPinCoordinate,powerpin,block,blockPin,width,spacing,true );
                getInitSolution(blockPinCoordinate,powerpin,block,blockPin,width,spacing,false);
            }
        }
    }
}
Coordinate3D RouterV4::gridToAbsolute(Coordinate3D gridCoordinate)
{
    Coordinate3D coordinateAbs;
    Point<int> pt = getAbsolutePoint(gridCoordinate);
    coordinateAbs.x = pt.x ;
    coordinateAbs.y = pt.y ;
    coordinateAbs.z = gridCoordinate.z ;
    return coordinateAbs ;
}
void RouterV4::saveMultiPinCandidates(string powerPin , vector<Coordinate3D> solutions )
{
    if( multiPinCandidates.find(powerPin) == multiPinCandidates.end() ) multiPinCandidates.insert(make_pair(powerPin, vector<Coordinate3D>()));
    for( auto solution : solutions )
    {
        Coordinate3D coordinate = gridToAbsolute(solution);
        multiPinCandidates[powerPin].push_back(coordinate);
    }
}
bool RouterV4::isMultiPin(string powerPin)
{
    return Connection.count(powerPin) > 1 ;
}
void RouterV4::legalizeEdge(Coordinate3D source , Coordinate3D target , Direction3D orient , Graph_SP * graph_sp , double width )
{
    if( source == target ) return ;
    int cnt = 0 ;
    if( orient == topOrient )
    {
        cnt = target.z - source.z ;
        for(int i = 0 ; i < cnt ; i++)
        {
            int from = translate3D_1D(target);
            target.z -= 1 ;
            int viaWeight = RouterHelper.getViaWeight(width * width, target.z);
            int to = translate3D_1D(target);
            graph_sp->UpdateWeight(from, to, viaWeight);
            graph_sp->UpdateWeight(to, from , viaWeight);
        }
    }
    else if( orient == bottomOrient )
    {
        cnt = source.z - target.z ;
        for(int i = 0 ; i < cnt ; i++)
        {
            int from = translate3D_1D(target);
            int viaWeight = RouterHelper.getViaWeight(width * width, target.z);
            target.z += 1 ;
            int to = translate3D_1D(target);
            graph_sp->UpdateWeight(from, to, viaWeight);
            graph_sp->UpdateWeight(to, from , viaWeight);
        }
    }
    else if( orient == upOrient )
    {
        cnt = target.y - source.y ;
        for(int i = 0 ; i < cnt ; i++)
        {
            int x = target.x ;
            int y = target.y ;
            int z = target.z ;
            int from = translate3D_1D(target);
            target.y -= 1 ;
            int to = translate3D_1D(target);
            graph_sp->UpdateWeight(from, to,RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y-1][x].length) * 10000);
            graph_sp->UpdateWeight(to, from , RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y-1][x].length) * 10000);
        }
    }
    else if (orient == downOrient)
    {
        cnt = source.y - target.y ;
        for(int i = 0 ; i < cnt ; i++)
        {
            int x = target.x ;
            int y = target.y ;
            int z = target.z ;
            int from = translate3D_1D(target);
            target.y += 1 ;
            int to = translate3D_1D(target);
            graph_sp->UpdateWeight(from, to,RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x].length) * 10000);
            graph_sp->UpdateWeight(to, from , RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x].length) * 10000);
        }
    }
    else if( orient == leftOrient )
    {
        cnt = source.x - target.x ;
        for(int i = 0 ; i < cnt ; i++)
        {
            int x = target.x ;
            int y = target.y ;
            int z = target.z ;
            int from = translate3D_1D(target);
            target.x += 1 ;
            int to = translate3D_1D(target);
            graph_sp->UpdateWeight(from, to,RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x].width) * 10000);
            graph_sp->UpdateWeight(to, from , RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x].width) * 10000);
        }
    }
    else if( orient == rightOrient)
    {
        cnt = target.x - source.x ;
        for(int i = 0 ; i < cnt ; i++)
        {
            int x = target.x ;
            int y = target.y ;
            int z = target.z ;
            int from = translate3D_1D(target);
            target.x -= 1 ;
            int to = translate3D_1D(target);
            graph_sp->UpdateWeight(from, to,RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x-1].width) * 10000);
            graph_sp->UpdateWeight(to, from , RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, width * UNITS_DISTANCE, Grids[y][x-1].width) * 10000);
        }
    }
    
}
void RouterV4::legalizeAllOrient(Coordinate3D coordinate , Graph_SP * graph_sp , double width , double spacing , double originWidth)
{
    // top
    auto lastLegal = getLastIlegalCoordinate(topOrient, coordinate , width , spacing , originWidth);
    legalizeEdge(coordinate, lastLegal, topOrient , graph_sp , width);
    // bottom
    lastLegal = getLastIlegalCoordinate(bottomOrient, coordinate , width , spacing , originWidth);
    legalizeEdge(coordinate, lastLegal, bottomOrient , graph_sp , width);
    // up
    lastLegal = getLastIlegalCoordinate(upOrient, coordinate , width , spacing , originWidth);
    legalizeEdge(coordinate, lastLegal, upOrient , graph_sp , width);
    // down
    lastLegal = getLastIlegalCoordinate(downOrient, coordinate , width , spacing , originWidth);
    legalizeEdge(coordinate, lastLegal, downOrient , graph_sp , width);
    // left
    lastLegal = getLastIlegalCoordinate(leftOrient, coordinate , width , spacing , originWidth);
    legalizeEdge(coordinate, lastLegal, leftOrient , graph_sp , width);
    // right
    lastLegal = getLastIlegalCoordinate(rightOrient, coordinate , width , spacing , originWidth);
    legalizeEdge(coordinate, lastLegal, rightOrient , graph_sp , width);
}
Coordinate3D RouterV4::getLastIlegalCoordinate(Direction3D orient , Coordinate3D sourceGrid , double width , double spacing , double originWidth)
{
    Coordinate3D targetGrid = sourceGrid ;
    if( orient == topOrient )
    {
        if( sourceGrid.z == highestMetal ) return targetGrid ;
        targetGrid.z += 1 ;
    }
    else if( orient == bottomOrient )
    {
        if( sourceGrid.z == lowestMetal ) return targetGrid ;
        targetGrid.z -= 1 ;
    }
    else if( orient == upOrient )
    {
        int currentY = getAbsolutePoint(sourceGrid).y ;
        int targetY = currentY + (0.5 * width + spacing)*UNITS_DISTANCE + 0.5 * originWidth * UNITS_DISTANCE ;
        if( Horizontal.find(targetY) == Horizontal.end() )
        {
            
            return sourceGrid ;
        }
        targetGrid.y = getGridY(targetY);
    }
    else if (orient == downOrient)
    {
        int currentY = getAbsolutePoint(sourceGrid).y ;
        int targetY = currentY - (0.5 * width + spacing)*UNITS_DISTANCE - 0.5 * originWidth * UNITS_DISTANCE;
        if( Horizontal.find(targetY) == Horizontal.end() )
        {
            
            return sourceGrid ;
        }
        targetGrid.y = getGridY(targetY);
    }
    else if( orient == leftOrient )
    {
        int currentX = getAbsolutePoint(sourceGrid).x ;
        int targetX = currentX - (0.5 * width + spacing)*UNITS_DISTANCE - 0.5 * originWidth * UNITS_DISTANCE ;
        if( Vertical.find(targetX) == Vertical.end() )
        {
            
            return sourceGrid ;
        }
        targetGrid.x = getGridX(targetX);
    }
    else if( orient == rightOrient)
    {
        int currentX = getAbsolutePoint(sourceGrid).x ;
        int targetX = currentX + (0.5 * width + spacing)*UNITS_DISTANCE + 0.5 * originWidth * UNITS_DISTANCE ;
        if( Vertical.find(targetX) == Vertical.end() )
        {
            
            return sourceGrid ;
        }
        targetGrid.x = getGridX(targetX);
    }
    return targetGrid ;
}
double RouterV4::getCost(string spiceName)
{
    // penalty constant
    int penaltyParameter = 1 ;
    // 超過 2% 會有penalty
    int penaltyRange = 2 ;
    double cost = 0 ;
    string cmd = "./ngspice " + spiceName + " -o simulation" ;
    system(cmd.c_str());
    ngspice ng_spice ;
    ng_spice.initvoltage();
    for( auto routingList : currentRoutingLists )
    {
//        Coordinate3D sourceGrid( getGridX(routingList.sourceCoordinate.x) , getGridY(routingList.sourceCoordinate.y) , routingList.sourceCoordinate.z );
//        Coordinate3D targetGrid( getGridX(routingList.targetCoordinate.x) , getGridY(routingList.targetCoordinate.y) , routingList.targetCoordinate.z );
//        string sourceKey = getNgSpiceKey(sourceGrid) ;
//        string targetKey = getNgSpiceKey(targetGrid) ;
        string sourceKey = getNgSpiceKey(routingList.sourceCoordinate) ;
        string targetKey = getNgSpiceKey(routingList.targetCoordinate) ;
        if( ng_spice.voltages.find(sourceKey) == ng_spice.voltages.end() ) assert(0);
        if( ng_spice.voltages.find(targetKey) == ng_spice.voltages.end() ) assert(0);
        
        double sourceV = ng_spice.voltages[sourceKey];
        double targetV = ng_spice.voltages[targetKey];
        double drop = (sourceV - targetV) * 100 ;
        double constaint = RouterHelper.getIRDropConstaint(routingList.targetBlockName, routingList.targetBlockPinName);
        if( constaint >= drop )
        {
            cost += 0 ;
        }
        else
        {
            double diff = drop - constaint ;
            cost += diff ;
            double penaltyCount = diff / penaltyRange ;
            cost += penaltyCount * penaltyParameter ;
        }
    }
    return cost ;
}
Coordinate3D RouterV4::AbsToGrid(Coordinate3D coordinateABS)
{
    Coordinate3D coordinate( getGridX(coordinateABS.x) , getGridY(coordinateABS.y) , coordinateABS.z );
    return coordinate ;
}
vector<Coordinate3D> RouterV4::selectPath(string powerPin , Graph_SP * graph_sp , int target, int source  , string block , string blockPin , double width , double spacing , double originWidth)
{
    double minCost = INT_MAX; 
    vector<Coordinate3D> minCostSolutions ;
    vector<Coordinate3D> selectedPath ;
    Coordinate3D sourceGrid = translate1D_3D(source);
    Coordinate3D targetGrid = translate1D_3D(target);
    // legalize target
    legalizeAllOrient(targetGrid, graph_sp , width , spacing , originWidth);
    // first Route
    if( multiPinCandidates[powerPin].empty() )
    {
        legalizeAllLayer(sourceGrid, graph_sp,width,spacing , originWidth);
        graph_sp->Dijkstra(target);
        auto paths= graph_sp->getPath(source);
        for( auto path : paths )
            selectedPath.push_back(translate1D_3D(path));
        generateSpiceList(selectedPath, powerPin, block , blockPin , width);
        return selectedPath ;
    }
    cout << "multipin candidate:" << multiPinCandidates[powerPin].size() << endl;
//    for( auto candidate : multiPinCandidates[powerPin] )
    for( int i = 0 ; i < multiPinCandidates[powerPin].size() ; i+=100 )
    {
        if( i > multiPinCandidates[powerPin].size()  ) break;
        Coordinate3D candidate = multiPinCandidates[powerPin][i];
        Coordinate3D coordinate( getGridX(candidate.x) , getGridY(candidate.y) , candidate.z );
        if( coordinate == sourceGrid )
            legalizeAllLayer(sourceGrid, graph_sp , width , spacing , originWidth);
        else
            legalizeAllOrient(coordinate, graph_sp , width ,spacing , originWidth);
        graph_sp->Dijkstra(target);
        auto path1D = graph_sp->getPath(translate3D_1D(coordinate));
        if( path1D.empty() ) continue ;
        vector<Coordinate3D> path3D ;
        for( auto p : path1D )
            path3D.push_back(translate1D_3D(p));
        SpiceGenerator tmp = sp_gen ;
        tmp.setSpiceName("tmp.sp");
        string key = block ;
        key.append(blockPin);
        auto initTargetPath = sourceTargetInitPath[key] ;
        
        tmp.addSpiceCurrent(powerPin, gridToString(initTargetPath[0],false), RouterHelper.getCurrent(block, blockPin));
        for( auto & Path : initTargetPath )
        {
            Path.x = getGridX(Path.x);
            Path.y = getGridY(Path.y);
        }
        genResistance(path3D, powerPin , tmp , width);
        genResistance(initTargetPath,powerPin,tmp , width );
        
        tmp.toSpice();
        tmp.addSpiceCmd();
//        generateSpiceList(path3D, powerPin, blockinfo);
//        sp_gen.toSpice();
//        sp_gen.addSpiceCmd();
        double currentCost = getCost("tmp.sp");
        if( currentCost < minCost )
        {
            minCost = currentCost;
            minCostSolutions = path3D ;
        }
    }
    string key = block ;
    key.append(blockPin);
    auto initTargetPath = sourceTargetInitPath[key] ;
    
    sp_gen.addSpiceCurrent(powerPin, gridToString(initTargetPath[0],false), RouterHelper.getCurrent(block, blockPin));
    for( auto & Path : initTargetPath )
    {
        Path.x = getGridX(Path.x);
        Path.y = getGridY(Path.y);
    }
    genResistance(minCostSolutions, powerPin , sp_gen , width );
    genResistance(initTargetPath,powerPin,sp_gen , width);
    
    return minCostSolutions ;
}
// x y
void RouterV4::saveRoutingList(Coordinate3D source , Coordinate3D target , string powerPin , BlockInfo blockinfo)
{
    RoutingPath routePath;
    routePath.sourceName = powerPin ;
    routePath.sourceCoordinate = source;
    routePath.targetCoordinate = target;
    routePath.targetBlockName = blockinfo.BlockName ;
    routePath.targetBlockPinName = blockinfo.BlockPinName ;
    currentRoutingLists.push_back(routePath);
}
void RouterV4::legalizeAllLayer(Coordinate3D source , Graph_SP * graph_sp , double width , double spacing , double originWidth)
{
    for(int z = lowestMetal ; z <= highestMetal ; z++)
    {
        Coordinate3D temp = source;
        temp.z = z ;
        legalizeAllOrient(temp, graph_sp ,width ,spacing , originWidth);
    }
}
pair<vector<string>,map<string,vector<Path>>> RouterV4::getNetOrdering(double width, double spacing , double originWidth)
{
    cout << "Begin determine Net Ordering ..." << endl;
    clock_t Start = clock();
    // 決定tree 的順序
    vector<string> treeOrder ;
    // tree 內部的順序
    map<string,vector<Path>> innerTreeOrder ;
    map<double,Path> criteriaOrdering ;
    map<InnerTreeObj,Path,InnerTreeObjComparator> innerTreeOrderMap ;
    for( auto it = Connection.begin() ; it != Connection.end() ; it = Connection.upper_bound(it->first))
    {
        auto ret = Connection.equal_range(it->first);
        for (auto i = ret.first; i != ret.second; ++i)
        {
            string powerpin = i->first ;
            string block = i->second.BlockName ;
            string blockPin = i->second.BlockPinName ;
            
//            cout << powerpin << " " << block << " " << blockPin << endl;
            InitGrids(powerpin,width , spacing);
            Graph_SP * graph_sp = InitGraph_SP(width,spacing);
            vector<Block> powerPinCoordinates = RouterHelper.getPowerPinCoordinate(powerpin);
            Block powerPinCoordinate = powerPinCoordinates[0];
            Block BlockPinCoordinate = RouterHelper.getBlock(block, blockPin);
            Coordinate3D sourceGrid = LegalizeTargetEdge(powerPinCoordinate , graph_sp , width , spacing);
            Coordinate3D targetGrid = LegalizeTargetEdge(BlockPinCoordinate , graph_sp , width , spacing);
            int source = translate3D_1D(sourceGrid);
            int target = translate3D_1D(targetGrid);
            legalizeAllLayer(sourceGrid, graph_sp , width , spacing , originWidth);
            vector<Coordinate3D> solutions ;
            graph_sp->Dijkstra(target);
            auto paths= graph_sp->getPath(source);
            delete [] graph_sp; 
            for( auto path : paths )
                solutions.push_back(translate1D_3D(path));
            auto friendlyForm = translateToFriendlyForm(solutions);
            int defaultLayer = solutions[0].z ;
            double sourceV = stod(VoltageMaps[powerpin]);
            double constraint = RouterHelper.getIRDropConstaint(block, blockPin) / 100 ;
            double allowDrop = sourceV * ( constraint );
            double Sum = 0 ;
            double totalDistance = 0 ;
            double I = RouterHelper.getCurrent(block, blockPin);
            auto Distance = getAbsoluteDistance(friendlyForm, Point<int>(solutions[0].x,solutions[0].y));
            for( int i = 0 ; i < friendlyForm.size() ; i++ )
            {
                double deltaV = 0 ;
                if( friendlyForm[i].first !=  topOrient && friendlyForm[i].first != bottomOrient)
                {
                    totalDistance += Distance[i] ;
                    double currentResistance = LayerMaps[ RouterHelper.translateIntToMetalName(defaultLayer) ].RESISTANCE_RPERSQ ;
                    
                    double R = RouterHelper.calculateResistance(currentResistance, width * UNITS_DISTANCE, Distance[i]);
                    deltaV = I * R ;
                }
                else
                {
                    if( friendlyForm[i].first ==  topOrient )
                    {
                        double R = RouterHelper.getViaWeight(width * width , defaultLayer ) / 10000 ;
                        deltaV = I * R ;
                        defaultLayer += 1;
                        
                    }
                    if( friendlyForm[i].first ==  bottomOrient )
                    {
                        defaultLayer -= 1;
                        double R = RouterHelper.getViaWeight(width * width , defaultLayer ) / 10000 ;
                        deltaV = I * R ;
                    }
                }
                Sum += deltaV;
            }
            Path p ;
            p.source = powerpin;
            p.target.first = block ;
            p.target.second = blockPin ;
            
            double criteria = (allowDrop - Sum)/totalDistance ;
            InnerTreeObj innerObj ;
            innerObj.current = I ;
            innerObj.critical = criteria ;
            if( criteriaOrdering.find(criteria) != criteriaOrdering.end() )
            {
                innerObj.critical *= 0.99;
            }
            criteriaOrdering.insert(make_pair(criteria, p));
            innerTreeOrderMap.insert(make_pair(innerObj, p));
        }
    }
    // decide external order
    for(auto order : criteriaOrdering)
    {
        
        bool insert = true ;
        for(auto treeorder : treeOrder)
        {
            if( treeorder == order.second.source )
            {
                insert = false ;
                break;
            }
        }
        if(insert)treeOrder.push_back(order.second.source);
    }
    // decide inner order
    for(auto innerOrder : innerTreeOrderMap)
    {
        if( innerTreeOrder.find(innerOrder.second.source) == innerTreeOrder.end()) innerTreeOrder.insert(make_pair(innerOrder.second.source, vector<Path>()));
        innerTreeOrder[innerOrder.second.source].push_back(innerOrder.second);
    }
//    for(auto order : distanceOrdering)
//    {
//        if( innerTreeOrder.find(order.second.source) == innerTreeOrder.end()) innerTreeOrder.insert(make_pair(order.second.source, vector<Path>()));
//        innerTreeOrder[order.second.source].push_back(order.second);
//    }
    clock_t End = clock();
    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
    cout << "Net Ordering cost " << duration << "(s)" << endl;
    return make_pair(treeOrder, innerTreeOrder);
}
/*
 Algorithm:
    決定netOrder ， 用來長Steiner Tree ，以VDD為單位
        definition of critical: 單位長度掉得電壓越大越critical( V/LC )
        netOrder: 1. 最critical
    長Steiner Tree:
        線長最長得線開始merge
    長出Steiner Tree 以後 ， 利用電流原理去Tune 線寬 ， 有個標準去判斷這種線寬是否合適
        Tune wire width: 
            用 pin to pin 去估線寬，共同部份累加。
            估線寬要考慮congestion
        問題：標準為何？
        方法：標準：
            1. routabliloity ?
                繞不出來的線，技術性detour(如果符合IR DROP)，看可不可以解決routabliloity的問題
        不符合標準須長出新的Tree(一樣必須考慮congestion) trade off between congestion and wirelength( IR-DROP )
        問題：如何長出新的樹？
            1. 要把哪些點踢掉，要新增到哪些點
            2. 如果把root拿掉怎麼辦(?) 應該不會，會一直做iteration讓所有人IR 都過
        方法：如何長出新的樹:
            1. 從critical程度最重的開始拔，會有個cost function 去評估這棵樹的 cost ，拔到cost 無法再被降低
               拔的點要插哪裡：(算得出來？)
                cost function:
                    prioritized order:
                    1. routabliloity
                    2. 有沒有符合各點IR DROP
                    3. total metal usage 根據大會給的包括權重
 
 
 */
void RouterV4::SteinerTreeConstruction( bool isSimulation , vector<Coordinate3D> & solutions , double current , double constraint , double voltage , string powerPin , string block , string blockPin ,  Graph * & steinerTree)
{
    
    vector<Coordinate3D> absSolutions;
    for(auto sol : solutions)
    {
        auto coordinate = gridToAbsolute(sol);
        absSolutions.push_back(coordinate);
    }
    auto friendlyForm = translateToFriendlyForm(solutions);
    
    if( steinerTree == nullptr )
    {
        steinerTree = new Graph[1];
        steinerTree->initialize(absSolutions[0]);
    }
    else
    {
        steinerTree->setSteriner(absSolutions[0]);
    }
    steinerTree->setSimulationMode(isSimulation);
    // 從source 到 target
    for(int i = 1 ; i < absSolutions.size() ; i++)
    {
//        cout << absSolutions[i].x << " " << absSolutions[i].y << " " << absSolutions[i].z << endl;
        Point<int> pt1(absSolutions[i-1].x , absSolutions[i-1].y);
        Point<int> pt2(absSolutions[i].x , absSolutions[i].y);
        double distance = RouterHelper.getManhattanDistance( pt1 , pt2);
        steinerTree->insert(absSolutions[i-1], absSolutions[i], distance);
    }
    steinerTree->addLeaf(*(--absSolutions.end()), constraint , current , voltage);
    steinerTree->addLeafInfo(*(--absSolutions.end()) , powerPin, block, blockPin);
}
vector<Coordinate3D> RouterV4::selectMergePoint(double constraint , double current , double voltage , Graph * steinerTree , string powerPin , Graph_SP * graph_sp , int target, int source  , string block , string blockPin , double width , double spacing , double originWidth)
{
    Coordinate3D sourceGrid = translate1D_3D(source);
    Coordinate3D targetGrid = translate1D_3D(target);
    // legalize target
    legalizeAllLayer(targetGrid, graph_sp , width , spacing , originWidth);
    // first Route
    if( multiPinCandidates[powerPin].empty() )
    {
        vector<Coordinate3D> selectedPath ;
        legalizeAllLayer(sourceGrid, graph_sp , width , spacing , originWidth);
        graph_sp->Dijkstra(target);
        auto paths= graph_sp->getPath(source);
        for( auto path : paths )
            selectedPath.push_back(translate1D_3D(path));
        generateSpiceList(selectedPath, powerPin, block , blockPin , width);
        return selectedPath ;
    }
    else
    {
        double minCost = INT_MAX;
        vector<Coordinate3D> minCostSolutions ;
//        for( auto candidate : multiPinCandidates[powerPin] )
//        {
//            Coordinate3D coordinate3D( getGridX(candidate.x) , getGridY(candidate.y) , candidate.z );
//            if( coordinate3D == sourceGrid )
//                legalizeAllLayer(sourceGrid, graph_sp , width , spacing , originWidth);
//            else
//                legalizeAllOrient(coordinate3D, graph_sp , width , spacing , originWidth);
//        }
//        graph_sp->Dijkstra(target);
        for(int i = 0 ; i < multiPinCandidates[powerPin].size() ; i += multiPinCandidates[powerPin].size() / 3  )
        {
            if( i > multiPinCandidates[powerPin].size()  ) break;
            Coordinate3D candidate = multiPinCandidates[powerPin][i];
            Coordinate3D coordinate3D( getGridX(candidate.x) , getGridY(candidate.y) , candidate.z );
            if( coordinate3D == sourceGrid )
                legalizeAllLayer(sourceGrid, graph_sp , width , spacing , originWidth);
            else
                legalizeAllOrient(coordinate3D, graph_sp , width ,spacing , originWidth);
            graph_sp->Dijkstra(target);
            int mergePoint = translate3D_1D(coordinate3D) ;
            auto paths = graph_sp->getPath(mergePoint);
            vector<Coordinate3D> solutions ;
            for( auto path : paths )
                solutions.push_back(translate1D_3D(path));
            if(!paths.empty())
            {
                SteinerTreeConstruction(true , solutions, current, constraint, voltage, powerPin, block, blockPin, steinerTree);
                double FOM = steinerTree->analysis();
                if( minCost > FOM )
                {
                    minCost = FOM ;
                    minCostSolutions = solutions ;
                }
                vector<Coordinate3D> absSolutions ;
                for(auto sol : solutions)
                    absSolutions.push_back(gridToAbsolute(sol));
                steinerTree->erase(absSolutions);
                steinerTree->reset() ;
            }
        }
        string key = block ;
        key.append(blockPin);
        auto initTargetPath = sourceTargetInitPath[key] ;
        
        sp_gen.addSpiceCurrent(powerPin, gridToString(initTargetPath[0],false), RouterHelper.getCurrent(block, blockPin));
        for( auto & Path : initTargetPath )
        {
            Path.x = getGridX(Path.x);
            Path.y = getGridY(Path.y);
        }
        genResistance(minCostSolutions, powerPin , sp_gen ,width );
        genResistance(initTargetPath,powerPin,sp_gen , width);
        return minCostSolutions ;
    }

    return vector<Coordinate3D>() ;
}
void RouterV4::SteinerTreeReduction(Graph * &steinerTree , vector<Coordinate3D> & terminals)
{
    for(auto terminal : terminals)
    {
        string encodeString = steinerTree->encode(terminal);
        
    }
}
void RouterV4::InitializeSpiceGen(Graph * steinerTree)
{
    vector<LeafInfo> leafInfos = steinerTree->getLeafInfos();
    auto initSourcePath = sourceTargetInitPath[leafInfos[0].powerPin] ;
    sp_gen.initSpiceVdd(leafInfos[0].powerPin, gridToString(initSourcePath[0],false), stod(VoltageMaps[leafInfos[0].powerPin]));
    for( auto & Path : initSourcePath )
    {
        Path.x = getGridX(Path.x);
        Path.y = getGridY(Path.y);
    }
    genResistance(initSourcePath,leafInfos[0].powerPin,sp_gen,DEFAULTWIDTH);
    for(auto leafInfo : leafInfos)
    {
        
        string key;
        key.append(leafInfo.block).append(leafInfo.blockPin);
        
        auto initTargetPath = sourceTargetInitPath[key] ;
        
        sp_gen.addSpiceCurrent(leafInfo.powerPin, gridToString(initTargetPath[0],false), RouterHelper.getCurrent(leafInfo.block, leafInfo.blockPin));
        
        for( auto & Path : initTargetPath )
        {
            Path.x = getGridX(Path.x);
            Path.y = getGridY(Path.y);
        }
        
        genResistance(initTargetPath,leafInfo.powerPin,sp_gen,DEFAULTWIDTH);
    }
    
    // generate resistance
//    genResistance(paths,powerPinName,sp_gen,width);
    
}
vector<Coordinate3D> RouterV4::getCorner(vector<pair<Direction3D, int>> & friendlyForm , Coordinate3D source)
{
    vector<Coordinate3D> corners ;
    Coordinate3D temp = source ;
    
    for( auto path : friendlyForm )
    {
        // "UP";
        if( path.first == 0 )
        {
            temp.y += path.second;
            corners.push_back(temp);
        }
        // "DOWN"
        if( path.first == 1 )
        {
            temp.y -= path.second;
            corners.push_back(temp);
        }
        // "LEFT"
        if( path.first == 2 )
        {
            temp.x -= path.second;
            corners.push_back(temp);
        }
        // "RIGHT"
        if( path.first == 3 )
        {
            temp.x += path.second;
            corners.push_back(temp);
        }
        // "TOP"
        if( path.first == 4 )
        {
            temp.z += path.second;
        }
        // "BOTTOM"
        if( path.first == 5 )
        {
            temp.z -= path.second;
        }
    }
    return corners ;
}
int RouterV4::gridYToAbs(int gridY)
{
    return ( gridY != 0 ) ? *std::next(Horizontal.begin(), gridY-1) : 0 ;
}
int RouterV4::gridXToAbs(int gridX)
{
    return ( gridX != 0 ) ? *std::next(Vertical.begin(), gridX-1) : 0 ;
}
Coordinate3D RouterV4::getNext(Direction3D direction , Coordinate3D corner)
{
    if( direction == upOrient )
    {
        int y = gridYToAbs(corner.y);
        int nextY = gridYToAbs(corner.y+1);
        if( nextY - y >= (0.5 * DEFAULTWIDTH + DEFAULTSPACING) * UNITS_DISTANCE )
        {
            corner.y += 1;
            return corner;
        }
        while ( nextY - y < (0.5 * DEFAULTWIDTH + DEFAULTSPACING) * UNITS_DISTANCE )
        {
            corner.y += 1;
            nextY = gridYToAbs(corner.y);
        }
        return corner ;
    }
    else if( direction == downOrient )
    {
        int y = gridYToAbs(corner.y);
        int nextY = gridYToAbs(corner.y-1);
        if( y - nextY >= (0.5 * DEFAULTWIDTH + DEFAULTSPACING) * UNITS_DISTANCE )
        {
            corner.y -= 1;
            return corner;
        }
        while ( y - nextY < (0.5 * DEFAULTWIDTH + DEFAULTSPACING) * UNITS_DISTANCE )
        {
            corner.y -= 1;
            nextY = gridYToAbs(corner.y);
        }
        return corner ;
    }
    else if( direction == leftOrient )
    {
        int x = gridXToAbs(corner.x);
        int nextX = gridXToAbs(corner.x-1);
        if( x - nextX >= (0.5 * DEFAULTWIDTH + DEFAULTSPACING) * UNITS_DISTANCE )
        {
            corner.x -= 1;
            return corner;
        }
        while ( x - nextX < (0.5 * DEFAULTWIDTH + DEFAULTSPACING) * UNITS_DISTANCE )
        {
            corner.x -= 1;
            nextX = gridXToAbs(corner.x);
        }
        return corner ;
    }
    else if( direction == rightOrient )
    {
        int x = gridXToAbs(corner.x);
        int nextX = gridXToAbs(corner.x+1);
        if( nextX - x >= (0.5 * DEFAULTWIDTH + DEFAULTSPACING) * UNITS_DISTANCE )
        {
            corner.x += 1;
            return corner;
        }
        while ( nextX - x < (0.5 * DEFAULTWIDTH + DEFAULTSPACING) * UNITS_DISTANCE )
        {
            corner.x += 1;
            nextX = gridXToAbs(corner.x);
        }
        return corner ;
    }
    return Coordinate3D();
}
Coordinate3D RouterV4::selectSource(Coordinate3D corner)
{
    return corner;
}
Coordinate3D RouterV4::selectTarget(Coordinate3D corner)
{
    return corner ;
}
void RouterV4::optimize(Graph * steinerTree)
{
    Simulation();
    steinerTree->reduction();
    steinerTree->rectifyCurrent() ;
    steinerTree->rectifyWidth() ;
    vector<vector<Vertex *>> traversePaths = steinerTree->traverse() ;
    if( !NoPassRoutingLists.empty() )
    {
        for( auto leaf : traversePaths )
        {
            LeafInfo leafInfo = steinerTree->getLeafInfo();
            Coordinate3D source = leaf[0]->coordinate ;
            Coordinate3D target = leaf[leaf.size()-1]->coordinate;
            vector<Coordinate3D> solutions = parallelRoute(leafInfo.powerPin, leafInfo.block, leafInfo.blockPin, source, target, DEFAULTWIDTH, DEFAULTSPACING, DEFAULTWIDTH);
            if(solutions.empty())
                cout << "";
            if( !solutions.empty() )
            {
                fillSpNetMaps(solutions, leafInfo.powerPin, leafInfo.block , leafInfo.blockPin , DEFAULTWIDTH , true );
                def_gen.toOutputDef();
                genResistance(solutions,leafInfo.powerPin,sp_gen , DEFAULTWIDTH);
                Simulation();
            }
        }
    }
}
void RouterV4::Route()
{
    
    auto ordering = getNetOrdering(DEFAULTWIDTH,DEFAULTSPACING,DEFAULTWIDTH);
    auto treeOrder = ordering.first ;
    auto innerTreeOrder = ordering.second ;
    
    InitPowerPinAndBlockPin(DEFAULTWIDTH,DEFAULTSPACING);
    for(auto tree : treeOrder)
    {
        Graph * steinerTree = nullptr ;
        for(auto innerTree : innerTreeOrder[tree])
        {
            BlockInfo blockinfo ;
            string powerpin = innerTree.source ;
            blockinfo.BlockName = innerTree.target.first ;
            blockinfo.BlockPinName = innerTree.target.second;
//            cout << powerpin << " " << blockinfo.BlockName << " " << blockinfo.BlockPinName << endl;
            double current = RouterHelper.getCurrent(blockinfo.BlockName, blockinfo.BlockPinName);
            double constraint = RouterHelper.getIRDropConstaint(blockinfo.BlockName, blockinfo.BlockPinName);
            double voltage = stod(VoltageMaps[powerpin]);
            vector<Block> powerPinCoordinates = RouterHelper.getPowerPinCoordinate(powerpin);
            bool init = true ;
            for(auto powerPinCoordinate : powerPinCoordinates)
            {
//                Block powerPinCoordinate = powerPinCoordinates[0];
                Block BlockPinCoordinate = RouterHelper.getBlock(blockinfo.BlockName, blockinfo.BlockPinName);
                InitGrids(powerpin,DEFAULTWIDTH , DEFAULTSPACING);
                Graph_SP * graph_sp = InitGraph_SP(DEFAULTWIDTH,DEFAULTSPACING);
                Coordinate3D sourceGrid = LegalizeTargetEdge(powerPinCoordinate , graph_sp , DEFAULTWIDTH , DEFAULTSPACING );
                Coordinate3D targetGrid = LegalizeTargetEdge(BlockPinCoordinate , graph_sp , DEFAULTWIDTH , DEFAULTSPACING);
                saveRoutingList(gridToAbsolute(sourceGrid),gridToAbsolute(targetGrid),powerpin,blockinfo);
                int source = translate3D_1D(sourceGrid);
                int target = translate3D_1D(targetGrid);
                vector<Coordinate3D> solutions = (init ) ? selectMergePoint(constraint , current , voltage , steinerTree , powerpin, graph_sp, target , source , blockinfo.BlockName , blockinfo.BlockPinName , DEFAULTWIDTH , DEFAULTSPACING , DEFAULTWIDTH) : selectMergePoint(constraint , current , voltage , steinerTree , powerpin, graph_sp, source , target , blockinfo.BlockName , blockinfo.BlockPinName , DEFAULTWIDTH , DEFAULTSPACING , DEFAULTWIDTH);
                //            vector<Coordinate3D> solutions = selectPath(powerpin, graph_sp, target, source, blockinfo.BlockName, blockinfo.BlockPinName, DEFAULTWIDTH, DEFAULTSPACING, DEFAULTWIDTH);
                //            cout << powerpin << " " << blockinfo.BlockName << " " << blockinfo.BlockPinName << endl;
                SteinerTreeConstruction(false , solutions,current, constraint , voltage , powerpin , blockinfo.BlockName , blockinfo.BlockPinName, steinerTree);
                fillSpNetMaps(solutions, powerpin, blockinfo.BlockName , blockinfo.BlockPinName , DEFAULTWIDTH ,true );
                saveMultiPinCandidates(powerpin, solutions);
                def_gen.toOutputDef();
                delete [] graph_sp ;
                init = false ;
            }
//            Simulation();
        }
//        optimize(steinerTree);
        delete [] steinerTree;
//        Simulation();
//        widthTable.clear();
    }
//    InitPowerPinAndBlockPin(DEFAULTWIDTH,DEFAULTSPACING);
//    def_gen.toOutputDef();
//    GlobalRouter gr ;
//    auto orders = gr.getNetOrdering();
//
//    for(auto order : orders)
//    {
//        BlockInfo blockinfo ;
//        string powerpin = order.second.source ;
//        blockinfo.BlockName = order.second.target.first ;
//        blockinfo.BlockPinName = order.second.target.second ;
//        vector<Block> powerPinCoordinates = RouterHelper.getPowerPinCoordinate(powerpin);
//        Block powerPinCoordinate = powerPinCoordinates[0];
//        Block BlockPinCoordinate = RouterHelper.getBlock(blockinfo.BlockName, blockinfo.BlockPinName);
//        InitGrids(powerpin,DEFAULTWIDTH , DEFAULTSPACING);
//        Graph_SP * graph_sp = InitGraph_SP(DEFAULTWIDTH,DEFAULTSPACING);
//        Coordinate3D sourceGrid = LegalizeTargetEdge(powerPinCoordinate , graph_sp , DEFAULTWIDTH , DEFAULTSPACING);
//        Coordinate3D targetGrid = LegalizeTargetEdge(BlockPinCoordinate , graph_sp , DEFAULTWIDTH , DEFAULTSPACING);
//        saveRoutingList(gridToAbsolute(sourceGrid),gridToAbsolute(targetGrid),powerpin,blockinfo);
//        int source = translate3D_1D(sourceGrid);
//        int target = translate3D_1D(targetGrid);
//        vector<Coordinate3D> solutions = selectPath(powerpin, graph_sp, target , source , blockinfo.BlockName , blockinfo.BlockPinName , DEFAULTWIDTH , DEFAULTSPACING , DEFAULTWIDTH);
//        fillSpNetMaps(solutions, powerpin, blockinfo.BlockName , blockinfo.BlockPinName , DEFAULTWIDTH,true );
//        saveMultiPinCandidates(powerpin, solutions);
//        def_gen.toOutputDef();
//        delete [] graph_sp ;
//    }
    Simulation();
//
//    
//    while (!NoPassRoutingLists.empty())
//    {
//        for(auto noPassRoutingList : NoPassRoutingLists)
//        {
//            if( !parallelRoute(noPassRoutingList.sourceName, noPassRoutingList.targetBlockName, noPassRoutingList.targetBlockPinName, noPassRoutingList.sourceCoordinate, noPassRoutingList.targetCoordinate, WIDTH, SPACING) )
//                assert(0);
//        }
//        Simulation();
//    }
//    sp_gen.toSpice();
//    system("rm tmp.sp");
//    system("rm simulation");
}
bool RouterV4::isSameLayer(vector<Coordinate3D> & path)
{
    int z = path[0].z;
    for(int i = 1 ; i < path.size() ; i++)
    {
        if( z != path[i].z )
            return false;
    }
    return true;
}
// coordindate 為 絕對座標 + Z
vector<Coordinate3D> RouterV4::parallelRoute(string powerPin ,string blockName , string blockPinName , Coordinate3D source , Coordinate3D target , double width , double spacing , double originWidth )
{
    
    Graph_SP * graph_sp = new Graph_SP[1];
    vector<Coordinate3D> solutions ;
    InitGrids(powerPin, DEFAULTWIDTH, DEFAULTSPACING);
    graph_sp = InitGraph_SP(DEFAULTWIDTH, DEFAULTSPACING);
    Coordinate3D sourceGrid = AbsToGrid(source);
    Coordinate3D targetGrid = AbsToGrid(target);
    legalizeAllLayer(sourceGrid, graph_sp , width , spacing , originWidth);
//    legalizeAllOrient(sourceGrid, graph_sp , width ,spacing , originWidth);
//    legalizeAllOrient(targetGrid, graph_sp , width , spacing , originWidth);
    legalizeAllLayer(targetGrid, graph_sp , width , spacing , originWidth);
    int source1D = translate3D_1D(sourceGrid);
    int target1D = translate3D_1D(targetGrid);
    graph_sp->Dijkstra(target1D);
    auto paths= graph_sp->getPath(source1D);
    delete [] graph_sp ;
    if( paths.empty() ) return vector<Coordinate3D>();
    for( auto path : paths )
        solutions.push_back(translate1D_3D(path));
    return solutions;
//    bool sameLayer = isSameLayer(solutions);
//    fillSpNetMaps(solutions, powerPin, blockName , blockPinName , width , true );
//    def_gen.toOutputDef();
//    genResistance(solutions,powerPin,sp_gen , width);
//    return (sameLayer) ? solutions : vector<Coordinate3D>();
}

// x y are abs , z is grid
string RouterV4::getNgSpiceKey(Coordinate3D coordinate3d)
{
    int z = coordinate3d.z ;
//    Point<int> pt = getAbsolutePoint(coordinate3d);
    string MetalName = RouterHelper.translateIntToMetalName(z) ;
    string result ;
    // to lowercase
    transform(MetalName.begin(), MetalName.end(), MetalName.begin(), ::tolower);
    result.append(MetalName).append("_").append(to_string(coordinate3d.x)).append("_").append(to_string(coordinate3d.y));
    return result ;
}
void RouterV4::Simulation()
{
    output_gen.clear();
    NoPassRoutingLists.clear();
    output_gen.setOutputFilesName(outputfilesName);
    sp_gen.toSpice();
    sp_gen.addSpiceCmd();
    string cmd = "./ngspice " + spiceName + " -o simulation" ;
    system(cmd.c_str());
    ngspice ng_spice ;
    ng_spice.initvoltage();
    for( auto routingList : currentRoutingLists )
    {
//        Coordinate3D sourceGrid( getGridX(routingList.sourceCoordinate.x) , getGridY(routingList.sourceCoordinate.y) , routingList.sourceCoordinate.z );
//        Coordinate3D targetGrid( getGridX(routingList.targetCoordinate.x) , getGridY(routingList.targetCoordinate.y) , routingList.targetCoordinate.z );
        string sourceKey = getNgSpiceKey(routingList.sourceCoordinate) ;
        string targetKey = getNgSpiceKey(routingList.targetCoordinate) ;
        if( ng_spice.voltages.find(sourceKey) == ng_spice.voltages.end() ) assert(0);
        if( ng_spice.voltages.find(targetKey) == ng_spice.voltages.end() ) assert(0);
        double sourceV = stod(VoltageMaps[routingList.sourceName]);
        double targetV = ng_spice.voltages[targetKey];
        double drop = (sourceV - targetV) / sourceV * 100 ;
        double constaint = RouterHelper.getIRDropConstaint(routingList.targetBlockName, routingList.targetBlockPinName);
        cout << routingList.sourceName << " to " << routingList.targetBlockName << "_" << routingList.targetBlockPinName << " Drop " << drop << "(%) " ;
        if( constaint >= drop )
        {
            cout << " Pass" << endl;
        }
        else
        {
            cout << " No Pass" << endl;
            NoPassRoutingLists.push_back(routingList);
        }
        cout << "IR Drop Constraint:" << constaint << "(%)"<< endl;
        cout << endl;
        output_gen.setIRDrop(routingList.targetBlockName, routingList.targetBlockPinName, drop);
    }
    output_gen.toOutputFiles();
}
double RouterV4::getMetalResistance(int layer)
{
    return LayerMaps[RouterHelper.translateIntToMetalName(layer)].RESISTANCE_RPERSQ ;
}
void RouterV4::genResistance(vector<Coordinate3D> & paths , string powerPinName , SpiceGenerator & spiceGenerator , double width )
{
    for(int i = 0 ; i < paths.size() - 1 ; i++)
    {
        string node1 = gridToString(paths[i] , true );
        string node2 = gridToString(paths[i+1] , true );
        Point<int> pt1 = getAbsolutePoint(paths[i]);
        Point<int> pt2 = getAbsolutePoint(paths[i+1]);
        // distance 0 means via
        int distance = ( pt1.x == pt2.x ) ? abs(pt1.y - pt2.y) : abs(pt1.x - pt2.x);
        // 目前打最小顆via (HardCode)
        int area = (int)width * (int)width ;
        double viaResistance = 10 ;
        double parallelViaResistance = viaResistance / area / 1   ;
        double resistance = ( distance != 0 ) ? RouterHelper.calculateResistance(getMetalResistance(paths[i].z), width * UNITS_DISTANCE, distance) : parallelViaResistance ;
        if(resistance < 0 ) assert(0);
        spiceGenerator.addSpiceResistance(powerPinName, node1, node2, resistance);
    }
}
int RouterV4::getGridX(int x)
{
    if( Vertical.find(x) == Vertical.end() ) return  0 ;
    return (int)distance(Vertical.begin(), Vertical.find(x)) + 1;
}
int RouterV4::getGridY(int y)
{
    if( Horizontal.find(y) == Horizontal.end() ) return  0 ;
    return (int)distance(Horizontal.begin(), Horizontal.find(y)) + 1 ;
}
void RouterV4::generateSpiceList(vector<Coordinate3D> & paths , string powerPinName , string blockName , string blockPinName , double width)
{
    string key;
    key.append(blockName).append(blockPinName);
    auto initSourcePath = sourceTargetInitPath[powerPinName] ;
    auto initTargetPath = sourceTargetInitPath[key] ;
    sp_gen.initSpiceVdd(powerPinName, gridToString(initSourcePath[0],false), stod(VoltageMaps[powerPinName]));
    sp_gen.addSpiceCurrent(powerPinName, gridToString(initTargetPath[0],false), RouterHelper.getCurrent(blockName, blockPinName));
    for( auto & Path : initSourcePath )
    {
        Path.x = getGridX(Path.x);
        Path.y = getGridY(Path.y);
    }
    for( auto & Path : initTargetPath )
    {
        Path.x = getGridX(Path.x);
        Path.y = getGridY(Path.y);
    }
    // generate resistance
    genResistance(initSourcePath,powerPinName,sp_gen,DEFAULTWIDTH);
    genResistance(paths,powerPinName,sp_gen,width);
    genResistance(initTargetPath,powerPinName,sp_gen,DEFAULTWIDTH);
    
}
Coordinate3D RouterV4::getGridCoordinate( Block block  )
{
    int x = 0 , y = 0 ;
    if( block.Direction == TOP )
    {
        x = (block.LeftDown.x + block.RightUp.x) / 2 ;
        y = block.LeftDown.y ;
    }
    else if( block.Direction == DOWN )
    {
        x = (block.LeftDown.x + block.RightUp.x) / 2 ;
        y = block.RightUp.y ;
    }
    else if( block.Direction == RIGHT )
    {
        x = block.LeftDown.x;
        y = ( block.LeftDown.y + block.RightUp.y ) / 2;
    }
    else if( block.Direction == LEFT )
    {
        x = block.RightUp.x;
        y = (block.LeftDown.y + block.RightUp.y ) / 2 ;
    }
    Point<int> pt(x,y);
    
    int xCoordinate = ( Vertical.find(x) != Vertical.end() ) ? (int)distance(Vertical.begin(), Vertical.find(pt.x)) + 1 : x;
    int yCoordinate = ( Horizontal.find(y) != Horizontal.end() ) ? (int)distance(Horizontal.begin(), Horizontal.find(pt.y)) + 1 : y ;
    int topLayer = RouterHelper.translateMetalNameToInt( *(--block.Metals.end()) );
//    if( xCoordinate == Vertical.size()) xCoordinate -= 1 ;
//    if( yCoordinate == Horizontal.size()) yCoordinate -= 1 ;
    return Coordinate3D(xCoordinate,yCoordinate,topLayer);
}
void RouterV4::toGridGraph()
{
    bool init = true ;
    
    for( auto row : Grids )
    {
        for( auto col : row )
        {
            if( init ){ cout<< " + FIXED" ; init = false ;}
            else cout << " NEW ";
            if( RouterHelper.IsBlock( col )) cout << " METAL5 " ;
            else cout << " METAL5 " ;
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

void RouterV4::CutGrid(double width , double spacing )
{
    Horizontal.clear();
    Vertical.clear();
    
    auto powerInfos = RouterHelper.getPowerPinInfo() ;
    auto blockPinInfos = RouterHelper.getBlockPinInfo() ;
    int x = 0 , y = 0 ;
//    for(auto powerinfo : powerInfos)
//        cout << powerinfo.LeftDown << " " << powerinfo.RightUp << " "<< powerinfo.Direction << endl;
//    cout << endl;
//    for(auto blockpininfo : blockPinInfos)
//        cout << blockpininfo.LeftDown << " " << blockpininfo.RightUp  << " " << blockpininfo.Direction << endl;
//    cout << endl;
    for(auto powerinfo : powerInfos)
    {
        if( powerinfo.Direction == TOP )
        {
            x = (powerinfo.LeftDown.x + powerinfo.RightUp.x) / 2 ;
            y = powerinfo.LeftDown.y ;
        }
        else if( powerinfo.Direction == DOWN )
        {
            x = (powerinfo.LeftDown.x + powerinfo.RightUp.x) / 2 ;
            y = powerinfo.RightUp.y ;
        }
        else if( powerinfo.Direction == RIGHT )
        {
            x = powerinfo.LeftDown.x;
            y = ( powerinfo.LeftDown.y + powerinfo.RightUp.y ) / 2;
        }
        else if( powerinfo.Direction == LEFT )
        {
            x = powerinfo.RightUp.x;
            y = (powerinfo.LeftDown.y + powerinfo.RightUp.y ) / 2 ;
        }
        if(y>0 && y <= DIEAREA.pt2.y)Horizontal.insert(y);
        if(x>0 && x <= DIEAREA.pt2.x)Vertical.insert(x);
    }
    for(auto blockpininfo : blockPinInfos)
    {
        if( blockpininfo.Direction == TOP )
        {
            x = (blockpininfo.LeftDown.x + blockpininfo.RightUp.x) / 2 ;
            y = blockpininfo.LeftDown.y ;
        }
        else if( blockpininfo.Direction == DOWN )
        {
            x = (blockpininfo.LeftDown.x + blockpininfo.RightUp.x) / 2 ;
            y = blockpininfo.RightUp.y ;
        }
        else if( blockpininfo.Direction == RIGHT )
        {
            x = blockpininfo.LeftDown.x;
            y = ( blockpininfo.LeftDown.y + blockpininfo.RightUp.y ) / 2;
        }
        else if( blockpininfo.Direction == LEFT )
        {
            x = blockpininfo.RightUp.x;
            y = (blockpininfo.LeftDown.y + blockpininfo.RightUp.y ) / 2 ;
        }
        if(y>0 && y <= DIEAREA.pt2.y)Horizontal.insert(y);
        if(x>0 && x <= DIEAREA.pt2.x)Vertical.insert(x);
    }
    
    auto blocks = RouterHelper.getBlockRectangle();
    for( auto block : blocks )
    {
        int leftX = block.LeftDown.x - (( 0.5 * width + spacing ) * UNITS_DISTANCE);
        int rightX = block.RightUp.x + (( 0.5 * width + spacing ) * UNITS_DISTANCE);
        int downY = block.LeftDown.y - (( 0.5 * width + spacing ) * UNITS_DISTANCE);
        int upY = block.RightUp.y + (( 0.5 * width + spacing ) * UNITS_DISTANCE);
        if( downY > 0 && downY <= DIEAREA.pt2.y )Horizontal.insert(downY);
        if( upY > 0 && upY <= DIEAREA.pt2.y)Horizontal.insert(upY);
        if( leftX > 0 && leftX <= DIEAREA.pt2.x)Vertical.insert(leftX);
        if( rightX > 0 && rightX <= DIEAREA.pt2.x)Vertical.insert(rightX);
    }
    for( auto key : obstacles )
    {
        for(auto block : key.second)
        {
            int leftX = block.LeftDown.x - (( 0.5 * width + spacing ) * UNITS_DISTANCE);
            int rightX = block.RightUp.x + (( 0.5 * width + spacing ) * UNITS_DISTANCE);
            int downY = block.LeftDown.y - (( 0.5 * width + spacing ) * UNITS_DISTANCE);
            int upY = block.RightUp.y + (( 0.5 * width + spacing ) * UNITS_DISTANCE);
            if( downY > 0 && downY <= DIEAREA.pt2.y )Horizontal.insert(downY);
            if( upY > 0 && upY <= DIEAREA.pt2.y)Horizontal.insert(upY);
            if( leftX > 0 && leftX <= DIEAREA.pt2.x)Vertical.insert(leftX);
            if( rightX > 0 && rightX <= DIEAREA.pt2.x)Vertical.insert(rightX);
        }
    }
    Vertical.insert(DIEAREA.pt2.x);
    Horizontal.insert(DIEAREA.pt2.y);
    
    if( *Vertical.begin() == DIEAREA.pt1.x ) Vertical.erase(Vertical.begin());
    if( *Horizontal.begin() == DIEAREA.pt1.y ) Horizontal.erase(Horizontal.begin());
    
}
void RouterV4::updateGrid(CrossInfo result , Grid & grid)
{
    for( int z = lowestMetal ; z <= highestMetal ; z++ )
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
        if( result.viaIsCross )
        {
            if( (z >= result.lowerMetal - 1 ) && ( z < result.upperMetal ) )
                grid.verticalEdges[z].topEdge = false;
            if( z > result.lowerMetal && z <= result.upperMetal + 1 )
                grid.verticalEdges[z].bottomEdge = false;
        }
    }
}
void RouterV4::InitGrids(string source , double width , double spacing , bool cutGrid, vector<int> SpecialHorizontal ,  vector<int> SpecialVertical )
{
    Grids.clear();
//    cout << "Begin Initialize  Grid Graph ..." << endl;
//    clock_t Start = clock();
    
    if(cutGrid)CutGrid(width, spacing);
    
    for(auto h : SpecialHorizontal)
        Horizontal.insert(h);
    for(auto v : SpecialVertical)
        Vertical.insert(v);
//    //    CutByBlockBoundary();
//    //    CutByUserDefine();
    Point<int> startpoint(0,0) ;
    for( auto h : Horizontal )
    {
        vector<Grid> temp ;
        for(auto v : Vertical)
        {
            Grid grid ;
            grid.capacities.resize(highestMetal+1);
            grid.Edges.resize(highestMetal+1);
            grid.verticalEdges.resize(highestMetal+1);
            Point<int> CrossPoint(v,h);
            grid.width = CrossPoint.x - startpoint.x ;
            grid.length = CrossPoint.y - startpoint.y ;
            grid.startpoint = startpoint ;
            // 判斷有沒有跟block有交叉
            Rectangle rect(grid.startpoint , Point<int>( grid.startpoint.x + grid.width , grid.startpoint.y + grid.length ));
            Rectangle via ;
            via.LeftDown.x = grid.startpoint.x - (width * UNITS_DISTANCE / 2 ) ;
            via.LeftDown.y = grid.startpoint.y - (width * UNITS_DISTANCE / 2 ) ;
            via.RightUp.x = grid.startpoint.x + (width * UNITS_DISTANCE / 2 ) ;
            via.RightUp.y = grid.startpoint.y + (width * UNITS_DISTANCE / 2 ) ;
            via.LeftDown.x -= DEFAULTSPACING * UNITS_DISTANCE ;
            via.LeftDown.y -= DEFAULTSPACING * UNITS_DISTANCE ;
            via.RightUp.x += DEFAULTSPACING * UNITS_DISTANCE ;
            via.RightUp.y += DEFAULTSPACING * UNITS_DISTANCE ;
            CrossRegion crossRegion = RouterHelper.getCrossRegion(rect);
            if( crossRegion == LeftUp )
            {
                for( auto block : RouterHelper.leftUpBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
            }
            else if (crossRegion == LeftDown)
            {
                for( auto block : RouterHelper.leftDownBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
            }
            else if( crossRegion == RightUp )
            {
                for( auto block : RouterHelper.rightUpBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
            }
            else if( crossRegion == RightDown )
            {
                for( auto block : RouterHelper.rightDownBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
            }
            else if(crossRegion == LeftUpAndLeftDown)
            {
                for( auto block : RouterHelper.leftUpBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
                for( auto block : RouterHelper.leftDownBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
            }
            else if(crossRegion == RightUpAndRightDown)
            {
                for( auto block : RouterHelper.rightUpBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
                for( auto block : RouterHelper.rightDownBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
            }
            else if( crossRegion == LeftDownAndRightDown )
            {
                for( auto block : RouterHelper.leftDownBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
                for( auto block : RouterHelper.rightDownBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
            }
            else if( crossRegion == LeftUpAndRightUp )
            {
                for( auto block : RouterHelper.leftUpBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
                for( auto block : RouterHelper.rightUpBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
            }
            else if(crossRegion == Center)
            {
                for( auto block : RouterHelper.leftUpBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
                for( auto block : RouterHelper.rightUpBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
                for( auto block : RouterHelper.leftDownBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
                for( auto block : RouterHelper.rightDownBlockMap )
                {
                    auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, via , block.second , width , spacing);
                    updateGrid(crosssWithBlockResult, grid);
                }
            }
            
            for( auto obstacle : obstacles )
            {
//                clock_t Start = clock();
//                if( source == obstacle.first ) continue ;
                for( auto o : obstacle.second )
                {
                    auto crosssWithObstacleResult = RouterHelper.isCrossWithBlock(rect , via ,o, width , spacing);
                    updateGrid(crosssWithObstacleResult, grid);
                }
//                clock_t End = clock();
//                double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
//                cout << "obstacles: We cost " << duration << "(s)" << endl;
            }
//            auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect , grid );
//            updateGrid(crosssWithBlockResult, grid);
            // 判斷有沒有跟線有交叉
//            auto crosssWithObstacleResult = RouterHelper.isCrossWithObstacle(rect, source, obstacles , grid );
//            updateGrid(crosssWithObstacleResult, grid);
            startpoint.x += (CrossPoint.x - startpoint.x) ;
            temp.push_back(grid);
        }
        startpoint.x = 0 ;
        startpoint.y = h ;
        Grids.push_back(temp);
    }
    
//    clock_t End = clock();
//    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
//    cout << "Initialize  Grid Graph Done" << endl ;
//    cout << "We cost " << duration << "(s)" << endl;
}


void RouterV4::printAllGrids()
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
vector<int> RouterV4::getAbsoluteDistance(vector<pair<Direction3D, int>> & friendlyPaths , Point<int> startPoint)
{
    vector<int> temp;
    int x = startPoint.x ;
    int y = startPoint.y ;
    for(auto path : friendlyPaths)
    {
        if( path.first == rightOrient)
        {
            int sum = 0 ;
            int progress = path.second ;
            for( int i = 0 ; i < progress ; i++ )
                sum += Grids[y][x+i].width ;
            x += progress;
            temp.push_back(sum);
        }
        else if(path.first == leftOrient)
        {
            int sum = 0 ;
            int progress = path.second ;
            for( int i = 0 ; i < progress ; i++ )
                sum += Grids[y][x-1-i].width ;
            x -= progress ;
            temp.push_back(sum);
        }
        else if( path.first == upOrient)
        {
            int sum = 0 ;
            int progress = path.second ;
            for( int i = 0 ; i < progress ; i++ )
                sum += Grids[y+i][x].length ;
            y += progress;
            temp.push_back(sum);
        }
        else if (path.first == downOrient)
        {
            int sum = 0 ;
            int progress = path.second ;
            for( int i = 0 ; i < progress ; i++ )
                sum += Grids[y-1-i][x].length ;
            y -= progress ;
            temp.push_back(sum);
        }
        else if( path.first == topOrient || path.first == bottomOrient )
        {
            temp.push_back(path.second);
        }
    }
    return temp;
}
vector<pair<Direction3D, int>> RouterV4::translateToFriendlyForm( vector<Coordinate3D> & Paths )
{
    Direction3D initDirection = topOrient ;
    if( (int)(Paths[1].x - Paths[0].x) > 0) initDirection = rightOrient ;
    if( (int)(Paths[1].x - Paths[0].x) < 0) initDirection = leftOrient ;
    if( (int)(Paths[1].y - Paths[0].y) > 0) initDirection = upOrient ;
    if( (int)(Paths[1].y - Paths[0].y) < 0) initDirection = downOrient ;
    if( (int)(Paths[1].z - Paths[0].z) > 0) initDirection = topOrient ;
    if( (int)(Paths[1].z - Paths[0].z) < 0) initDirection = bottomOrient ;
    vector<pair<Direction3D, int>> paths ;
    int cnt = 0 ;
    Direction3D currentDir = initDirection ;
    Direction3D nextDir = initDirection ;
   
    for(int i = 0 ; i < Paths.size() ; i++)
    {
        if( i + 1 >= Paths.size() )
        {
            paths.push_back(make_pair(currentDir, cnt));
            break;
        }
        if( (int)(Paths[i+1].x - Paths[i].x) > 0) nextDir = rightOrient ;
        if( (int)(Paths[i+1].x - Paths[i].x) < 0) nextDir = leftOrient ;
        if( (int)(Paths[i+1].y - Paths[i].y) > 0) nextDir = upOrient ;
        if( (int)(Paths[i+1].y - Paths[i].y) < 0) nextDir = downOrient ;
        if( (int)(Paths[i+1].z - Paths[i].z) > 0) nextDir = topOrient ;
        if( (int)(Paths[i+1].z - Paths[i].z) < 0) nextDir = bottomOrient ;
        if( currentDir == nextDir && nextDir == rightOrient ) cnt += (Paths[i+1].x - Paths[i].x);
        if( currentDir == nextDir && nextDir == leftOrient ) cnt += (Paths[i].x - Paths[i+1].x);
        if( currentDir == nextDir && nextDir == upOrient ) cnt += (Paths[i+1].y - Paths[i].y);
        if( currentDir == nextDir && nextDir == downOrient ) cnt += (Paths[i].y - Paths[i+1].y);
        if( currentDir == nextDir && nextDir == topOrient ) cnt += (Paths[i+1].z - Paths[i].z);
        if( currentDir == nextDir && nextDir == bottomOrient ) cnt += (Paths[i].z - Paths[i+1].z);
        
        if( currentDir != nextDir )
        {
            paths.push_back(make_pair(currentDir, cnt));
            cnt = 0 ;
            currentDir = nextDir ;
            i -= 1 ;
        }
    }
//    cout << "translate:" << endl;
//    for( auto p : paths )
//    {
//        if( p.first == 0 )  cout << "UP";
//        if( p.first == 1 )  cout << "DOWN";
//        if( p.first == 2 )  cout << "LEFT";
//        if( p.first == 3 )  cout << "RIGHT";
//        if( p.first == 4 )  cout << "TOP";
//        if( p.first == 5 )  cout << "BOTTOM";
//        cout << " " << p.second << endl;
//    }
//    cout << endl;
    return paths;
}
