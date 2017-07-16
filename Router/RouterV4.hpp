//
//  RouterV4.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 05/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef RouterV4_hpp
#define RouterV4_hpp

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
#include "SpiceGenerator.hpp"
using namespace std;



class RouterV4 {
    
    
public:
    RouterV4(string spice, string def , string output);
    
    ~RouterV4();
    
    void Route();
    
    vector< vector< Grid > > Grids ;
private:
    string spiceName ;
    string defName ;
    string outputfilesName ;
    set<int> Vertical ;
    set<int> Horizontal ;
    int lowestMetal ;
    int highestMetal ;
    int WIDTH = 10 ;
    int SPACING = 10 ;
    set<int> boundList ;
    Converter converter;
    SpiceGenerator sp_gen ; 
    map<string , Coordinate3D> MagicPoints ;
    
    
    map<string,vector<Coordinate3D>> sourceTargetInitPath;
    
    // key: vdd_source 
    map<string,vector<BlockCoordinate>> obstacles;
    
    RouterUtil RouterHelper;
    
    void fillSpNetMaps( vector<Coordinate3D> & paths , string powerPinName , BlockInfo blockinfo  );
    
    Coordinate3D LegalizeTargetEdge(Block block , Graph_SP * graph_sp);
    
    Point<int> getAbsolutePoint( Coordinate3D coordinate3d );
    
    Coordinate3D getGridCoordinate( Block block );
    
    void InitBoundList();
    
    void generateSpiceList(vector<Coordinate3D> & paths , string powerPinName , BlockInfo blockinfo );
    
    string gridToString(Coordinate3D);
    
    void AddVirtualObstacle();
    
    void BlockFrontDoor(Graph_SP * graph_sp , string sourcePowerPin);
    
    void BlockGridCoordinate( Graph_SP * graph_sp , Block & block);
    
    void InitPowerPinAndBlockPin();
    
    void getInitSolution(Block powerBlock  , string powerpin, BlockInfo blockinfo , bool source );
    
    void InitState();
    
    void updateGrid(CrossInfo result , Grid  & grid );
    
    void CutGrid(int width , int spacing);
    
    void InitGrids(string source );
    
    Graph_SP * InitGraph_SP();
    
    int translate3D_1D(Coordinate3D coordinate3d);
    
    Coordinate3D translate1D_3D(int index);
    
    void toGridGraph();
    
    void printAllGrids();
    
    vector<pair<Direction3D, int>> translateToFriendlyForm( vector<Coordinate3D> & Paths );
    
    vector<int> getAbsoluteDistance(vector<pair<Direction3D, int>> & friendlyPaths , Point<int> startPoint);
};

#endif /* RouterV4_hpp */
