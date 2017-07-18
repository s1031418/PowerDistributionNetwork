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
#include <algorithm>
#include "defrw.h"
#include "../Parsers/ngspice.hpp"
#include "flute_net.hpp"
#include "RouterUtil.hpp"
#include "RouterComponents.hpp"
#include "GlobalRouter.hpp"
#include "DefGenerator.hpp"
#include "SpiceGenerator.hpp"
using namespace std;

struct Comparator
{
    bool operator()(const Line & left, const Line & right) const
    {
        if ( left.pt1.x == right.pt1.x )
        {
            return left.pt1.y < right.pt1.y;
        }
        
        return left.pt1.x < right.pt1.x;
    }
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
    int SPACING = 6 ;
    set<int> boundList ;
    SpiceGenerator sp_gen ;
    DefGenerator def_gen ;
    
    
    map<string , Coordinate3D> MagicPoints ;
    
    
    map<string,vector<Coordinate3D>> sourceTargetInitPath;
    
    // key: vdd_source 
    map<string,vector<BlockCoordinate>> obstacles;
    
    RouterUtil RouterHelper;
    
    string getNgSpiceKey(Coordinate3D coordinate3d);
    
    void fillSpNetMaps( vector<Coordinate3D> & paths , string powerPinName , BlockInfo blockinfo  );
    
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
    
    void genResistance(vector<Coordinate3D> & paths , string powerPinName);
    
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
};

#endif /* RouterV4_hpp */
