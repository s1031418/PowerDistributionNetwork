//
//  Tree.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 11/08/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "Graph.hpp"


Graph::Graph()
{
    root = new Vertex[1];
}


Graph::~Graph()
{
    destoryGraph();
}

void Graph::destoryGraph()
{
//    for(auto vertex : LUT)
//    {
//        if( vertex.second->fanIn != nullptr ) delete [] vertex.second->fanIn ;
//        for(auto fanout : vertex.second->fanOut)
//            delete [] fanout ;
//        delete [] vertex.second ;
//    }
}
void Graph::initialize(Coordinate3D source , double current , double length)
{
    string encodeString = encode(source);
    Vertex * vertex = new Vertex[1];
    vertex->current = current ;
    vertex->coordinate = source ;
    vertex->length = length ;
    vertex->fanIn = nullptr ;
    root = vertex ;
    LUT.insert(make_pair(encodeString, vertex));
}
vector<Coordinate3D> Graph::getPath(Coordinate3D target)
{
    stack<Coordinate3D> reversePaths ;
    vector<Coordinate3D> paths;
    string encodeString = encode(target);
    Vertex * ptr = LUT[encodeString];
    while (ptr->fanIn != nullptr)
    {
        reversePaths.push(ptr->coordinate);
        ptr = ptr->fanIn ;
    }
    reversePaths.push(ptr->coordinate);
    while (!reversePaths.empty())
    {
        paths.push_back( reversePaths.top() ) ;
        reversePaths.pop();
    }
    return paths ;
}
void Graph::insert(Coordinate3D source , Coordinate3D target , double current , double length )
{
    string encodeString = encode(source);
    if( LUT.find(encodeString) == LUT.end() )
        assert(0);
    Vertex * frontVertex = LUT[encodeString] ;
    Vertex * nextVertex = new Vertex[1];
    nextVertex->coordinate = target ;
    nextVertex->current = current;
    nextVertex->length = length ;
    nextVertex->fanIn = frontVertex ;
    frontVertex->fanOut.push_back(nextVertex);
    encodeString = encode(target);
    LUT.insert(make_pair(encodeString, nextVertex));
}
void Graph::reduction()
{
    for(auto fanout : root->fanOut)
        reduceVertex(root , fanout);
}
void Graph::reduceVertex(Vertex * front , Vertex * next)
{
    Vertex * ptr1 = front ;
    Vertex * ptr2 = next;
    double length = 0 ;
    for(int i = 0; i < ptr1->fanOut.size() ; i++)
    {
        if( ptr1->fanOut[i] == next )
            ptr1->fanOut.erase(ptr1->fanOut.begin() + i); 
    }
    while( ptr2->fanOut.size() < 2 )
    {
        length += ptr2->length ;
        if( ptr2->fanOut.size() == 0 ) break ; 
        ptr2 = ptr2->fanOut[0];
        Vertex * tmp = ptr2->fanIn ;
        tmp->fanOut.clear();
        delete [] tmp ;
    }
    ptr1->fanOut.push_back(ptr2);
    ptr1->length = length;
    ptr2->fanIn = ptr1 ;
    for(auto fanout : ptr2->fanOut)
    {
        reduceVertex(ptr2 , fanout);
    }
}
string Graph::encode(Coordinate3D coordinate)
{
    // 預期 xString , yString , zString size 不會超過9，也就是size <= 9
    string xString = to_string(coordinate.x);
    string yString = to_string(coordinate.y);
    string zString = to_string(coordinate.z);
    assert(xString.size() < 9);
    assert(xString.size() < 9);
    assert(xString.size() < 9);
    string encodeString ;
    // x encoding
    encodeString.append(to_string(xString.size())).append(xString);
    // y encoding
    encodeString.append(to_string(yString.size())).append(yString);
    // z encoding
    encodeString.append(to_string(zString.size())).append(zString);
    return encodeString ;
}
Coordinate3D Graph::decode(string encodeString)
{
    int xSize = stoi(encodeString.substr(0,1));
    int x = stoi(encodeString.substr(1,xSize));
    int ySize = stoi(encodeString.substr(1+xSize,1));
    int y = stoi(encodeString.substr(1+xSize+1 , ySize));
    int zSize = stoi(encodeString.substr(1+xSize+1+ySize,1));
    int z = stoi(encodeString.substr(1+xSize+1+ySize+1,zSize));
    return Coordinate3D (x,y,z) ; ;
}
