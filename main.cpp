//
//  main.cpp
//  EDA_Contest2017(PDN)
//
//  Created by Jun-Sheng Wu on 26/04/2017.
//  Copyright © 2017 Jun-Sheng Wu. All rights reserved.
//

#include <iostream>
#include <string>
#include "lefrw.h"
#include <stdlib.h>
#include "defrw.h"
#include <unordered_map>
#include <map>
#include "InitialFileParser.hpp"
#include <cstdio>
#include "Converter.hpp"
//#include "DetailRouter.hpp"
#include "RouterV4.hpp"
#include <memory>
#include "PDN.hpp"
#include <stdexcept>
#include "verilog.hpp"
#include "Router.hpp"
#include <array>
using namespace std ;

string exec(const char* cmd);
char ** getlefargv(char * argv[]);
char ** getdefargv(char * argv[]);
void freeargv(char * lefargv[] , char * defargv[]);
void runlef(char * argv[]);
void by(int num , std::multimap<std::string,Nets> & NETSMULTIMAPS);
void test(int num);
void rundef(char * argv[]);
// Ex. Team1.exe case1.v case1_input.def tech.lef blocks.lef initial_files caseX


int main(int argc,  char * argv[])
{
    string CaseName = argv[6];
    verilog verilog(argv[1]);
    InitialFileParser initialfile(argv[5]) ;
    char ** lefargv = getlefargv(argv) ;
    char ** defargv = getdefargv(argv) ;
    defrw def(2, defargv) ;
    lefrw lef(3, lefargv);
    lef.run();
    def.run();
    initialfile.run();
    verilog.run();
    
    RouterV4 router ;
    router.Route();
    Converter converter(CaseName);
    
//    set<int> b ;
//    b.insert(1);
//    b.insert(4);
//    b.insert(5);
//    cout << std::distance(b.begin(), b.find(5));
//    
//    
//    cout << "";
    
//    flute_net f ;
//    f.Demo();
//    Graph_SP g ;
//    g.Demo();
    
//    RouterUtil rutil ;
//    Rectangle rect1( Point<int>( 488000 , 646600 ) , Point<int>(  500000 , 649200) ) ;
//    Rectangle rect2( Point<int>( 495000 , 653600) , Point<int>(  712000 , 663600 ) ) ;
//    cout << boolalpha << rutil.isCross(rect1, rect2) << endl;
//    cout << "Cross Area:" << rutil.getCrossArea(rect1, rect2) << endl;
//    auto Rectangle = rutil.getCrossRect(rect1, rect2) ;
//    cout << "Cross Rect:" << Rectangle.LeftDown << "," << Rectangle.RightUp << endl;
    
//    Router router ;
//    router.Route();
//    vector<Coordinate3D> Paths ;
//    Paths.push_back(Coordinate3D(2,2,2));
//    Paths.push_back(Coordinate3D(2,3,2));
//    Paths.push_back(Coordinate3D(2,3,1));
//    Paths.push_back(Coordinate3D(2,4,1));
//    Paths.push_back(Coordinate3D(3,4,1));
//    Paths.push_back(Coordinate3D(4,3,1));
//    Paths.push_back(Coordinate3D(5,3,1));
//    Paths.push_back(Coordinate3D(6,3,1));
//    Paths.push_back(Coordinate3D(7,3,1));
//    Paths.push_back(Coordinate3D(8,3,1));
//    Paths.push_back(Coordinate3D(9,3,1));
//    Paths.push_back(Coordinate3D(10,3,1));
//    Paths.push_back(Coordinate3D(10,4,1));
//    Paths.push_back(Coordinate3D(10,5,1));
//    Paths.push_back(Coordinate3D(10,6,1));
//    Paths.push_back(Coordinate3D(10,7,1));
//    router.TestToOutoutDef(Paths);
    
//    Graph_SP g9(4);
//    //    Graph_SP g9;
//    g9.AddEdge(0, 2, 1);
//    g9.AddEdge(0, 1, 1);
//    
//    g9.AddEdge(2, 3, 1);
//    g9.AddEdge(1, 3, 1);
//    g9.Dijkstra(0);
//    
//    auto p = g9.getPath(3);
//    for(auto x : p)
//        cout << x << endl;
//    cout << "3";
//    DetailRouter dr ;
//    dr.DetailRoute();
    //GlobalRouter gr ;
    //gr.Route();
//    int x = 1;
//    int y = 98;
//    int z = 2;
//    int WIDTH = 10000 ;
//    int LENGTH = 10000 ;
//    
////    cout << x + WIDTH * (y + DEPTH * z);
//    cout << "OriginCoordinate:" << "(" << x << "," << y << "," << z << ")" << endl;
//    
//    
//    // transform 3d to 1d
//    
//    int one_D = x + y * WIDTH + z * WIDTH * LENGTH ;
//    cout << "Transform 3d to 1d:" ;
//    cout << one_D << endl;
//
//    // tranform 3d to 1d
//    x = one_D % WIDTH ;
//    y = ( one_D / WIDTH ) % LENGTH ;
//    z = one_D / ( WIDTH * LENGTH ) ;
//    cout << "Transform 1d to 3d:";
//    cout << "(" << x << "," << y << "," << z << ")" << endl;
    
    
    // transform 1d to 2d
//     size = WIDTH;
//     y = one_D / (int)size ;
//     x = one_D % (int)size ;
//    cout << "2d:" << x << "," << y << endl;
   
    
    
//    gr.Route();
//    gr.Route();
    
    
    
//    Debugger db ;
//    db.printAllSpecialNetMsg(SpecialNetsMaps);
    
    //for(int i = 0 ; i < 3 ; i++)
    //{
        //converter.toLocationFile();
       ////// converter.Visualize();
        ////test(2);
      //}
    PDN a(CaseName);
    for(int i = 0 ; i < 3; i++)
    {
        converter.toLocationFile();
        a.Optimize();
    }
    converter.toSpice();
    ngspice ng ;
    ng.init(CaseName);
    cout<<"*************FINAL:***************\n";
    ng.printStats(converter.DestinationMap);
    //cout<<"-----ngspice---end------\n";
    //converter.toLocationFile();
    //a.Optimize();
   // converter.toLocationFile();
    //a.Optimize();
   // converter.toLocationFile();
    //a.Optimize();
  //  converter.toLocationFile();
//    for(int i = 0 ; i < 3 ; i++)
//    {
//        converter.toLocationFile();
       //// converter.Visualize();
       //// test(2);
//    }
//    cout<<"-----ngspice---end------\n";
//    PDN a(CaseName);
//    a.Optimize();
//    a.Optimize();
    //a.ToSpecialNetsMaps();
//    converter.toOutputDef();
//    converter.Visualize();
    cout<<"*************************************\n";
    delete[] lefargv;
    delete[] defargv ;
    return 0;
}
void test(int num)
{
    vector<string> PinNames;
    for( auto it = PinMaps.begin(), end = PinMaps.end(); it != end;it = PinMaps.upper_bound(it->first))
    {
        PinNames.push_back(it->first);
    }
    
    for(auto PinName : PinNames)
    {
        by(num , SpecialNetsMaps[PinName].NETSMULTIMAPS);
    }
    
}
void by(int num , std::multimap<std::string,Nets> & NETSMULTIMAPS)
{
    for( auto it = NETSMULTIMAPS.begin(), end = NETSMULTIMAPS.end(); it != end;it = NETSMULTIMAPS.upper_bound(it->first))
    {
        auto first = NETSMULTIMAPS.lower_bound(it->first);
        auto last = NETSMULTIMAPS.upper_bound(it->first);
        while (first != last) {
            first->second.ROUNTWIDTH *= num ;
            first++;
        }
    }
}
string exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != NULL)
            result += buffer.data();
    }
    return result;
}
char ** getlefargv(char * argv[])
{
    // new memory allocation
    char ** LefInput ;
    LefInput = new char *[3];
    LefInput[0] = new char [1000];
    LefInput[1] = new char [1000];
    LefInput[2] = new char [1000];
    // initialize Lef argument value
    LefInput[0] = argv[0];
    LefInput[1] = argv[3];
    LefInput[2] = argv[4];
    return LefInput ;
}
char ** getdefargv(char * argv[])
{
    char ** DefInput ;
    DefInput = new char *[2];
    DefInput[0] = new char [1000];
    DefInput[1] = new char [1000];
    // initialize Lef argument value
    DefInput[0] = argv[0];
    DefInput[1] = argv[2];
    return DefInput ;
}

