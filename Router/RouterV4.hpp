//
//  RouterV4.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 05/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef RouterV4_hpp
#define RouterV4_hpp

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
#include "Graph.hpp"
#include "InnerTreeObj.h"
using namespace std;


struct RoutingPath {
    string sourceName;
//    Coordinate3D sourceCoordinate ;
    string targetBlockName ;
    string targetBlockPinName; 
    Coordinate3D targetCoordinate ;
    double voltage ;
    double diffVoltage ; 
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
    double DEFAULTWIDTH = 10 ;
    double DEFAULTSPACING = 2 ;
    set<int> boundList ;
    
    SpiceGenerator sp_gen ;
    DefGenerator def_gen ;
    OutputFilesGenerator output_gen ;
    // 用來查詢各個點的width
    
    map<string,BlockCoordinate> leftBlockMap ;
    
    map<string,BlockCoordinate> rightBlockMap ;
    
    // 絕對座標＋GridZ
    vector<RoutingPath> currentRoutingLists ;
    
    vector<RoutingPath> NoPassRoutingLists ; 
    
    map<string,vector<Coordinate3D>> multiPinCandidates;
    
    map<string,vector<Coordinate3D>> normalDistributionCandidates; 
    
    map<string , Coordinate3D> MagicPoints ;
    
    map<string,vector<Coordinate3D>> sourceTargetInitPath;
    
    // key: vdd_source 
//    map<string,vector<BlockCoordinate>> obstacles;
    
    map<string,vector<BlockCoordinate>> leftObstacles ;
    
    map<string,vector<BlockCoordinate>> rightObstacles ;
    
    
    RouterUtil RouterHelper;
    
    string getNgSpiceKey(Coordinate3D coordinate3d);
    
    void fillSpNetMaps( vector<Coordinate3D> & paths , string powerPinName ,string blockName , string blockPinName  , double width , bool peek );
    
    Coordinate3D LegalizeTargetEdge(Block block , Graph_SP * graph_sp , double width , double spacing);
    
    double getMetalResistance(int layer);
    
    Point<int> getAbsolutePoint( Coordinate3D coordinate3d );
    
    void Simulation();
    
    // absolute point to grid point
    int getGridX(int x);
    
    int getGridY(int y);
    
    Coordinate3D getGridCoordinate( Block block );
    
    void InitBoundList();
    
    void BlockTopBottom(Graph_SP * graph_sp);
    
    void genResistance(vector<Coordinate3D> & paths , string powerPinName , SpiceGenerator & sp_gen , double width );
    
    void generateSpiceList(Coordinate3D powerPoint , Coordinate3D blockPoint , vector<Coordinate3D> & paths , string powerPinName , string blockName , string blockPinName , double width );
    
    string gridToString(Coordinate3D , bool);
    
    void AddVirtualObstacle();
    
    void BlockFrontDoor(Graph_SP * graph_sp , string sourcePowerPin);
    
    void BlockGridCoordinate( Graph_SP * graph_sp , Block & block);
    
    void InitPowerPinAndBlockPin(double width , double spacing );
    
    void getInitSolution(Block powerBlock  , string powerpin, string blockName , string BlockPinName , double width , double spacing , bool source );
    
    void InitState();
    
    void updateGrid(CrossInfo result , Grid  & grid );
    
    void CutGrid(double width , double spacing);
    
    void InitGrids(string source , double width , double spacing , bool cutGrid = true , vector<int> SpecialHorizontal = vector<int>(),  vector<int> SpecialVertical = vector<int>());
    
    Graph_SP * InitGraph_SP(int lowerLayer , int higherLayer , double width , double spacing);
    
    int translate3D_1D(Coordinate3D coordinate3d);
    
    Coordinate3D translate1D_3D(int index);
    
    void toGridGraph();
    
    void printAllGrids();
    
    vector<pair<Direction3D, int>> translateToFriendlyForm( vector<Coordinate3D> & Paths );
    
