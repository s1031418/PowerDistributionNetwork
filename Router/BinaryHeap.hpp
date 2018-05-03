//
//  BinaryHeap.hpp
//  Priorty_Queue_BinaryHeap
//
//

#ifndef BinaryHeap_hpp
#define BinaryHeap_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <set>
#include <unordered_map>
// element 為資料，key為priority
struct HeapNode {
    int element , key ;
    HeapNode():element(0),key(0){};
    HeapNode(int node , int key):element(node) , key(key){};
};

using namespace std;


class BinaryHeap {
    
public:
    BinaryHeap();
    BinaryHeap(int n );
    bool IsEmpty();
    
    // Min-Priority Queue
    void MinHeapify(int node , int length);
    void BuildMinHeap(vector<int> array);
    void BuildMinHeap(vector<HeapNode> array);
    
    void DecreaseKey(int node , int newKey);
    void MinHeapInsert(int node , int key);
    int Minimum(); // 回傳vertex位置的index
    int ExtractMin(); // 回傳vertex位置的index
private:
    vector<HeapNode> heap ;
    unordered_map<int, int> LUT;
    void swap(struct HeapNode & p1 , struct HeapNode &p2);
    void swap(int key1 , int key2);
    int FindPosition(int node );
    int GetParentNode(int node);
    
};

#endif /* BinaryHeap_hpp */
