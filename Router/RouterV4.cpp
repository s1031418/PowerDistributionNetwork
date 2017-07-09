//
//  RouterV4.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 05/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "RouterV4.hpp"
// bug: block 還是要加大在判斷 + minmal space + 1/2 *width 不然會short
// legalize target pin 不能只加一個單位，要判斷加到超過 1/2 *width+ minmal space




RouterV4::RouterV4()
{
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
    cout << "";
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
    cout << "Begin Initialize 3D Shortest Path Graph ..." << endl;
    clock_t Start = clock();
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
                int weights = RouterHelper.getWeights(z);
                if( boundList.find(index) == boundList.end() )
                {
                    if( Grids[y][x].Edges[z].downEdge )
                    {
                        graph_sp->AddEdge(index, index+Right, Grids[y][x].width / UNITS_DISTANCE * weights );
                        graph_sp->AddEdge(index+Right, index, Grids[y][x].width / UNITS_DISTANCE* weights ) ;
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Right, Max_Distance);
                        graph_sp->AddEdge(index+Right, index, Max_Distance);
                    }
                    if( Grids[y][x-1].Edges[z].downEdge )
                    {
                        graph_sp->AddEdge(index, index+Left, Grids[y][x-1].width/ UNITS_DISTANCE* weights );
                        graph_sp->AddEdge(index+Left, index, Grids[y][x-1].width/ UNITS_DISTANCE* weights);
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Left, Max_Distance);
                        graph_sp->AddEdge(index+Left, index, Max_Distance);
                    }
                    if( Grids[y][x].Edges[z].leftEdge )
                    {
                        graph_sp->AddEdge(index, index+Up, Grids[y][x].length/ UNITS_DISTANCE* weights);
                        graph_sp->AddEdge(index+Up, index, Grids[y][x].length/ UNITS_DISTANCE* weights);
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Up, Max_Distance);
                        graph_sp->AddEdge(index+Up, index, Max_Distance);
                    }
                    if( Grids[y-1][x].Edges[z].leftEdge )
                    {
                        graph_sp->AddEdge(index, index+Down, Grids[y-1][x].length/ UNITS_DISTANCE* weights);
                        graph_sp->AddEdge(index+Down, index, Grids[y-1][x].length/ UNITS_DISTANCE* weights);
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Down, Max_Distance);
                        graph_sp->AddEdge(index+Down, index, Max_Distance);
                    }
                    if( z == lowestMetal )
                    {
                        graph_sp->AddEdge(index, index+Top, 1);
                    }
                    else if (z == highestMetal)
                    {
                        graph_sp->AddEdge(index, index+Bottom, 1);
                    }
                    else
                    {
                        graph_sp->AddEdge(index, index+Top, 1);
                        graph_sp->AddEdge(index, index+Bottom, 1);
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
Point<int> RouterV4::getAbsolutePoint( Coordinate3D coordinate3d )
{
    int x = coordinate3d.x ;
    int y = coordinate3d.y ;
    int X = ( x != 0 ) ? *std::next(Vertical.begin(), x-1) : 0 ;
    int Y = ( y != 0 ) ? *std::next(Horizontal.begin(), y-1) : 0 ;
    return Point<int>(X,Y);
}
void RouterV4::LegalizeTargetEdge(Block coordinate ,Graph_SP * graph_sp )
{
    auto target3D = getGridCoordinate(coordinate) ;
    int currentIndex = translate3D_1D(Coordinate3D(target3D.x,target3D.y,target3D.z));
    int YSize = (int)Grids.size()+1 ;
    int XSize = (int)Grids[0].size()+1;
    int Up = XSize ;
    int Right = 1 ;
    int Top = (XSize) * (YSize);
    
    if( coordinate.Direction == TOP )
    {
        int cnt = 1 ;
        int currentY = getAbsolutePoint(translate1D_3D(currentIndex)).y ;
        int nextY = getAbsolutePoint(translate1D_3D(currentIndex+Up)).y ;
        while( nextY - currentY <= 0.5 * WIDTH + SPACING )
        {
            cnt++ ;
            nextY = getAbsolutePoint(translate1D_3D(currentIndex + (cnt * Up))).y ;
        }
        for( int i = 0 ; i < cnt ; i++ )
            graph_sp->UpdateWeight(currentIndex+Up, currentIndex, Grids[target3D.y][target3D.x].length);
    }
    else if(coordinate.Direction == DOWN)
    {
        int cnt = 1 ;
        int currentY = getAbsolutePoint(translate1D_3D(currentIndex)).y ;
        int nextY = getAbsolutePoint(translate1D_3D(currentIndex-Up)).y ;
        while( currentY - nextY <= 0.5 * WIDTH + SPACING )
        {
            cnt++ ;
            nextY = getAbsolutePoint(translate1D_3D(currentIndex - (cnt * Up))).y ;
        }
        for( int i = 0 ; i < cnt ; i++ )
            graph_sp->UpdateWeight(currentIndex-Up, currentIndex, Grids[target3D.y-1][target3D.x].length);
    }
    else if( coordinate.Direction == RIGHT )
    {
        int cnt = 1 ;
        int currentX = getAbsolutePoint(translate1D_3D(currentIndex)).x ;
        int nextX = getAbsolutePoint(translate1D_3D(currentIndex+Right)).x ;
        while( nextX - currentX <= 0.5 * WIDTH + SPACING )
        {
            cnt++ ;
            nextX = getAbsolutePoint(translate1D_3D(currentIndex + (cnt * Right))).y ;
        }
        for( int i = 0 ; i < cnt ; i++ )
            graph_sp->UpdateWeight(currentIndex+Right, currentIndex, Grids[target3D.y][target3D.x].width);
    }
    else if( coordinate.Direction == LEFT )
    {
        int cnt = 1 ;
        int currentX = getAbsolutePoint(translate1D_3D(currentIndex)).x ;
        int nextX = getAbsolutePoint(translate1D_3D(currentIndex-Right)).x ;
        while( currentX - nextX <= 0.5 * WIDTH + SPACING )
        {
            cnt++ ;
            nextX = getAbsolutePoint(translate1D_3D(currentIndex - (cnt * Right ) )).y ;
        }
        for( int i = 0 ; i < cnt ; i++ )
            graph_sp->UpdateWeight(currentIndex-Right, currentIndex, Grids[target3D.y][target3D.x-1].width);
    }
    // target point is not allow use via
    if(target3D.z == highestMetal)
    {
        graph_sp->UpdateWeight(currentIndex-Top, currentIndex, Max_Distance);
    }
    else if (target3D.z == lowestMetal)
    {
        graph_sp->UpdateWeight(currentIndex+Top, currentIndex, Max_Distance);
    }
    else
    {
        graph_sp->UpdateWeight(currentIndex+Top, currentIndex, Max_Distance);
        graph_sp->UpdateWeight(currentIndex-Top, currentIndex, Max_Distance);
    }
    
}
void RouterV4::fillSpNetMaps( vector<Coordinate3D> & paths , string powerPinName , BlockInfo blockinfo )
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
    auto Distance = getAbsoluteDistance(friendlyForm, Point<int>(source.x,source.y));
    for(auto d : Distance)
        cout << d << endl;
    for( int i = 0 ; i < friendlyForm.size() ; i++ )
    {
        Direction3D diection = friendlyForm[i].first ;
        Direction3D nextDirection = friendlyForm[i+1].first ;
        int progress = friendlyForm[i].second ;
        if( diection == upOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x , oringinTargetPoint.y + Distance[i]);
            targetPoint = oringinTargetPoint ;
            if( i != friendlyForm.size() -1 )targetPoint.y += (DEFAULT_WIDTH * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = DEFAULT_WIDTH * UNITS_DISTANCE ;
            net.SHAPE = "STRIPE";
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            BlockCoordinate blockCoordinate ;
            blockCoordinate.LeftDown = startPoint ;
            blockCoordinate.LeftDown.x -= net.ROUNTWIDTH / 2 ;
            blockCoordinate.RightUp = targetPoint ;
            blockCoordinate.RightUp.x += net.ROUNTWIDTH / 2 ;
            blockCoordinate.lowerMetal = layer ;
            blockCoordinate.upperMetal = layer ;
            if( obstacles.find(powerPinName) == obstacles.end() ) obstacles.insert(make_pair(powerPinName, vector<BlockCoordinate>()));
            obstacles[powerPinName].push_back(blockCoordinate);
            cout << layer << " " << startPoint << " " << targetPoint << endl;
        }
        else if( diection == downOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x , oringinTargetPoint.y - Distance[i]);
            targetPoint = oringinTargetPoint ;
            if( i != friendlyForm.size() -1 )targetPoint.y -= (DEFAULT_WIDTH * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = DEFAULT_WIDTH * UNITS_DISTANCE ;
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            net.SHAPE = "STRIPE";
            BlockCoordinate blockCoordinate ;
            blockCoordinate.RightUp = startPoint ;
            blockCoordinate.RightUp.x += net.ROUNTWIDTH / 2 ;
            blockCoordinate.LeftDown = targetPoint ;
            blockCoordinate.LeftDown.x -= net.ROUNTWIDTH / 2 ;
            blockCoordinate.lowerMetal = layer ;
            blockCoordinate.upperMetal = layer ;
            if( obstacles.find(powerPinName) == obstacles.end() ) obstacles.insert(make_pair(powerPinName, vector<BlockCoordinate>()));
            obstacles[powerPinName].push_back(blockCoordinate);
            cout << layer << " " << startPoint << " " << targetPoint << endl;
        }
        else if( diection == leftOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x - Distance[i], oringinTargetPoint.y );
            targetPoint = oringinTargetPoint ;
            if( i != friendlyForm.size() -1 )targetPoint.x -= (DEFAULT_WIDTH * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = DEFAULT_WIDTH * UNITS_DISTANCE ;
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            net.SHAPE = "STRIPE";
            BlockCoordinate blockCoordinate ;
            blockCoordinate.RightUp = startPoint ;
            blockCoordinate.RightUp.y += net.ROUNTWIDTH / 2 ;
            blockCoordinate.LeftDown = targetPoint ;
            blockCoordinate.LeftDown.y -= net.ROUNTWIDTH / 2 ;
            blockCoordinate.lowerMetal = layer ;
            blockCoordinate.upperMetal = layer ;
            if( obstacles.find(powerPinName) == obstacles.end() ) obstacles.insert(make_pair(powerPinName, vector<BlockCoordinate>()));
            obstacles[powerPinName].push_back(blockCoordinate);
            cout << layer << " " << startPoint << " " << targetPoint << endl;
        }
        else if( diection == rightOrient )
        {
            oringinTargetPoint = Point<int>( oringinTargetPoint.x + Distance[i], oringinTargetPoint.y );
            targetPoint = oringinTargetPoint ;
            if( i != friendlyForm.size() -1 )targetPoint.x += (DEFAULT_WIDTH * UNITS_DISTANCE / 2) ;
            net.METALNAME = RouterHelper.translateIntToMetalName(layer);
            net.ROUNTWIDTH = DEFAULT_WIDTH * UNITS_DISTANCE ;
            net.ABSOLUTE_POINT1 = startPoint ;
            net.ABSOLUTE_POINT2 = targetPoint ;
            net.SHAPE = "STRIPE";
            BlockCoordinate blockCoordinate ;
            blockCoordinate.LeftDown = startPoint ;
            blockCoordinate.LeftDown.y -= net.ROUNTWIDTH / 2 ;
            blockCoordinate.RightUp = targetPoint ;
            blockCoordinate.RightUp.y += net.ROUNTWIDTH / 2 ;
            blockCoordinate.lowerMetal = layer ;
            blockCoordinate.upperMetal = layer ;
            if( obstacles.find(powerPinName) == obstacles.end() ) obstacles.insert(make_pair(powerPinName, vector<BlockCoordinate>()));
            obstacles[powerPinName].push_back(blockCoordinate);
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
    
    SpecialNetsMaps.insert(make_pair(powerPinName, specialnet));
    Converter converter("TEST");
    converter.toOutputDef();
}

void RouterV4::Route()
{
    
    GlobalRouter gr ;
    auto orders = gr.getNetOrdering();
    for(auto order : orders)
    {
        
        BlockInfo blockinfo ;
        string powerpin = order.second.source ;
        blockinfo.BlockName = order.second.target.first ;
        blockinfo.BlockPinName = order.second.target.second ;
        
        
        Block powerPinCoordinate = RouterHelper.getPowerPinCoordinate(powerpin);
        Block BlockPinCoordinate = RouterHelper.getBlock(blockinfo.BlockName, blockinfo.BlockPinName);
        
        InitGrids(powerpin);
        
        Graph_SP * graph_sp = InitGraph_SP();
        LegalizeTargetEdge(BlockPinCoordinate , graph_sp);
        int source = translate3D_1D(getGridCoordinate(powerPinCoordinate));
        int target = translate3D_1D(getGridCoordinate(BlockPinCoordinate));
                               
        Grids.begin();
        
        
        graph_sp->Dijkstra(source);
        auto paths = graph_sp->getPath(target);
        if(paths.empty())
        {
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
        cout << "Paths" << endl;
        for(auto p : paths)
        {
            temp.push_back(translate1D_3D(p));
            auto s = translate1D_3D(p);
            cout << s.x << " " << s.y << " " << s.z << endl;
        }
        fillSpNetMaps(temp, powerpin, blockinfo);
        delete [] graph_sp ;
        cout << "";
    }
    
    
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
    cout << "Begin Initialize  Grid Graph ..." << endl;
    clock_t Start = clock();
    
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
                auto crosssWithBlockResult = RouterHelper.isCrossWithBlock(rect, block.second);
                updateGrid(crosssWithBlockResult, grid);
            }
            for( auto obstacle : obstacles )
            {
                if( source == obstacle.first ) continue ;
                for( auto o : obstacle.second )
                {
                    auto crosssWithObstacleResult = RouterHelper.isCrossWithBlock(rect,o);
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
    clock_t End = clock();
    double duration = (End - Start) / (double)CLOCKS_PER_SEC ;
    cout << "Initialize  Grid Graph Done" << endl ;
    cout << "We cost " << duration << "(s)" << endl;
    
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
