//
//  Graph_SP.cpp
//  Dijkstra
//
//  Created by 吳峻陞 on 26/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "Graph_SP.hpp"



Graph_SP::Graph_SP()
    :num_vertex(0)
{
    DirectionMode = false ;
}
Graph_SP::Graph_SP(int n)
    :num_vertex(n)
{
    AdjList.resize(num_vertex);
    DirectionMode = false ; 
}
void Graph_SP::AddEdge(int from , int to , int weight)
{
    for( auto list : AdjList[from] )
    {
        if( list.first == to )
            return ;
    }
    AdjList[from].push_back(make_pair(to, weight));
}
void Graph_SP::PrintDataArray(vector<int> array)
{
    for(int i = 0 ; i < num_vertex ; i++)
        cout << setw(4) << i ;
    
    cout << endl;
    for(int i = 0 ; i < num_vertex ; i++)
        cout << setw(4) << array[i];
    cout << endl;
}
void Graph_SP::PrintIntArray(int * array)
{
    
}

void Graph_SP::InitalizeSingleSource(int Start)
{
     
    distance.resize(num_vertex);
    predecessor.resize(num_vertex);
    turns.resize(num_vertex);
    predecessorTurns.resize(num_vertex);
    for(int i = 0 ; i < num_vertex ;i++)
    {
        distance[i] = Max_Distance ;
        predecessor[i] = -1;
        turns[i] = 0;
        predecessorTurns[i] = Init ;
    }
    distance[Start] = 0 ;
    predecessorTurns[Start] = TurnUp;
}
TurnDirection Graph_SP::getTurnDirection(int diff)
{
    if( diff == right ) return TurnRight;
    else if( diff == up ) return TurnUp;
    else if( diff == (-1*right) ) return TurnLeft;
    else if( diff == (-1*up) ) return TurnDown;
    else if ( diff == top ) return TurnTop ;
    else if ( diff == -1*(top) )return TurnBottom ;
    else assert(0);
}

void Graph_SP::Relax(int from , int to , int weight)
{
    
    if(DirectionMode)
    {
        int diff = to - from ;
        TurnDirection turndir = getTurnDirection(diff);
        if( turndir == TurnTop || turndir == TurnBottom )
            predecessorTurns[to] = predecessorTurns[from];
        else
            predecessorTurns[to] = turndir ;
        if( predecessorTurns[from] != TurnTop && predecessorTurns[from] != TurnBottom &&
           predecessorTurns[to] != TurnTop && predecessorTurns[from] != TurnBottom &&
           predecessorTurns[from] != predecessorTurns[to] ) turns[to]++;
        if( distance[to] > distance[from] + weight  )
        {
            distance[to] = distance[from] + weight  ;
            predecessor[to] = from ;
        }
        else if(  distance[to] == distance[from] + weight && turns[to] > turns[from] )
        {
            predecessor[to] = from ;  
        }
    }
    else
    {
        if( distance[to] > distance[from] + weight  )
        {
            distance[to] = distance[from] + weight ;
            predecessor[to] = from ;
        }
    }
}

vector<int> Graph_SP::getPath( int target )
{
    
    vector<int> Paths ;
    int pre = predecessor[target];
    if( pre == -1 )
    {
        cout << "No Solution" << endl;
        return vector<int>();
    }
    Paths.push_back(target);
    while( pre != -1 )
    {
        Paths.push_back(pre);
        pre = predecessor[pre];
    }
    
//    cout << "Shortest Path:" << endl;
//    
//    for( int i = (int)Paths.size()-1 ; i >= 0 ; i-- )
//        cout << setw(4) << Paths[i]  ;
//    cout << setw(4) << target << endl;
    return Paths ;
}
int Graph_SP::getShortestPath(int target)
{
    
    return 0;
}
void Graph_SP::Demo()
{
    Graph_SP g9(6);
//    Graph_SP g9;
    g9.AddEdge(0, 1, 8);g9.AddEdge(0, 5, 1);
    g9.AddEdge(1, 0, 3);g9.AddEdge(1, 2, 1);
    g9.AddEdge(2, 0, 5);g9.AddEdge(2, 3, 2);g9.AddEdge(2, 4, 2);
    g9.AddEdge(3, 1, 4);g9.AddEdge(3, 2, 6);g9.AddEdge(3, 4, 7);g9.AddEdge(3, 5, 3);
    g9.AddEdge(5, 3, 2);g9.AddEdge(5, 4, 8);
    
    g9.Dijkstra(0);
}
void Graph_SP::resize(int n )
{
    num_vertex = n ;
    AdjList.resize(num_vertex);
}
void Graph_SP::UpdateWeight( int from , int to , int newWeight )
{
    for(auto it = AdjList[from].begin() ; it != AdjList[from].end() ; it++)
    {
        if( it->first == to )
            it->second = newWeight ; 
    }
}
void Graph_SP::SetRight(int Right)
{
    right = Right;
}
void Graph_SP::SetUp(int Up)
{
    up = Up ;
}

void Graph_SP::SetDirectionMode(bool mode)
{
    DirectionMode = mode ;
}
void Graph_SP::SetTop(int Top)
{
    top = Top ;
}
void Graph_SP::Dijkstra(int Start )
{
    InitalizeSingleSource(Start);
    BinaryHeap minQueue(num_vertex);
    minQueue.BuildMinHeap(distance);
    while (!minQueue.IsEmpty())
    {
        int u = minQueue.ExtractMin();
        for(auto it = AdjList[u].begin() ; it != AdjList[u].end() ; it++)
        {
            Relax(u, it->first, it->second);
            minQueue.DecreaseKey(it->first, distance[it->first]);
        }
    }
//    cout << "print predecessor:" << endl;
//    PrintDataArray(predecessor);
//    cout << "print distance:" << endl;
//    PrintDataArray(distance);
}
