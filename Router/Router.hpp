//
//  Router.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 27/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Router_hpp
#define Router_hpp

#include <stdio.h>
#include "lefrw.h"
#include "defrw.h"
#include "Graph_SP.hpp"
#include "PDNHelper.hpp"
#include "Converter.hpp"
#include "InitialFileParser.hpp"
#include "verilog.hpp"
#include "lefrw.h"
#include "defrw.h"
#include "flute_net.hpp"
#include "RouterUtil.hpp"
#include "RouterComponents.hpp"
#include "GlobalRouter.hpp"



//struct Edge {
//    int to ;
//    int weight ;
//};

struct SBox {
    Point<int> LeftDown ;
    Point<int> RightUp ;
    int lowestMetalLayer ;
    int highestMetalLayer ;
};

enum RelativeDirection
{
    N ,
    S ,
    W ,
    E ,
    NE ,
    NW ,
    SE ,
    SW
};

class Router {
    
public:
    Router();
    ~Router();
    void Route();
    vector< vector< Grid > > Grids ;
    set<int> boundList ;
    
    vector<pair<Direction3D, int>> translateToFriendlyForm( vector<Coordinate3D> & Paths );
    
    
private:
    // variable:
    RouterUtil RouterHelper;
    Graph_SP graph;
    set<int> Vertical ;
    set<int> Horizontal ;
    Graph_SP *  InitGraph_SP( SBox sbox , DIRECTION preferDirection  );
    unsigned lowest ; // lowest metal layer
    unsigned highest ; // highest metal layer
    // record the front door point
    map<string,Coordinate3D> SpecialPoints ;
    
    //key為powerSource Name ， value 為 source曾走過的路
    // 如果有multi target的情況，利用這個table讓原本不可以走的路變成可以走的
    map<string,set<int> > historyTable;
    
    
    void CutGrid();
    
    void fixSource();
    
    void fixTarget();
    
    void InitSpPoints();
    
    void printAllGrids();
    
    void InitBoundList();
    
    void InitGrids();
    
    int getTurnCount(vector<int> paths);
    
    Coordinate3D translate1D_3D(int index);
    
    void fillSpNetMaps( vector<Coordinate3D> & paths , string powerPinName , BlockInfo blockinfo );
    
    void toGridGraph();
    
    int getCandidates( Block block );
    
    void updateGrids(vector<int> paths);
    
    void updateGrids(vector<int> paths , Graph_SP & graph_sp ) ;
    
    int translate3D_1D(Coordinate3D coordinate3d);
    
    void CutByUserDefine();
    
    
    RelativeDirection getRelativeDirection( Block b1 , Block b2 );
    
};

#endif /* Router_hpp */
