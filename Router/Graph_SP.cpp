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
    
}
Graph_SP::Graph_SP(int n)
    :num_vertex(n)
{
    AdjList.resize(num_vertex);
    
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
    
    for(int i = 0 ; i < num_vertex ;i++)
    {
        distance[i] = Max_Distance ;
        predecessor[i] = -1;
        
        
    }
    distance[Start] = 0 ;
}

void Graph_SP::Relax(int from , int to , int weight)
{
    if( distance[to] > distance[from] + weight  )
    {
        distance[to] = distance[from] + weight ;
        predecessor[to] = from ;
    }
}
void Graph_SP::Prim(int Start)
{
    
}
vector<int> Graph_SP::getPath(  )
{
    
    vector<int> Paths ;
    int pre = predecessor[terminal];
    if( pre == -1 )
    {
        cout << "No Solution" << endl;
        return vector<int>();
    }
    Paths.push_back(terminal);
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
    return distance[target];
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
    
//    g9.Dijkstra(0);
    
}
void Graph_SP::resize(int n )
{
    num_vertex = n ;
    AdjList.resize(num_vertex);
}
void Graph_SP::UpdateWeight( int from , int to , int newWeight )
{
    for(auto it = AdjList[from].begin() ; it != AdjList[from].end() ; ++it)
    {
        if( it->first == to )
            it->second = newWeight ; 
    }
}

void Graph_SP::Dijkstra(int Start , int Target)
{
    source = Start ;
    terminal = Target ; 
    InitalizeSingleSource(Start);
    BinaryHeap minQueue(num_vertex);
    minQueue.BuildMinHeap(distance);
    while (!minQueue.IsEmpty())
    {
        int u = minQueue.ExtractMin();
        if( u == Target ) break;
        for(auto it = AdjList[u].begin() ; it != AdjList[u].end() ; ++it)
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
