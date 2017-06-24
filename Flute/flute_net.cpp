//
//  flute_net.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 02/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "flute_net.hpp"




flute_net::flute_net()
{
    
}

flute_net::~flute_net()
{
    
}
void flute_net::printSteinerTree()
{
    
    for(int i = 0 ; i < SteinerTree.size() ; i++)
    {
        if(SteinerTree[i].isSteiner) cout << "s" ;
        cout << i << ":" << setw(3) <<  "x=" << SteinerTree[i].x << setw(3) << "y=" << SteinerTree[i].y << setw(3)<< "e=" << SteinerTree[i].target << endl;
    }
    printf("FLUTE wirelength = %d\n", length);
    printf("FLUTE wirelength (without RSMT construction) = %d\n", length_noRSMT);
}
int flute_net::getShortestPathLength(vector<Point<int>> & orders)
{
    int shortestPath = 0 ;
    for( int i = 0 ; i < orders.size()   ; i++ )
    {
        shortestPath += getManhattanDistance(orders[i], orders[i+1]);
    }
    return shortestPath ;
}
vector<Point<int>> flute_net::getShortestPathOrder( Point<int> source , Point<int> target )
{
    printSteinerTree();
    vector<Point<int>> orders ;
    auto index = getSourceTargetIdx(source, target);
    cout << "source:" << index.first << endl;
    cout << "target:" << index.second << endl;
    Graph_SP graph((int)SteinerTree.size()) ;
    for(int i = 0 ; i < SteinerTree.size() ; i++)
    {
        if( i != SteinerTree[i].target )
        {
            graph.AddEdge(i, SteinerTree[i].target, 1);
            graph.AddEdge(SteinerTree[i].target, i , 1);
        }
    }
    graph.Dijkstra(index.first);
    vector<int> path = graph.getPath(index.second);
    
    for(int i = (int)path.size() - 1 ; i >= 0 ; i--)
    {
        cout << path[i] << "->" ;
        Point<int> pt( (int)SteinerTree[ path[i] ].x , (int)SteinerTree[ path[i] ].y) ;
        orders.push_back(pt);
    }
    cout << endl;
    return orders ;
//    if( SteinerTree.size() == 2 )
//    {
//        return length ;
//    }
//    else
//    {
//        
//        auto index = getSourceTargetIdx(source, target);
////        cout << "Source Index:" << index.first << endl;
////        cout << "Target Index:" << index.second << endl;
//        Graph_SP graph((int)SteinerTree.size()) ;
//        for(int i = 0 ; i < SteinerTree.size() ; i++)
//        {
//            if( i != SteinerTree[i].target )
//            {
//                graph.AddEdge(i, SteinerTree[i].target, 1);
//                graph.AddEdge(SteinerTree[i].target, i , 1);
//            }
//        }
//        graph.Dijkstra(index.first);
//        vector<int> path = graph.getPath(index.second);
//        int shortestPath = 0 ;
//        for(int i = (int)path.size() - 1 ; i > 0 ; i--)
//        {
//            shortestPath += getManhattanDistance(Point<int>(SteinerTree[path[i]].x , SteinerTree[path[i]].y), Point<int>(SteinerTree[path[i-1]].x , SteinerTree[path[i-1]].y));
//        }
//        return shortestPath ;
////        cout << shortestPath << endl;
////        cout << endl;
////        printSteinerTree();
//    }
//    assert(0);
//    return 1;
}
unsigned flute_net::getManhattanDistance(Point<int> pt1 , Point<int> pt2)
{
    return abs(pt1.x - pt2.x) + abs(pt1.y - pt2.y) ;
}
pair<unsigned, unsigned> flute_net::getSourceTargetIdx(Point<int> source , Point<int> target)
{
    int SIdx = 0 ; // Source index
    int TIdx = 0 ; // Target index
    int Index = 0 ;
    for( auto node : SteinerTree )
    {
        if( node.x == source.x && node.y == source.y  )
            SIdx = Index ;
        if( node.x == target.x && node.y == target.y)
            TIdx = Index;
        Index++ ;
    }
    return make_pair(SIdx, TIdx);
}
void flute_net::getSteinerTree(vector<Point<int>> Points)
{
    SteinerTree.clear();
    int d=0;
    int x[MAXD], y[MAXD];
    Tree flutetree;
    for( auto pt : Points )
    {
        x[d] = pt.x ;
        y[d] = pt.y ;
        d++ ;
    }
    readLUT();
    
    flutetree = flute(d, x, y, ACCURACY);
    gettree(flutetree, SteinerTree);
    length = flutetree.length;
    length_noRSMT = flute_wl(d, x, y, ACCURACY);
    
}
void flute_net::printDebugMeg()
{
    for(int i = 0 ; i < SteinerTree.size() ; i++)
    {
        if(SteinerTree[i].isSteiner)
        {
            cout << "s" ;
            cout << "NEW METAL5 ";
        }
        else
        {
            cout << "NEW METAL6 ";
        }
        cout << "1000 ( " << (long)SteinerTree[i].x << " " << (long)SteinerTree[i].y  << " ) " ;
        cout << "( " << (long)SteinerTree[i].x << " " << (long)SteinerTree[i].y + 100  << " ) " << endl;
    }
}
void flute_net::Demo()
{
    flute_net flute;
    vector<Point<int>> Points;
    // VDD4 NOBLOCK
    Points.push_back(Point<int>(1200000,0));
    Points.push_back(Point<int>(2684860,181800));
    Points.push_back(Point<int>(3006765,230300));
    Points.push_back(Point<int>(3149025,529000));
    Points.push_back(Point<int>(2421200,654200));
    Points.push_back(Point<int>(2459675,1060700));
    Points.push_back(Point<int>(2834545,1048300));
    
    
    
    
    
    
    // VDD4
//    Points.push_back(Point<int>(1200000,0));
//    Points.push_back(Point<int>(2684860,181800));
//    Points.push_back(Point<int>(3006765,230300));
//    Points.push_back(Point<int>(3149025,529000));
//    Points.push_back(Point<int>(2421200,654200));
//    Points.push_back(Point<int>(2459675,1060700));
//    Points.push_back(Point<int>(2834545,1048300));
//    Points.push_back(Point<int>(2584400, 181800));
//    Points.push_back(Point<int>(2784300, 181800));
//    Points.push_back(Point<int>(2584400, 381710));
//    Points.push_back(Point<int>(2784300, 381710));
//    Points.push_back(Point<int>(2906800, 230300));
//    Points.push_back(Point<int>(3106800, 230300));
//    Points.push_back(Point<int>(3106800, 430300));
//    Points.push_back(Point<int>(2906800, 430300));
//    Points.push_back(Point<int>(2320840, 454300));
//    Points.push_back(Point<int>(2520695, 454300));
//    Points.push_back(Point<int>(2320840, 654200));
//    Points.push_back(Point<int>(2520660, 654200));
//    Points.push_back(Point<int>(3051300, 529000));
//    Points.push_back(Point<int>(3251290, 529000));
//    Points.push_back(Point<int>(3251285, 728720));
//    Points.push_back(Point<int>(3051300, 728720));
//    Points.push_back(Point<int>(2736905, 848400));
//    Points.push_back(Point<int>(2936790, 848400));
//    Points.push_back(Point<int>(2737130, 1048300));
//    Points.push_back(Point<int>(2936725, 1048300));
//    Points.push_back(Point<int>(2357435, 1060700));
//    Points.push_back(Point<int>(2557500, 1060700));
//    Points.push_back(Point<int>(2357435, 860700));
//    Points.push_back(Point<int>(2557500, 860700));
    
    
    
    
    
    // VDD3
//    Points.push_back(Point<int>(1000000,0));
//    Points.push_back(Point<int>(636340,498175));
//    Points.push_back(Point<int>(699985,1135410));
//    Points.push_back(Point<int>(767275,1444640));
    
    flute.getSteinerTree(Points);
    flute.printDebugMeg();
    flute.printSteinerTree();
}
