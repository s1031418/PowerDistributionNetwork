//
//  DetailRouter.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 13/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef DetailRouter_hpp
#define DetailRouter_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "RouterUtil.hpp"
#include <map>
#include <set>
#include "GlobalRouter.hpp"
#include "Components.h"
using namespace std;



class RouteConstraint
{
public:
    vector<int> AllowLayers ; // 允許走第幾層layer
    vector<DIRECTION> AllowDirections ;
    
};
class DetailRouter
{
    
public:
    DetailRouter();
    ~DetailRouter();
    // Model為3D的Graph
    void InitGrids();
    
    
    
    
    void InitGraph_SP( Graph_SP & graph_sp );
    
    void DetailRoute();
    
    // 依照Pitch切uniform的Grid
    void CutByPitch(double pitch , set<int> & Horizontal , set<int> & Vertical );
    
    
    void printGlobalSols();
    
    
private:
    map<string ,vector< vector< Grid > >> MLGrids ; // MutiLayer Grids
    RouterUtil RouteHelper ; 
    GlobalRouter gr ;
};

#endif /* DetailRouter_hpp */
