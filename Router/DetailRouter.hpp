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

using namespace std;
class DetailRouter
{
    
public:
    DetailRouter();
    ~DetailRouter();
    // Model為3D的Graph 
    void InitGrids();
    void DetailRoute();
    
    void CutByPitch(double pitch , set<int> & Horizontal , set<int> & Vertical );
    
private:
    map<string ,vector< vector< Grid > >> MLGrids ; // MutiLayer Grids
    RouterUtil RouteHelper ; 
    GlobalRouter gr ;
};

#endif /* DetailRouter_hpp */
