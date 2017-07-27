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
#include "InitialFileParser.hpp"
#include "verilog.hpp"
#include "lefrw.h"
#include <algorithm>
#include "defrw.h"
#include "../Parsers/ngspice.hpp"
#include "flute_net.hpp"
#include "RouterUtil.hpp"
#include "RouterComponents.hpp"
#include "GlobalRouter.hpp"
#include "DefGenerator.hpp"
#include "SpiceGenerator.hpp"
#include "OutputFilesGenerator.hpp"
using namespace std;


struct RoutingPath {
    string sourceName;
    Coordinate3D sourceCoordinate ;
    string targetBlockName ;
    string targetBlockPinName; 
    Coordinate3D targetCoordinate ;
};

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
    SpiceGenerator sp_gen ;
    DefGenerator def_gen ;
    OutputFilesGenerator output_gen ;  
    // 絕對座標＋GridZ
    vector<RoutingPath> currentRoutingLists ;
    
    map<string,vector<Coordinate3D>> multiPinCandidates;
    
    map<string , Coordinate3D> MagicPoints ;
    
    map<string,vector<Coordinate3D>> sourceTargetInitPath;
    
    // key: vdd_source 
    map<string,vector<BlockCoordinate>> obstacles;
    
    RouterUtil RouterHelper;
    
    string getNgSpiceKey(Coordinate3D coordinate3d);
    
    void fillSpNetMaps( vector<Coordinate3D> & paths , string powerPinName , BlockInfo blockinfo , bool peek );
    
    Coordinate3D LegalizeTargetEdge(Block block , Graph_SP * graph_sp);
    
    double getMetalResistance(int layer);
    
    Point<int> getAbsolutePoint( Coordinate3D coordinate3d );
    
    void Simulation();
    
    // absolute point to grid point
    int getGridX(int x);
    
    int getGridY(int y);
    
    Coordinate3D getGridCoordinate( Block block );
    
    void InitBoundList();
    
    void BlockTopBottom(Graph_SP * graph_sp);
    
    void genResistance(vector<Coordinate3D> & paths , string powerPinName , SpiceGenerator & sp_gen );
    
    void generateSpiceList(vector<Coordinate3D> & paths , string powerPinName , BlockInfo blockinfo );
    
    string gridToString(Coordinate3D , bool);
    
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
    
    // 傳入grid座標，轉成絕對座標存起來
    void saveMultiPinCandidates(string powerPin , vector<Coordinate3D> solutions );
    
    bool isMultiPin(string powerPin);
    
    void legalizeAllOrient(Coordinate3D coordinate , Graph_SP * graph_sp);
    
    vector<Coordinate3D> selectPath(string powerPin , Graph_SP * graph_sp , int target , int source , BlockInfo blockinfo);
    
    Coordinate3D getLastIlegalCoordinate(Direction3D orient , Coordinate3D sourceGrid);
    
    void legalizeEdge(Coordinate3D source , Coordinate3D target , Direction3D orient , Graph_SP * graph_sp);
    
    void saveRoutingList(Coordinate3D sourceGrid , Coordinate3D targetGrid , string powerPin , BlockInfo blockinfo);
    
    Coordinate3D gridToAbsolute(Coordinate3D gridCoordinate);
    
    void legalizeAllLayer(Coordinate3D source , Graph_SP * graph_sp);
    
    double getCost(string spiceName);
    
    // 第一個為viaName,第二個為via location set
    pair<string,vector<Point<int>>>  getViaLocation(Nets & net , Point<int> & orginTarget , bool top);
};

#endif /* RouterV4_hpp */
