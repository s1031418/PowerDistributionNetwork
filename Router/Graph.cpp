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
Vertex * Graph::getVertex(Coordinate3D coordinate)
{
    string encodeString = encode(coordinate);
    return LUT[encodeString];
}
void Graph::destoryGraph()
{
//    for(auto & vertex : LUT)
    for(auto it = LUT.begin() ; it != LUT.end() ; ++it)
    {
//        if( vertex.second->fanIn != nullptr ) delete [] vertex.second->fanIn ;
//        for(auto fanout : vertex.second->fanOut)
//            delete [] fanout ;
        delete [] it->second;
    }
    LUT.clear();
}
void Graph::initialize(Coordinate3D source )
{
    string encodeString = encode(source);
    Vertex * vertex = new Vertex[1];
    vertex->coordinate = source ;
    vertex->fanIn = nullptr ;
    root = vertex ;
    LUT.insert(make_pair(encodeString, vertex));
}
vector<Vertex *> Graph::getPath(Coordinate3D target)
{
    stack<Vertex *> reversePaths ;
    vector<Vertex *> paths;
    string encodeString = encode(target);
    Vertex * ptr = LUT[encodeString];
    while (ptr->fanIn != nullptr)
    {
        reversePaths.push(ptr);
        ptr = ptr->fanIn ;
    }
    reversePaths.push(ptr);
    while (!reversePaths.empty())
    {
        paths.push_back( reversePaths.top() ) ;
        reversePaths.pop();
    }
    return paths ;
}
void Graph::rectifyCurrent()
{
    for(auto leaf : leafs)
    {
        Vertex * ptr = leaf ;
        string encodeString = encode(ptr->coordinate) ;
        double current = CUT[encodeString];
        while ( ptr != nullptr )
        {
            ptr->current += current ;
            ptr = ptr->fanIn ;
        }
    }
}
void Graph::erase(vector<Coordinate3D> coordinates)
{
    for(auto encodeString : history)
    {
        auto it = LUT.find(encodeString) ;
        it->second->fanIn->fanOut.pop_back();
        if( !LUT[encodeString]->isSteiner )
        {
            Vertex * vertex = LUT[encodeString];
            delete [] vertex ; 
            LUT.erase(it);
        }
    }
    history.clear();
    leafs.pop_back();
    leafQueue.pop();
}
double Graph::getTotalMetalUsage()
{
    double TotalMetalWeight = 0 ;
    auto allPaths = traverse();
    for( auto leaf : allPaths )
    {
        for(auto leafPath : leaf)
        {
            if( leafPath->length != 0 )
            {
                int layer = leafPath->coordinate.z ;
                double weight = stod(WeightsMaps[RouterHelper.getAlias(RouterHelper.translateIntToMetalName(layer))]);
                TotalMetalWeight += leafPath->length * 10000 /UNITS_DISTANCE / UNITS_DISTANCE * weight;
            }
        }
    }
    return TotalMetalWeight;
}
double Graph::analysis()
{
    rectifyCurrent();
    rectifyWidth();
    double cost = 0 ;
    double totalMetalUsage = getTotalMetalUsage();
    // penalty constant
    int penaltyParameter = 10000000 ;
    // 超過 2% 會有penalty
    double penaltyRange = 1 ;
    for( auto leaf : leafs )
    {
        Vertex * ptr = leaf ;
        // 一開始長出來那個金屬的voltage drop
        // double targetVR = ptr->current * LayerMaps[RouterHelper.translateIntToMetalName(ptr->coordinate.z)].RESISTANCE_RPERSQ * ;
        string encodeString = encode(ptr->coordinate) ;
        double constraint = COT[encodeString];
        double sourceV = VT[encodeString];
        double realDrop = 0 ;
        auto leafPaths = getPath(leaf->coordinate);
        // backtrace
        for(int i = (int)leafPaths.size() - 1 ; i >= 0 ; i--)
        {
            ptr = leafPaths[i] ;
//            cout << ptr->current << endl;
            int layer = leafPaths[i]->coordinate.z ;
            double RPERSQ = LayerMaps[RouterHelper.translateIntToMetalName(layer)].RESISTANCE_RPERSQ ;
            // 假設width = 10 ;
            realDrop += leafPaths[i]->current * RPERSQ * leafPaths[i]->length / 10000;
            Vertex * fanIn = leafPaths[i]->fanIn ;
            if( fanIn == nullptr ) break;
            bool calulatedVia = ( ptr->coordinate.z != fanIn->coordinate.z ) ? true : false ;
            if( calulatedVia )
            {
                string key = ( ptr->coordinate.z > fanIn->coordinate.z ) ? RouterHelper.translateIntToMetalName(--layer) : RouterHelper.translateIntToMetalName(layer);
                // hard code 第一顆via 未來要改成動態
//                int width = RouterHelper.ViaInfos[key][0].width ;
//                int length = RouterHelper.ViaInfos[key][0].length ;
//                int viaArea = width * length ;
                realDrop += leafPaths[i]->current * RouterHelper.ViaInfos[key][0].resistance / 100;
            }
        }
        double targetV = sourceV - realDrop ;
        double drop = (sourceV - targetV) / sourceV * 100 ;
        if( constraint < drop )
        {
            double diff = drop - constraint ;
            int penaltyCount = diff / penaltyRange ;
            cost += ( diff + penaltyCount ) * penaltyParameter ;
        }
        
    }
    cost += totalMetalUsage ;
    return cost / UNITS_DISTANCE / UNITS_DISTANCE;
}
void Graph::addLeafInfo(Coordinate3D leaf , string powerPin , string block , string blockPin)
{
    string encodeString = encode(leaf); 
    LeafInfo leafInfo ;
    leafInfo.powerPin = powerPin ;
    leafInfo.block = block ;
    leafInfo.blockPin = blockPin ;
    leafQueue.push(leafInfo);
}
LeafInfo Graph::getLeafInfo()
{
    LeafInfo temp = leafQueue.front() ;
    leafQueue.pop();
    return temp ; 
}
vector<vector<Vertex *>> Graph::traverse()
{
    vector<vector<Vertex *>> traversePath ;
    map<Vertex *,bool> traverseTable ; // Traverse Table( query vertex is being traverse or not )
    for(auto leaf : leafs)
    {
        vector<Vertex * > leafPath ;
        stack<Vertex *> reversePaths ;
        string encodeString = encode(leaf->coordinate);
        Vertex * ptr = LUT[encodeString];
        while (ptr->fanIn != nullptr)
        {
            if( traverseTable.find(ptr) != traverseTable.end() )
            {
                break;
            }
            reversePaths.push(ptr);
            traverseTable.insert(make_pair(ptr, true));
            ptr = ptr->fanIn ;
        }
        reversePaths.push(ptr);
        while (!reversePaths.empty())
        {
            leafPath.push_back( reversePaths.top() ) ;
            reversePaths.pop();
        }
        traversePath.push_back(leafPath);
    }
    return traversePath ;
}
vector<vector<Vertex *>> Graph::getAllPath()
{
    vector<vector<Vertex *>> allPaths ;
    for(auto leaf : leafs)
    {
        vector<Vertex *> path = getPath(leaf->coordinate);
        allPaths.push_back(path);
    }
    return allPaths; 
}
void Graph::printAllPath()
{
    for(auto leaf : leafs)
    {
        LeafInfo leafInfo = getLeafInfo();
        vector<Vertex *> path = getPath(leaf->coordinate);
        cout << "coordinate:" << endl ;
        for(auto p : path)
        {
            cout << "("<< p->coordinate.x << "," << p->coordinate.y << "," << p->coordinate.z << ")" << "->" ;
            cout << "(" << p->current << "," << p->width << ")->"  ;
//            cout << "current:" << p->current << endl;
//            cout << "length:" << p->length << endl;
//            cout << "width:" << p->width << endl;
            
         }
        cout << leafInfo.powerPin << " " << leafInfo.block << " " << leafInfo.blockPin ;
        cout << endl;
    }
}
pair<double, double> Graph::getQuadraticEquation(double a , double b , double c )
{
    double solution1 , solution2;
    solution1=((-b)+sqrt(b*b-4*a*c))/(2*a);
    solution2=((-b)-sqrt(b*b-4*a*c))/(2*a);
    return make_pair(solution1, solution2);
}
void Graph::reset()
{
    for(auto lut : LUT)
    {
        lut.second->current = 0 ;
        lut.second->width = 0 ;
        lut.second->isSteiner = false; 
    }
}
void Graph::rectifyWidth()
{
    
    for(auto leaf : leafs)
    {
        Vertex * ptr = leaf ;
        // 一開始長出來那個金屬的voltage drop
        // double targetVR = ptr->current * LayerMaps[RouterHelper.translateIntToMetalName(ptr->coordinate.z)].RESISTANCE_RPERSQ * ;
        string encodeString = encode(ptr->coordinate) ;
        double constraint = COT[encodeString];
        double allowDrop = VT[encodeString] * (constraint ) / 100 ;
        double firstCoefficient = allowDrop ;
        double secodeCoefficient = 0 ;
        double constant = 0 ;
        while ( true )
        {
            double current = ptr->current ;
            double length = ptr->length ;
            int layer = ptr->coordinate.z ;
            double RPERSQ = LayerMaps[RouterHelper.translateIntToMetalName(layer)].RESISTANCE_RPERSQ ;
            secodeCoefficient += current * RPERSQ * length ;
            Vertex * fanIn = ptr->fanIn ;
            if( fanIn == nullptr )
            {
                break;
            }
            bool calulatedVia = ( ptr->coordinate.z != fanIn->coordinate.z ) ? true : false ;
            if( calulatedVia )
            {
                string key = ( ptr->coordinate.z > fanIn->coordinate.z ) ? RouterHelper.translateIntToMetalName(--layer) : RouterHelper.translateIntToMetalName(layer);
                // hard code 第一顆via 未來要改成動態
                int width = RouterHelper.ViaInfos[key][0].width ;
                int length = RouterHelper.ViaInfos[key][0].length ;
                int viaArea = width * length ;
                constant += current * viaArea * RouterHelper.ViaInfos[key][0].resistance ;
//                for( auto via : RouterHelper.ViaInfos[key] )
//                {
//                    int width = via.width;
//                    int length = via.length ;
//                    int viaArea = width * length ;
//                    constant += current * viaArea * via.resistance ;
//                }
            }
            ptr = ptr->fanIn ;
        }
        
        auto solutions = getQuadraticEquation(firstCoefficient, -1 * secodeCoefficient, constant);
        double width = (solutions.first > solutions.second) ? solutions.first : solutions.second ;
        int unitLength = MANUFACTURINGGRID * UNITS_DISTANCE ;
        if( (int)width % unitLength != 0)
        {
            int length = width / unitLength ;
            width = unitLength * (length + 1);
        }
        if( (int)width % (2 * unitLength ) != 0 ) width += unitLength ;
//        width += UNITS_DISTANCE;
        // 將線寬回傳
        ptr = leaf ;
        while ( ptr != nullptr )
        {
            if( ptr->width < width ) ptr->width = width ;
            ptr = ptr->fanIn ;
        }
    }
}
vector<LeafInfo> Graph::getLeafInfos()
{
    vector<LeafInfo> leafInfos ;
    auto temp = leafQueue ;
    while( !temp.empty() )
    {
        leafInfos.push_back(temp.front());
        temp.pop();
    }
    return leafInfos ; 
}
void Graph::addLeaf(Coordinate3D leaf , double constraint , double current , double voltage)
{
    string encodeString = encode(leaf);
    COT.insert(make_pair(encodeString, constraint));
    CUT.insert(make_pair(encodeString, current));
    VT.insert(make_pair(encodeString, voltage));
    leafs.push_back(LUT[encodeString]);
}
void Graph::setSimulationMode(bool isSimulation)
{
    simulationMode = isSimulation; 
}
void Graph::insert(Coordinate3D source , Coordinate3D target, double length )
{
    if( simulationMode )
    {
        string targetEncodeString = encode(target);
        if( LUT.find(targetEncodeString) == LUT.end() )
        {
            history.push_back(targetEncodeString);
        }
    }
    string encodeString = encode(source);
    if( LUT.find(encodeString) == LUT.end() )
        assert(0);
    Vertex * frontVertex = LUT[encodeString] ;
    Vertex * nextVertex = new Vertex[1];
    nextVertex->coordinate = target ;
    nextVertex->length = length ;
    nextVertex->fanIn = frontVertex ;
    bool insert = true;
    if(nextVertex == NULL) assert(0);
    for(int i = 0 ; i < frontVertex->fanOut.size() ; i++)
    {
        if( frontVertex->fanOut[i] == NULL ) assert(0);
        if(frontVertex->fanOut[i]->coordinate == nextVertex->coordinate )
            insert = false ;
    }
    if(insert) frontVertex->fanOut.push_back(nextVertex);
    encodeString = encode(target);
    LUT.insert(make_pair(encodeString, nextVertex));
}
void Graph::setSteriner(Coordinate3D coordinate)
{
    string encodeString = encode(coordinate);
    Vertex * vertex = LUT[encodeString];
    vertex->isSteiner = true ; 
}
void Graph::reduction()
{
    for(auto leaf : leafs)
    {
        Vertex * node = leaf;
        Vertex * ptr = leaf ;
        double sumOfLenth = 0 ;
        while ( true )
        {
            sumOfLenth += ptr->length ;
            Vertex * fanIn = ptr->fanIn ;
            if( fanIn == nullptr )
            {
                if( node != ptr )
                {
                    node->fanIn = ptr ;
                    node->length = sumOfLenth ;
                }
                break ;
            }
            if( fanIn->isSteiner || fanIn->coordinate.z != ptr->coordinate.z )
            {
                // reduction
                node->fanIn = fanIn ;
                node->length = sumOfLenth ;
                sumOfLenth = 0 ;
                node = fanIn;
            }
            ptr = fanIn ;
        }
    }
//    for(auto fanout : root->fanOut)
//        reduceVertex(root , fanout);
}
//void Graph::reduceVertex(Vertex * front , Vertex * next)
//{
//    Vertex * ptr1 = front ;
//    Vertex * ptr2 = next;
//    double length = 0 ;
//    for(int i = 0; i < ptr1->fanOut.size() ; i++)
//    {
//        if( ptr1->fanOut[i] == next )
//            ptr1->fanOut.erase(ptr1->fanOut.begin() + i); 
//    }
//    while( ptr2->fanOut.size() < 2 )
//    {
//        length += ptr2->length ;
//        if( ptr2->fanOut.size() == 0 ) break ; 
//        ptr2 = ptr2->fanOut[0];
//        Vertex * tmp = ptr2->fanIn ;
//        tmp->fanOut.clear();
//        delete [] tmp ;
//    }
//    ptr1->fanOut.push_back(ptr2);
//    ptr1->length = length;
//    ptr2->fanIn = ptr1 ;
//    for(auto fanout : ptr2->fanOut)
//    {
//        reduceVertex(ptr2 , fanout);
//    }
//}
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
