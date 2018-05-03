//
//  GlobalRouter.hpp
//  EDA_Contest2017(PDN)
//
//

#ifndef GlobalRouter_hpp
#define GlobalRouter_hpp

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

// User Define Parameters
// Define Global Routing Grid Size
// Grid Size = Scaling * minimal space
const unsigned Scaling = 100 ;


enum NetOrder
{
    IR_DROP , // use Dijkstra to get steiner point
    SHORTESTPATH // use flute to get steiner point
};

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


class GlobalRouter {
    
    
public:
    GlobalRouter();
    ~GlobalRouter();
    void Route();
    void toGridGraph();
    
    int getLength(vector<int> & path);
    
    map<string,string> LCSTable ;
    
    vector<GlobalSolution> GlobalSolutions ;

    //vector<vector<Coordinate3D>> GlobalSolutions;
    
    vector< vector< Grid > > Grids ;
    
    map<double,Path> getNetOrdering();
private:
    // variable:
    RouterUtil RouterHelper; 
    Graph_SP graph;
    set<int> Vertical ;
    set<int> Horizontal ;
    
    unsigned lowest ; // lowest metal layer
    unsigned highest ; // highest metal layer 
    
    
    void printAllGrids();
    
    
    void updateGraph_SP(set<int> & UpdateGrids , map<int,int> & hvTable);
    
    void Esitimate();
    
    double getEsitimateWidth(string powerPin , BlockInfo blockinfo );
    
    unsigned estimateCritical(vector<Point<int>> & Points);
    
    // cost function for Graph_SP edge weights
    unsigned cost(int z , bool horizontal , Grid & grid);
        
    ThreeDGridLocation get3DGridLocation(int z);
    
    TwoDGridLocation get2DGridLocation(int x , int y);
    
    pair<TwoDGridLocation, ThreeDGridLocation> getGridLocation(int x , int y , int z );
    // key:BlockName , value:那個block的左下及右上座標
    map<string, pair<Point<int>, Point<int>>> BlockMap;
    // -----------------------------------------------------
    // function:
    
    // Cut By BlockBoundary ( non-uniform )
    void CutByBlockBoundary();
    
    // Cut By Minimal Space ( uniform )
    void CutByUserDefine();
    
    void InitGrids();
    
    // Initialize Graph_SP and model 3D coordinate 
    void InitGraph_SP();
    
    // 轉換2d座標為1d
    int translate2D_1D(int x , int y);
    // 轉換1d座標為2d
    pair<int, int> translate1D_2D(int index);
    
    // translate 3D coordinate to 1D coordinate
    int translate3D_1D(int x , int y , int z);
    
    // translate 1D coordinate to 3D coordinate
    tuple<int,int,int> translate1D_3D(int index);
    
    // 給我一個方塊的左下右上，算出他在哪些Grid裡面
    // 如果這個方塊介於兩個的Grid之間
    // return 包比較多的那個GridY
    // 第一個值為X座標，第二個為Y座標
    pair<int, int> getGridCoordinate( Point<int> LeftDown , Point<int> RightUp );
    
    // 給我一個點，算出它屬於哪一個Grid
    // 如果點在交界上，如果是Ｘ就return左邊的，如果是Ｙ就return下面的
    // 第一個值為X座標，第二個為Y座標
    pair<int, int> getGridCoordinate( Point<int> pt );
    
    
    pair<int, int> ripple(int X , int Y);
    
    // print shortest path of Dijkstra algorithm
    void printPath(vector<int> & path);
    
    // get x y coordinate of flute results 
    Point<int> getTerminalCoordinate(int x , int y , int target , vector<node> & SteierTree) ;
    
};


#endif /* GlobalRouter_hpp */