    vector<int> getAbsoluteDistance(vector<pair<Direction3D, int>> & friendlyPaths , Point<int> startPoint);
    
    // 傳入grid座標，轉成絕對座標存起來
    void saveMultiPinCandidates(string powerPin , vector<Coordinate3D> solutions );
    
    bool isMultiPin(string powerPin);
    
    void legalizeAllOrient(bool allowIn , Coordinate3D coordinate , Graph_SP * graph_sp , double width , double spacing , double originWidth);
    
    vector<Coordinate3D> selectPath(string powerPin , Graph_SP * graph_sp , int target , int source , string block , string blockPin , double width , double spacing , double originWidth);
    
    Coordinate3D getLastIlegalCoordinate(Direction3D orient , Coordinate3D sourceGrid , double width , double spacing , double originWidth);
    
    void legalizeEdge(bool allowIn , Coordinate3D source , Coordinate3D target , Direction3D orient , Graph_SP * graph_sp , double width);
    
    void saveRoutingList(Coordinate3D target , string powerPin , BlockInfo blockinfo);
    
    Coordinate3D gridToAbsolute(Coordinate3D gridCoordinate);
    
    void legalizeAllLayer(bool allowIn , Coordinate3D source , Graph_SP * graph_sp , double width , double spacing , double originWidth);
    
    double getCost(string spiceName , double metalUsage);
    
    // 第一個為viaName,第二個為via location set
    pair<string,vector<Point<int>>>  getViaLocation(Nets & net , Point<int> & orginTarget , bool top , double width);
    
    vector<Coordinate3D> parallelRoute( string powerPin , string blockName , string blockPinName , Coordinate3D source , Coordinate3D target , double width , double spacing , double originWidth);
    
    Coordinate3D AbsToGrid(Coordinate3D coordinateABS);
    
    pair<vector<string>,map<string,vector<Path>>> getNetOrdering(double width, double spacing , double originWidth);
    
    void SteinerTreeConstruction(bool isSimulation ,  vector<Coordinate3D> & solutions , double current , double constraint , double voltage , string powerPin , string block , string blockPin , Graph * & steinerTree);
    
    void SteinerTreeReduction(Graph * &steinerTree , vector<Coordinate3D> & terminals);
    
    vector<Coordinate3D> selectMergePoint(Coordinate3D & powerPinCoordinate , Coordinate3D & BlockPinCoordinate , bool inverse , bool multiSource , double constraint , double current , double voltage , Graph * steinerTree , string powerPin , Graph_SP * graph_sp , int target, int source  , string block , string blockPin , double width , double spacing , double originWidth) ;
    
    void InitializeSpiceGen(Graph * steinerTree);
    
    
    void optimize(Graph * steinerTree);
    
    vector<Coordinate3D> getCorner(vector<pair<Direction3D, int>> & friendlyForm , Coordinate3D source );
    
    
    Coordinate3D getNext(Direction3D direction , Coordinate3D corner);
    
    int gridYToAbs(int gridY);
    
    int gridXToAbs(int gridX);
    
    bool isSameLayer(vector<Coordinate3D> & path);
    
    Coordinate3D selectSource(Coordinate3D corner);
    
    Coordinate3D selectTarget(Coordinate3D corner);
    
    void insertObstacles(CrossRegion crossRegion, string powerPinName , BlockCoordinate blockCoordinate);
    
    void updateGrids(CrossRegion crossRegion , bool blockOrObstacle , Rectangle rect1 , Rectangle via , double width , double spacing , Grid & grid);
    
    double getMetalUsage(vector<Coordinate3D> solutions , double width);
    
    bool isPossibleHasSolutions(Coordinate3D coordinate , Graph_SP * graph_sp , double width , double spacing , double originWidth);
    
    double getParallelFOM(string spiceName , double metalUsage , double originV);
};

#endif /* RouterV4_hpp */
