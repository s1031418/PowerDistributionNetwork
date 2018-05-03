//
//  BinaryHeap.cpp
//  Priorty_Queue_BinaryHeap
//
//

#include "BinaryHeap.hpp"


void BinaryHeap::swap(int key1 , int key2)
{
    auto temp = LUT[key1];
    LUT[key1] = LUT[key2];
    LUT[key2] = temp ;
}

void BinaryHeap::swap(struct HeapNode & p1 , struct HeapNode &p2)
{
    struct HeapNode temp = p1 ;
    p1 = p2 ;
    p2 = temp ;
}
int BinaryHeap::FindPosition(int node )
{
    if( LUT.find(node) == LUT.end() )
        return -1;
    return LUT[node];
}
int BinaryHeap::GetParentNode(int node)
{
    return floor(node/2);
}
BinaryHeap::BinaryHeap()
{
    heap.resize(1);
}
BinaryHeap::BinaryHeap(int n )
{
    heap.resize(n + 1);
}
bool BinaryHeap::IsEmpty()
{
    return (heap.size() == 1);
}
void BinaryHeap::MinHeapify(int node , int length)
{
    int left = 2 * node ;
    int right = 2 * node + 1;
    int smallest = 0 ;
    if( left <= length && heap[left].key < heap[node].key )
        smallest = left ;
    else
        smallest = node ;
    if( right <= length && heap[right].key < heap[smallest].key )
        smallest = right ;
    if( smallest != node )
    {
        swap(heap[smallest].element, heap[node].element);
        swap(heap[smallest], heap[node]);
        MinHeapify(smallest, length);
    }
    
        
}
void BinaryHeap::BuildMinHeap(vector<int> array)
{
    for(int i = 0 ; i < array.size() ; i++)
    {
        heap[ i + 1 ].element = i ;
        heap[ i + 1 ].key = array[i] ;
        LUT.insert(make_pair( i  , i + 1));
    }
    for( int i = (int)heap.size()/2 ; i >= 1 ; i-- )
        MinHeapify(i, (int)heap.size() -1 );
}
void BinaryHeap::BuildMinHeap(vector<HeapNode> array)
{
    for(int i = 0 ; i < array.size() ; i++)
    {
        heap[ i + 1 ].element = array[i].element ;
        heap[ i + 1 ].key = array[i].key ;
    }
    for( int i = (int)heap.size()/2 ; i >= 1 ; i-- )
        MinHeapify(i, (int)heap.size() -1 );
}
void BinaryHeap::DecreaseKey(int node , int newKey)
{
    int index_node = FindPosition(node);      // 找到node所在的位置index
    if( index_node == -1 ) return; 
    if (newKey > heap[index_node].key) {      // 如果不是把node的Key下修, 便終止此函式
//        std::cout << "new key is larger than current key\n";
        return;
    }
    heap[index_node].key = newKey;            // 更新node之Key後, 需要檢查是否新的subtree滿足Min Heap
    while (index_node > 1 && heap[GetParentNode(index_node)].key > heap[index_node].key) {
        swap(heap[index_node].element, heap[GetParentNode(index_node)].element);
        swap(heap[index_node], heap[GetParentNode(index_node)]);
        index_node = GetParentNode(index_node);
    }
    
        
}
void BinaryHeap::MinHeapInsert(int node , int key)
{
    heap.push_back(HeapNode(node,key));    // 在heap[]尾巴新增一個node
    
    DecreaseKey(node, key);
}
int BinaryHeap::Minimum()
{
    return heap[1].element ;
}
int BinaryHeap::ExtractMin()
{
    if (IsEmpty()) {
        std::cout << "error: heap is empty\n";
        exit(-1);
    }
    int min = heap[1].element;    // 此時heap的第一個node具有最小key值
    LUT[ heap[heap.size()-1].element ] = 1 ;
    LUT.erase(heap[1].element);
    
    // 便以min記錄其element, 最後回傳min
    // delete the first element/vertex
    heap[1] = heap[heap.size()-1];            // 把最後一個element放到第一個位置,
    
    heap.erase(heap.begin()+heap.size()-1);   // 再刪除最後一個element
    
    MinHeapify(1, (int)heap.size());          // 目前, heap[1]具有最大Key, 需要進行調整
    
    return min;       // 回傳heap中具有最小key的element
}
