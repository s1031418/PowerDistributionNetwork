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
    cout << "print predecessor:" << endl;
    PrintDataArray(predecessor);
    cout << "print distance:" << endl;
    PrintDataArray(distance);
}
