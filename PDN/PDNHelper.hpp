//
//  PDNHelper.hpp
//  EDA_Contest2017(PDN)
//
//

#ifndef PDNHelper_hpp
#define PDNHelper_hpp

#include <stdio.h>
#include "../Parsers/Components.h"
#include "RouterComponents.hpp"
#include <cmath>
#include "../Parsers/lefrw.h"
#include "../Parsers/defrw.h"
#include <string>
#include "../Parsers/InitialFileParser.hpp"
using namespace std ;

class PDNHelper {
    
public:
    
    PDNHelper();
    ~PDNHelper();
    vector<string> PinNames;
    
    map<string , vector<Block> > PowerMaps ;
    // Key: BlockName , value : BlockPin的詳細資訊 (包含BlockPinName , Blockpin 座標 , Blockpin 包含的Metal層)
    map<string , vector<Block> > BlockPinMaps ;
    
    map<string,vector<ViaInfo>> ViaInfos;
    
    vector<Block> getPowerPinInfo();
    
    vector<Block> getBlockPinInfo();
    
    double getCurrent(string blockName ,string blockPinName);
    
    double getIRDropConstaint(string blockName , string blockPinName);
    
    pair< Point<int>, Point<int> > getPowerPinCoordinate(int x , int y , Point<int> r_pt1, Point<int> r_pt2  , string orient);
    
    // 對Ｘ軸做鏡射
    Point<int> FlipX(float y_axis , Point<int> pt , DIRECTION orientation);
    
    // 對Ｙ軸做鏡射
    Point<int> FlipY(float x_axis , Point<int> pt , DIRECTION orientation);
    void InitBlockMaps();
    void InitPowerMaps();
    
    // 判斷兩個點是否水平
    bool isHorizontal(Point<int> pt1 , Point<int> pt2);
    //return 左下角座標及右上角座標
    pair<Point<int>, Point<int>> getBlockCoordinate(int x , int y , int width , int length  , string orient ) ;
    
    // 給我兩條線判斷是否交叉
    bool isCross(Line line1 , Line line2);
    
    // 給我一條線及左下右上的點判斷是否交叉
    bool isCrossWithCoordinate(Line line , pair< Point<int> , Point<int> > & blockCroordinate) ;

    //給我兩條線，回傳兩條線的交叉點
    //如果是Pseudo Line，就回傳Psedo Line的中點(VIA中點)
    Point<int> getCrossPoint(Line line1 , Line line2);
    
    // 給我兩個點，判斷是不是起點（ PowerPin內 ）
    // 如果是就把那個點回傳，否則回傳空的Point
//    Point<int> getStartPoint(Point<int> pt1 , Point<int> pt2);
    
    // 給我兩個點，判斷是不是終點（ BlockPin內 ）
    // 如果是就把那個點回傳，否則回傳空的Point
//    Point<int> getEndPoint(Point<int> pt1 , Point<int> pt2);
    
    double calculateResistance(double rpsq , int width , double length );
    
    //給我一個點，判斷他是屬於哪個Block跟哪個BlockPin
    //第一個是BlockName , BlockPinName
    pair<string, string> getBlockMsg(Point<int> pt);
    
    // 給BlockName跟BlockPinName ， return 這個BlockPin的Detail(Block)
    // 找不到就回傳空的Block
    Block getBlock( string BlockName , string BlockPinName );
    
    //給我一個點，判斷他是屬於哪個PowerPin
    // 回傳 PowerPin Name
    string getPowerPinMsg(Point<int> pt);
    
    vector<Block> getPowerPinCoordinate(string powerPinName);
    
    //給Block左下跟右上角，BlockPin左下跟右上角，還有方向
    //回傳旋轉完的左下跟右上
    pair<Point<int>, Point<int>> getBlockPinCoordinate(Point<int> BlcokLeftDown , Point<int> BlcokRightUp , Point<int> BlockPinLeftDown , Point<int> BlockPinRightUp , string orient);
    
    // 將Detination Map 全部轉為 string
    string DestinationMapToString(multimap<string,string>& DestinationMap);
    // 將NETSMULTI MAP 全部轉為 string
    string NETSMULTIMAPSToString(multimap<string,Nets> & NETSMULTIMAPS);
    
    
    Point<int> getCenter( Point<int> LeftDown , Point<int> RightUp );
    
    double getSourceVoltage(string powerpin);
    
    void InitViaInfos();
    
    int getCrossArea(Rectangle & rect1 , Rectangle & rect2);
    
    bool isCross(Rectangle & rect1 , Rectangle & rect2);
    
    string getAlias(string MetalName);
    
    double getManhattanDistance(Point<int> pt1 , Point<int> pt2);
    
    // get ManhattanDistance of three points
    double getManhattanDistance(Coordinate3D pt1 , Coordinate3D pt2);
};


inline bool PDNHelper::isCross(Rectangle & rect1 , Rectangle & rect2)
{
    //    pair<int, int> rect1Center = make_pair( ( rect1.LeftDown.x + rect1.RightUp.x ) / 2 , ( rect1.LeftDown.y + rect1.RightUp.y ) / 2 );
    //    pair<int, int> rect2Center = make_pair( ( rect2.LeftDown.x + rect2.RightUp.x ) / 2 , ( rect2.LeftDown.y + rect2.RightUp.y ) / 2 );
    //    int verticalDistance = abs(rect1Center.second - rect2Center.second);
    //    int horizontalDistance = abs(rect1Center.first - rect2Center.first);
    //    int rect1Width = rect1.RightUp.x - rect1.LeftDown.x ;
    //    int rect1Height = rect1.RightUp.y - rect1.LeftDown.y ;
    //    int rect2Width = rect2.RightUp.x - rect2.LeftDown.x ;
    //    int rect2Height = rect2.RightUp.y - rect2.LeftDown.y ;
    //    int verticalThreshold = (rect1Height + rect2Height)/2 ;
    //    int horizontalThreshold = (rect1Width + rect2Width)/2 ;
    if( abs((( rect1.LeftDown.y + rect1.RightUp.y ) >> 1)  - (( rect2.LeftDown.y + rect2.RightUp.y ) >> 1 )) > (rect1.RightUp.y - rect1.LeftDown.y + rect2.RightUp.y - rect2.LeftDown.y) >> 1
       || abs((( rect1.LeftDown.x + rect1.RightUp.x ) >> 1) - (( rect2.LeftDown.x + rect2.RightUp.x ) >> 1)) > (rect1.RightUp.x - rect1.LeftDown.x + rect2.RightUp.x - rect2.LeftDown.x) >> 1 )
        return false ;
    return true ;
    //    if( verticalDistance > verticalThreshold || horizontalDistance > horizontalThreshold )
    //        return false ;
    //    return true ;
}
#endif /* PDNHelper_hpp */

