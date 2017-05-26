//
//  Graph_SP.hpp
//  Dijkstra
//
//  Created by 吳峻陞 on 26/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Graph_SP_hpp
#define Graph_SP_hpp
#include <iomanip>
#include <stdio.h>
#include <list>
#include "BinaryHeap.hpp"
#include <climits>

const int Max_Distance = INT_MAX ;
class Graph_SP // SP means Shortest Path
{
    
public:
    Graph_SP();
    Graph_SP(int n);
    
    void AddEdge(int from , int to , int weight);
    void PrintDataArray(vector<int> array);
    void PrintIntArray(int * array);
    
    void InitalizeSingleSource(int Start);
    void Relax(int from , int to , int weight);
    
    void Dijkstra(int Start = 0);
    
    friend class BinaryHeap ;
    
private:
    int num_vertex ;
    vector<list<pair<int, int>>> AdjList ;
    vector<int> predecessor , distance ;
    
};

#endif /* Graph_SP_hpp */
