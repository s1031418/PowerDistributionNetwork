//
//  Predictor.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 24/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Predictor_hpp
#define Predictor_hpp

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
#include "GlobalRouter.hpp"
//enum TwoDGridLocation
//{
//    vertex_leftdown ,
//    vertex_leftup ,
//    vertex_rightdown ,
//    vertex_rightup ,
//    border_left ,
//    border_right ,
//    border_top ,
//    border_bottom ,
//    other_2D
//};
//enum ThreeDGridLocation
//{
//    top ,
//    bottom ,
//    other_3D
//};
//
//
//struct Edge {
//    int to ;
//    int weight ;
//};
//

class Predictor {
    
public:
    Predictor();
    ~Predictor();
    // estimate the width of the min ir drop constraint
    void estimate();
    
    map<double,Path> getNetOrdering();
    
private:
    
    // variable
    vector< vector< Grid > > Grids ;
    RouterUtil RouterHelper;
    Graph_SP graph;
    set<int> Vertical ;
    set<int> Horizontal ;
    unsigned lowest ; // lowest metal layer
    unsigned highest ; // highest metal layer
    
    
    // funciton
    
    // create 2D vector grid 
    void InitGrids();
    
    // Cut By BlockBoundary ( non-uniform )
    void CutByBlockBoundary();
    
    // Initialize Graph_SP and model 3D coordinate
    void InitGraph_SP();
    
    // calculate the point belong which grid 
    pair<int, int> getGridCoordinate( Point<int> pt );
    
    // translate 3D coordinate to 1D coordinate
    int translate3D_1D(int x , int y , int z);
    
    // translate 1D coordinate to 3D coordinate
    tuple<int,int,int> translate1D_3D(int index);
    
    // translate 2D coordinate to 1D coordinate
    int translate2D_1D(int x , int y);
    
    // translate 1D coordinate to 2D coordinate 
    pair<int, int> translate1D_2D(int index);
    
    // esitimate the solution of dijkstra algorithm of length 
    int getLength(vector<int> & path);
    
    // correspond the index , calculate the 3D location of this point
    ThreeDGridLocation get3DGridLocation(int z);
    
    // correspond the index , calculate the 2D location of this point
    TwoDGridLocation get2DGridLocation(int x , int y);
    
    // get 3D 2D location of this point 
    pair<TwoDGridLocation, ThreeDGridLocation> getGridLocation(int x , int y , int z );
    
    // cost function for Graph_SP edge weights
    unsigned cost(int z , bool horizontal , Grid & grid);
    
    // get Approximate ManhattanDistance of two block , correspond their center point
    int getApproximateManhattanDistance(Block block1 , Block block2 );
    
    // get ManhattanDistance of two points
    int getManhattanDistance(Point<int> pt1 , Point<int> pt2);
    
    
};


#endif /* Predictor_hpp */



