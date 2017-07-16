//
//  RouterV4.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 05/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "RouterV4.hpp"

// bug list:
// 上下不能走，grid edge上的點
// 拉皮(wiring)
// run time obstacle




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
Graph_SP * RouterV4::InitGraph_SP()
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
    graph_sp->SetRight(Right);
    graph_sp->SetUp(Up);
    graph_sp->SetTop(Top);
    graph_sp->SetDirectionMode(false);
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
                        graph_sp->AddEdge(index, index+Right, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, WIDTH * UNITS_DISTANCE, Grids[y][x].width) * 10000  );
                        graph_sp->AddEdge(index+Right, index, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, WIDTH * UNITS_DISTANCE, Grids[y][x].width) * 10000 ) ;
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Right, Max_Distance);
                        graph_sp->AddEdge(index+Right, index, Max_Distance);
                    }
                    if( Grids[y][x-1].Edges[z].downEdge )
                    {
                        
                        graph_sp->AddEdge(index, index+Left, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, WIDTH * UNITS_DISTANCE, Grids[y][x-1].width)* 10000 );
                        graph_sp->AddEdge(index+Left, index, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, WIDTH * UNITS_DISTANCE, Grids[y][x-1].width)* 10000 );
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Left, Max_Distance);
                        graph_sp->AddEdge(index+Left, index, Max_Distance);
                    }
                    if( Grids[y][x].Edges[z].leftEdge )
                    {
                        graph_sp->AddEdge(index, index+Up, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, WIDTH * UNITS_DISTANCE, Grids[y][x].length) * 10000);
                        graph_sp->AddEdge(index+Up, index, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, WIDTH * UNITS_DISTANCE, Grids[y][x].length) * 10000);
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Up, Max_Distance);
                        graph_sp->AddEdge(index+Up, index, Max_Distance);
                    }
                    if( Grids[y-1][x].Edges[z].leftEdge )
                    {
                        
                        graph_sp->AddEdge(index, index+Down, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, WIDTH * UNITS_DISTANCE, Grids[y-1][x].length)* 10000 );
                        graph_sp->AddEdge(index+Down, index, RouterHelper.calculateResistance(LayerMaps[RouterHelper.translateIntToMetalName(z)].RESISTANCE_RPERSQ, WIDTH * UNITS_DISTANCE, Grids[y-1][x].length) * 10000);
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Down, Max_Distance);
                        graph_sp->AddEdge(index+Down, index, Max_Distance);
                    }
                    if( z == lowestMetal )
                    {
                        graph_sp->AddEdge(index, index+Top, 1* 10000);
                    }
                    else if (z == highestMetal)
                    {
                        graph_sp->AddEdge(index, index+Bottom, 1* 10000);
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Top, 1* 10000);
                        graph_sp->AddEdge(index, index+Bottom, 1* 10000);
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
Coordinate3D RouterV4::LegalizeTargetEdge(Block block ,Graph_SP * graph_sp )
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
        while( nextY - currentY !=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE + blockLength )
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
        while( currentY - nextY !=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE + blockLength )
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
        while( nextX - currentX !=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE + blockWidth )
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
        while( currentX - nextX !=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE + blockWidth )
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
void RouterV4::fillSpNetMaps( vector<Coordinate3D> & paths , string powerPinName , BlockInfo blockinfo )
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
            targetPoint.y += (WIDTH * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = WIDTH * UNITS_DISTANCE ;
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
            targetPoint.y -= (WIDTH * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = WIDTH * UNITS_DISTANCE ;
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
            targetPoint.x -= (WIDTH * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = WIDTH * UNITS_DISTANCE ;
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
            targetPoint.x += (WIDTH * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = WIDTH * UNITS_DISTANCE ;
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
                net.ABSOLUTE_POINT1 = oringinTargetPoint;
                if( layer == 1 ) net.VIANAME = "via1_C";
                if( layer == 2 ) net.VIANAME = "via2_C";
                if( layer == 3 ) net.VIANAME = "via3_C";
                if( layer == 4 ) net.VIANAME = "via4_C";
                if( layer == 5 ) net.VIANAME = "via5_C";
                nets.push_back(net);
//                cout << layer << " " << oringinTargetPoint << " " ;
//                if( layer == 1 ) cout << "via1_C" << endl;
//                if( layer == 2 ) cout << "via2_C" << endl;
//                if( layer == 3 ) cout << "via3_C" << endl;
//                if( layer == 4 ) cout << "via4_C" << endl;
//                if( layer == 5 ) cout << "via5_C" << endl;
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
                nets.push_back(net);
                // hard code via5c 而且 第六層走到第五層要用查的，目前hardcode
//                cout << layer << " " << oringinTargetPoint << " " ;
                
//                if( layer == 6 ) cout << "via5_C" << endl;
//                if( layer == 5 ) cout << "via4_C" << endl;
//                if( layer == 4 ) cout << "via3_C" << endl;
//                if( layer == 3 ) cout << "via2_C" << endl;
//                if( layer == 2 ) cout << "via1_C" << endl;
                layer--;
            }
            
        }
        //-------------------------------//
        //update source point
        startPoint = oringinTargetPoint ;
        if( nextDirection == upOrient )
        {
            startPoint.y -= WIDTH / 2 * UNITS_DISTANCE;
            
        }
        if( nextDirection == downOrient )
        {
            startPoint.y += WIDTH / 2 * UNITS_DISTANCE;
        }
        if( nextDirection == leftOrient )
        {
            startPoint.x += WIDTH / 2 * UNITS_DISTANCE;
        }
        if( nextDirection == rightOrient )
        {
            startPoint.x -= WIDTH / 2 * UNITS_DISTANCE;
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
void RouterV4::BlockGridCoordinate( Graph_SP * graph_sp , Block & block)
{
    int XSize = (int)Grids[0].size()+1;
    int Up = XSize ;
    int Right = 1 ;
    auto blockGrid = getGridCoordinate(block);
    int currentIndex = translate3D_1D(Coordinate3D(blockGrid.x,blockGrid.y,blockGrid.z));
    
    if( block.Direction == TOP )
    {
        int cnt = 1 ;
        int currentY = getAbsolutePoint(translate1D_3D(currentIndex)).y ;
        int nextY = getAbsolutePoint(translate1D_3D(currentIndex+Up)).y ;
        // get how many counts should be calcaulated.
        while( nextY - currentY <=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE )
        {
            cnt++ ;
            nextY = getAbsolutePoint(translate1D_3D(currentIndex + (cnt * Up))).y ;
        }
        for( int i = 0 ; i <= cnt ; i++ )
        {
            for(int z = lowestMetal ; z <= highestMetal ; z++)
            {
                int index = translate3D_1D(Coordinate3D(blockGrid.x, blockGrid.y, z));
                bool turnRight = ( blockGrid.x == Grids[0].size() ) ? false : true ;
                bool turnUp = ( blockGrid.y == Grids.size() ) ? false : true ;
                bool turnLeft = ( blockGrid.x == 0 ) ? false : true ;
                bool turnDown = ( blockGrid.y == 0 ) ? false : true ;
                if( turnRight && Grids[blockGrid.y][blockGrid.x].Edges[z].downEdge )
                {
                    graph_sp->UpdateWeight(index+Right, index, Max_Distance - index);
                    graph_sp->UpdateWeight(index, index+Right, Max_Distance - index);
                }
                if( turnUp && Grids[blockGrid.y][blockGrid.x].Edges[z].leftEdge )
                {
                    graph_sp->UpdateWeight(index+Up, index, Max_Distance - index);
                    graph_sp->UpdateWeight(index, index+Up, Max_Distance - index);
                }
                if( turnLeft && Grids[blockGrid.y][blockGrid.x - 1 ].Edges[z].downEdge )
                {
                    graph_sp->UpdateWeight(index-Right, index, Max_Distance - index );
                    graph_sp->UpdateWeight(index, index-Right, Max_Distance - index );
                }
                if( turnDown && Grids[blockGrid.y - 1 ][blockGrid.x].Edges[z].leftEdge )
                {
                    graph_sp->UpdateWeight(index-Up, index, Max_Distance - index );
                    graph_sp->UpdateWeight(index, index-Up, Max_Distance - index );
                }
            }
            blockGrid.y += 1 ;
        }
    }
    else if( block.Direction == DOWN )
    {
        int cnt = 1 ;
        int currentY = getAbsolutePoint(translate1D_3D(currentIndex)).y ;
        int nextY = getAbsolutePoint(translate1D_3D(currentIndex-Up)).y ;
        while( currentY - nextY <=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE )
        {
            cnt++ ;
            nextY = getAbsolutePoint(translate1D_3D(currentIndex - (cnt * Up))).y ;
        }
        for( int i = 0 ; i <= cnt ; i++ )
        {
            for(int z = lowestMetal ; z <= highestMetal ; z++)
            {
                int index = translate3D_1D(Coordinate3D(blockGrid.x, blockGrid.y, z));
                bool turnRight = ( blockGrid.x == Grids[0].size() ) ? false : true ;
                bool turnUp = ( blockGrid.y == Grids.size() ) ? false : true ;
                bool turnLeft = ( blockGrid.x == 0 ) ? false : true ;
                bool turnDown = ( blockGrid.y == 0 ) ? false : true ;
                if( turnRight && Grids[blockGrid.y][blockGrid.x].Edges[z].downEdge )
                {
                    graph_sp->UpdateWeight(index+Right, index, Max_Distance - index);
                    graph_sp->UpdateWeight(index, index+Right, Max_Distance - index);
                }
                if( turnUp && Grids[blockGrid.y][blockGrid.x].Edges[z].leftEdge )
                {
                    graph_sp->UpdateWeight(index+Up, index, Max_Distance - index);
                    graph_sp->UpdateWeight(index, index+Up, Max_Distance - index);
                }
                if( turnLeft && Grids[blockGrid.y][blockGrid.x - 1 ].Edges[z].downEdge )
                {
                    graph_sp->UpdateWeight(index-Right, index, Max_Distance - index );
                    graph_sp->UpdateWeight(index, index-Right, Max_Distance - index );
                }
                if( turnDown && Grids[blockGrid.y - 1 ][blockGrid.x].Edges[z].leftEdge )
                {
                    graph_sp->UpdateWeight(index-Up, index, Max_Distance - index );
                    graph_sp->UpdateWeight(index, index-Up, Max_Distance - index );
                }
            }
            blockGrid.y -= 1 ;
        }
    }
    else if( block.Direction == LEFT )
    {
        int cnt = 1 ;
        int currentX = getAbsolutePoint(translate1D_3D(currentIndex)).x ;
        int nextX = getAbsolutePoint(translate1D_3D(currentIndex-Right)).x ;
        while( currentX - nextX <=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE )
        {
            cnt++ ;
            nextX = getAbsolutePoint(translate1D_3D(currentIndex - (cnt * Right ) )).x ;
        }
        for( int i = 0 ; i <= cnt ; i++ )
        {
            for(int z = lowestMetal ; z <= highestMetal ; z++)
            {
                int index = translate3D_1D(Coordinate3D(blockGrid.x, blockGrid.y, z));
                bool turnRight = ( blockGrid.x == Grids[0].size() ) ? false : true ;
                bool turnUp = ( blockGrid.y == Grids.size() ) ? false : true ;
                bool turnLeft = ( blockGrid.x == 0 ) ? false : true ;
                bool turnDown = ( blockGrid.y == 0 ) ? false : true ;
                if( turnRight && Grids[blockGrid.y][blockGrid.x].Edges[z].downEdge )
                {
                    graph_sp->UpdateWeight(index+Right, index, Max_Distance - index);
                    graph_sp->UpdateWeight(index, index+Right, Max_Distance - index);
                }
                if( turnUp && Grids[blockGrid.y][blockGrid.x].Edges[z].leftEdge )
                {
                    graph_sp->UpdateWeight(index+Up, index, Max_Distance - index);
                    graph_sp->UpdateWeight(index, index+Up, Max_Distance - index);
                }
                if( turnLeft && Grids[blockGrid.y][blockGrid.x - 1 ].Edges[z].downEdge )
                {
                    graph_sp->UpdateWeight(index-Right, index, Max_Distance - index );
                    graph_sp->UpdateWeight(index, index-Right, Max_Distance - index );
                }
                if( turnDown && Grids[blockGrid.y - 1 ][blockGrid.x].Edges[z].leftEdge )
                {
                    graph_sp->UpdateWeight(index-Up, index, Max_Distance - index );
                    graph_sp->UpdateWeight(index, index-Up, Max_Distance - index );
                }
            }
            blockGrid.x -= 1 ;
        }
    }
    else if (block.Direction == RIGHT)
    {
        int cnt = 1 ;
        int currentX = getAbsolutePoint(translate1D_3D(currentIndex)).x ;
        int nextX = getAbsolutePoint(translate1D_3D(currentIndex+Right)).x ;
        while( nextX - currentX <=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE )
        {
            cnt++ ;
            nextX = getAbsolutePoint(translate1D_3D(currentIndex + (cnt * Right))).x ;
        }
        for( int i = 0 ; i <= cnt ; i++ )
        {
            for(int z = lowestMetal ; z <= highestMetal ; z++)
            {
                int index = translate3D_1D(Coordinate3D(blockGrid.x, blockGrid.y, z));
                bool turnRight = ( blockGrid.x == Grids[0].size() ) ? false : true ;
                bool turnUp = ( blockGrid.y == Grids.size() ) ? false : true ;
                bool turnLeft = ( blockGrid.x == 0 ) ? false : true ;
                bool turnDown = ( blockGrid.y == 0 ) ? false : true ;
                if( turnRight && Grids[blockGrid.y][blockGrid.x].Edges[z].downEdge )
                {
                    graph_sp->UpdateWeight(index+Right, index, Max_Distance - index);
                    graph_sp->UpdateWeight(index, index+Right, Max_Distance - index);
                }
                if( turnUp && Grids[blockGrid.y][blockGrid.x].Edges[z].leftEdge )
                {
                    graph_sp->UpdateWeight(index+Up, index, Max_Distance - index);
                    graph_sp->UpdateWeight(index, index+Up, Max_Distance - index);
                }
                if( turnLeft && Grids[blockGrid.y][blockGrid.x - 1 ].Edges[z].downEdge )
                {
                    graph_sp->UpdateWeight(index-Right, index, Max_Distance - index );
                    graph_sp->UpdateWeight(index, index-Right, Max_Distance - index );
                }
                if( turnDown && Grids[blockGrid.y - 1 ][blockGrid.x].Edges[z].leftEdge )
                {
                    graph_sp->UpdateWeight(index-Up, index, Max_Distance - index );
                    graph_sp->UpdateWeight(index, index-Up, Max_Distance - index );
                }
            }
            blockGrid.x += 1 ;
        }
    }
}
void RouterV4::BlockFrontDoor(Graph_SP * graph_sp , string sourcePowerPin)
{
    
    
    for( auto it = Connection.begin() ; it != Connection.end() ; it = Connection.upper_bound(it->first))
    {
        auto ret = Connection.equal_range(it->first);
        for (auto i = ret.first; i != ret.second; ++i)
        {
            string powerpin = i->first ;
            string block = i->second.BlockName ;
            string blockPin = i->second.BlockPinName ;
//            cout << powerpin << " " << block << " " << blockPin << endl;
            Block powerPinCoordinate = RouterHelper.getPowerPinCoordinate(powerpin);
            Block blockPinCoordinate = RouterHelper.getBlock(block,blockPin);
            auto powerGrid = getGridCoordinate(powerPinCoordinate);
            auto blockPinGrid = getGridCoordinate(blockPinCoordinate);
            int currentIndex = translate3D_1D(Coordinate3D(powerGrid.x,powerGrid.y,powerGrid.z));
            currentIndex = translate3D_1D(Coordinate3D(blockPinGrid.x,blockPinGrid.y,blockPinGrid.z));
            if( powerpin != sourcePowerPin ) BlockGridCoordinate(graph_sp,powerPinCoordinate);
            BlockGridCoordinate(graph_sp,blockPinCoordinate);
        }
    }
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
void RouterV4::getInitSolution(Block block  , string powerpin, BlockInfo blockinfo  , bool source)
{
    vector<BlockCoordinate> virtualObstacles ;
    vector<Coordinate3D> paths ;
    auto sourceGrid = getGridCoordinate(block);
    string key = blockinfo.BlockName.append(blockinfo.BlockPinName);
    if( block.Direction == TOP )
    {
        int blockLength = block.RightUp.y - block.LeftDown.y ;
        Coordinate3D targetGrid = sourceGrid ;
        int currentY = getAbsolutePoint(sourceGrid).y ;
        targetGrid.y += 1 ;
        int nextY = getAbsolutePoint(targetGrid).y ;
        // get how many counts should be calcaulated.
        while( nextY - currentY !=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE + blockLength )
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
        fillSpNetMaps(paths, powerpin, blockinfo );
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
            virtualObstacle.LeftDown.x -= WIDTH / 2 * UNITS_DISTANCE ;
            virtualObstacle.RightUp.x += WIDTH / 2 * UNITS_DISTANCE ;
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
        while( currentY - nextY !=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE + blockLength )
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
        fillSpNetMaps(paths, powerpin, blockinfo  );
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
            virtualObstacle.LeftDown.x -= WIDTH / 2 * UNITS_DISTANCE ;
            virtualObstacle.RightUp.x += WIDTH / 2 * UNITS_DISTANCE ;
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
        while( nextX - currentX !=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE + blockWidth )
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
        fillSpNetMaps(paths, powerpin, blockinfo  );
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
            virtualObstacle.LeftDown.y -= WIDTH / 2 * UNITS_DISTANCE ;
            virtualObstacle.RightUp.y += WIDTH / 2 * UNITS_DISTANCE ;
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
        while( currentX - nextX !=  (0.5 * WIDTH + SPACING)*UNITS_DISTANCE + blockWidth )
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
        fillSpNetMaps(paths, powerpin, blockinfo  );
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
            virtualObstacle.LeftDown.y -= WIDTH / 2 * UNITS_DISTANCE ;
            virtualObstacle.RightUp.y += WIDTH / 2 * UNITS_DISTANCE ;
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
void RouterV4::InitPowerPinAndBlockPin()
{
    InitGrids("");
    for( auto it = Connection.begin() ; it != Connection.end() ; it = Connection.upper_bound(it->first))
    {
        auto ret = Connection.equal_range(it->first);
        for (auto i = ret.first; i != ret.second; ++i)
        {
            string powerpin = i->first ;
            string block = i->second.BlockName ;
            string blockPin = i->second.BlockPinName ;
            BlockInfo blockinfo ;
            blockinfo.BlockName = block ;
            blockinfo.BlockPinName = blockPin ;
            Block powerPinCoordinate = RouterHelper.getPowerPinCoordinate(powerpin);
            Block blockPinCoordinate = RouterHelper.getBlock(block,blockPin);
            if( i == ret.first ) getInitSolution(powerPinCoordinate,powerpin,blockinfo,true );
            getInitSolution(blockPinCoordinate,powerpin,blockinfo,false);
        }
    }
}
void RouterV4::Route()
{
    InitPowerPinAndBlockPin();
    GlobalRouter gr ;
    auto orders = gr.getNetOrdering();
    for(auto order : orders)
    {
        obstacles.begin();
        BlockInfo blockinfo ;
        string powerpin = order.second.source ;
        blockinfo.BlockName = order.second.target.first ;
        blockinfo.BlockPinName = order.second.target.second ;
        Block powerPinCoordinate = RouterHelper.getPowerPinCoordinate(powerpin);
        Block BlockPinCoordinate = RouterHelper.getBlock(blockinfo.BlockName, blockinfo.BlockPinName);
        InitGrids(powerpin);
        Graph_SP * graph_sp = InitGraph_SP();
        Coordinate3D sourceGrid = LegalizeTargetEdge(powerPinCoordinate , graph_sp);
        Coordinate3D targetGrid = LegalizeTargetEdge(BlockPinCoordinate , graph_sp);
        int source = translate3D_1D(sourceGrid);
        int target = translate3D_1D(targetGrid);
        graph_sp->Dijkstra(source);
        auto paths = graph_sp->getPath(target);
        if(paths.empty())
        {
//            toGridGraph();
            cout << "Path" << endl;
            cout << powerpin << " " << blockinfo.BlockName << " " << blockinfo.BlockPinName << endl;
            cout << "-------------------------------Source Info-------------------------------" << endl;
            if( powerPinCoordinate.Direction == 0 ) cout << "Direction:UP" << endl;
            if( powerPinCoordinate.Direction == 1 ) cout << "Direction:DOWN" << endl;
            if( powerPinCoordinate.Direction == 2 ) cout << "Direction:RIGHT" << endl;
            if( powerPinCoordinate.Direction == 3 ) cout << "Direction:LEFT" << endl;
            cout << "Source Absolute:" << powerPinCoordinate.LeftDown << " " << powerPinCoordinate.RightUp << endl ;
            Coordinate3D source3D = translate1D_3D(source);
            cout << "Source 3D:"<< source3D.x << " " << source3D.y << " " << source3D.z << endl;
            cout << "Source 1D:" << translate3D_1D(source3D) << endl;
            cout << "-------------------------------Target Info-------------------------------" << endl;
            if( BlockPinCoordinate.Direction == 0 ) cout << "Direction:UP" << endl;
            if( BlockPinCoordinate.Direction == 1 ) cout << "Direction:DOWN" << endl;
            if( BlockPinCoordinate.Direction == 2 ) cout << "Direction:RIGHT" << endl;
            if( BlockPinCoordinate.Direction == 3 ) cout << "Direction:LEFT" << endl ;
            cout << "Target Absolute:" << BlockPinCoordinate.LeftDown << " " << BlockPinCoordinate.RightUp << endl;
            Coordinate3D target3D = translate1D_3D(target);
            cout << "Target 3D:" << target3D.x << " " << target3D.y << " " << target3D.z << endl;
            cout << "Target 1D:" << translate3D_1D(target3D) << endl;
            assert(0);
        }
        
        vector<Coordinate3D> temp ;
//        cout << "Paths" << endl;
        // from source to target
        for(int i = (int)paths.size() - 1; i >= 0  ; i--)
        {
            temp.push_back(translate1D_3D(paths[i]));
        }
//        for(auto p : paths)
//        {
//            temp.push_back(translate1D_3D(p));
////            auto s = translate1D_3D(p);
////            cout << s.x << " " << s.y << " " << s.z << endl;
//        }
        fillSpNetMaps(temp, powerpin, blockinfo );
        def_gen.toOutputDef();
        generateSpiceList(temp, powerpin, blockinfo );
        sp_gen.toSpice();
        sp_gen.addSpiceCmd();
        string cmd = "./ngspice " + spiceName + " -o simulation" ;
        system(cmd.c_str());
        ngspice ng_spice ;
        ng_spice.initvoltage();
        
        double sourceV = ng_spice.voltages[getNgSpiceKey(sourceGrid)];
        double targetV = ng_spice.voltages[getNgSpiceKey(targetGrid)];
        double Drop = (sourceV - targetV) * 100 ;
        cout << powerpin << " to " << blockinfo.BlockName << "_" << blockinfo.BlockPinName << " Drop:" << Drop << "(%) " ; 
        double constaint = RouterHelper.getIRDropConstaint(blockinfo.BlockName, blockinfo.BlockPinName);
        if( constaint >= Drop )
            cout << "Pass" << endl;
        else
            cout << "No Pass" << endl;
        cout << "IR Drop Constraint:" << constaint << "(%)"<< endl;
        cout << endl;
        delete [] graph_sp ;
        
    }
    
    
}
string RouterV4::getNgSpiceKey(Coordinate3D coordinate3d)
{
    int z = coordinate3d.z ;
    Point<int> pt = getAbsolutePoint(coordinate3d);
    string MetalName = RouterHelper.translateIntToMetalName(z) ;
    string result ;
    // to lowercase
    transform(MetalName.begin(), MetalName.end(), MetalName.begin(), ::tolower);
    result.append(MetalName).append("_").append(to_string(pt.x)).append("_").append(to_string(pt.y));
    return result ;
}
void RouterV4::Simulation()
{
    
}
double RouterV4::getMetalResistance(int layer)
{
    return LayerMaps[RouterHelper.translateIntToMetalName(layer)].RESISTANCE_RPERSQ ;
}
void RouterV4::genResistance(vector<Coordinate3D> & paths , string powerPinName)
{
    for(int i = 0 ; i < paths.size() - 1 ; i++)
    {
        string node1 = gridToString(paths[i] , true );
        string node2 = gridToString(paths[i+1] , true );
        Point<int> pt1 = getAbsolutePoint(paths[i]);
        Point<int> pt2 = getAbsolutePoint(paths[i+1]);
        // distance 0 means via
        // 目前打最大顆via (HardCode)
        int distance = ( pt1.x == pt2.x ) ? abs(pt1.y - pt2.y) : abs(pt1.x - pt2.x);
        double resistance = ( distance != 0 ) ? RouterHelper.calculateResistance(getMetalResistance(paths[i].z), WIDTH * UNITS_DISTANCE, distance) : 0.25 ;
        if(resistance < 0 ) assert(0);
        sp_gen.addSpiceResistance(powerPinName, node1, node2, resistance);
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
void RouterV4::generateSpiceList(vector<Coordinate3D> & paths , string powerPinName , BlockInfo blockinfo )
{
    string key = blockinfo.BlockName ;
    key.append(blockinfo.BlockPinName);
    auto initSourcePath = sourceTargetInitPath[powerPinName] ;
    auto initTargetPath = sourceTargetInitPath[key] ;
    sp_gen.initSpiceVdd(powerPinName, gridToString(initSourcePath[0],false), stod(VoltageMaps[powerPinName]));
    sp_gen.addSpiceCurrent(powerPinName, gridToString(initTargetPath[0],false), RouterHelper.getCurrent(blockinfo.BlockName, blockinfo.BlockPinName));
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
    genResistance(initSourcePath,powerPinName);
    genResistance(paths,powerPinName);
    genResistance(initTargetPath,powerPinName);
    
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

void RouterV4::CutGrid(int width , int spacing )
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
        if(y>0)Horizontal.insert(y);
        if(x>0)Vertical.insert(x);
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
        if(y>0)Horizontal.insert(y);
        if(x>0)Vertical.insert(x);
    }
    
    auto blocks = RouterHelper.getBlockRectangle();
    for( auto block : blocks )
    {
        int leftX = block.LeftDown.x - (( 0.5 * width + spacing ) * UNITS_DISTANCE);
        int rightX = block.RightUp.x + (( 0.5 * width + spacing ) * UNITS_DISTANCE);
        int downY = block.LeftDown.y - (( 0.5 * width + spacing ) * UNITS_DISTANCE);
        int upY = block.RightUp.y + (( 0.5 * width + spacing ) * UNITS_DISTANCE);
        if( downY > 0 )Horizontal.insert(downY);
        if( upY > 0 )Horizontal.insert(upY);
        if( leftX > 0 )Vertical.insert(leftX);
        if( rightX > 0 )Vertical.insert(rightX);
    }
    for( auto key : obstacles )
    {
        for(auto block : key.second)
        {
            int leftX = block.LeftDown.x - (( 0.5 * width + spacing ) * UNITS_DISTANCE);
            int rightX = block.RightUp.x + (( 0.5 * width + spacing ) * UNITS_DISTANCE);
            int downY = block.LeftDown.y - (( 0.5 * width + spacing ) * UNITS_DISTANCE);
            int upY = block.RightUp.y + (( 0.5 * width + spacing ) * UNITS_DISTANCE);
            if( downY > 0 )Horizontal.insert(downY);
            if( upY > 0 )Horizontal.insert(upY);
            if( leftX > 0 )Vertical.insert(leftX);
            if( rightX > 0 )Vertical.insert(rightX);
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
    }
}
void RouterV4::InitGrids(string source)
{
    Grids.clear();
//    cout << "Begin Initialize  Grid Graph ..." << endl;
//    clock_t Start = clock();
    
    CutGrid(WIDTH, SPACING);
    
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
            Point<int> CrossPoint(v,h);
            grid.width = CrossPoint.x - startpoint.x ;
            grid.length = CrossPoint.y - startpoint.y ;
            grid.startpoint = startpoint ;
            // 判斷有沒有跟block有交叉
            Rectangle rect(grid.startpoint , Point<int>( grid.startpoint.x + grid.width , grid.startpoint.y + grid.length ));
            for(auto block : RouterHelper.BlockMap)
            {
                auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, block.second , WIDTH , SPACING);
                updateGrid(crosssWithBlockResult, grid);
            }
            for( auto obstacle : obstacles )
            {
                if( source == obstacle.first ) continue ;
                for( auto o : obstacle.second )
                {
                    auto crosssWithObstacleResult = RouterHelper.isCrossWithBlock(rect,o, WIDTH , SPACING);
                    updateGrid(crosssWithObstacleResult, grid);
                }
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
        if( (int)(Paths[i+1].x - Paths[i].x) > 0)
        {
            direction = rightOrient ;
//            cnt += ( Grids[Paths[i].y][Paths[i+1].x].startpoint.x - Grids[Paths[i].y][Paths[i].x].startpoint.x);
        }
        if( (int)(Paths[i+1].x - Paths[i].x) < 0)
        {
            direction = leftOrient ;
//            cnt += ( Grids[Paths[i].y][Paths[i].x].startpoint.x - Grids[Paths[i].y][Paths[i].x-1].startpoint.x);
        }
        if( (int)(Paths[i+1].y - Paths[i].y) > 0)
        {
            direction = upOrient ;
        }
        if( (int)(Paths[i+1].y - Paths[i].y) < 0)
        {
            direction = downOrient ;
        }
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
