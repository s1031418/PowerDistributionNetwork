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

#include "InitialFileParser.hpp"
#include "verilog.hpp"
#include "lefrw.h"
#include "defrw.h"
#include "flute_net.hpp"
#include "RouterUtil.hpp"
#include "RouterComponents.hpp"


enum TwoDGridLocation
{
    vertex_leftdown ,
    vertex_leftup ,
    vertex_rightdown ,
    vertex_rightup ,
    border_left ,
    border_right ,
    border_top ,
    border_bottom ,
    other_2D
};
enum ThreeDGridLocation
{
    top ,
    bottom ,
    other_3D
};


struct Edge {
    int to ;
    int weight ;
};


class Router {
    
public:
    Router();
    ~Router();
    void Route();
    vector< vector< Grid > > Grids ;
    set<int> boundList ;
private:
    // variable:
    RouterUtil RouterHelper;
    Graph_SP graph;
    set<int> Vertical ;
    set<int> Horizontal ;
    void InitGraph_SP( Graph_SP & graph_sp );
    unsigned lowest ; // lowest metal layer
    unsigned highest ; // highest metal layer
    
    void printAllGrids();
    void InitGrids();
    
    Coordinate3D translate1D_3D(int index);
    void toGridGraph();
    
    
    int translate3D_1D(Coordinate3D coordinate3d);
    
    void CutByUserDefine();
    
    
    
};

#endif /* Router_hpp */
