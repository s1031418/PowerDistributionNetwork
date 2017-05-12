//
//  Debugger.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 01/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Debugger_hpp
#define Debugger_hpp

#include <stdio.h>
#include "Components.h"
#include <map>
#include <iostream>
#include <vector>
#include <string>
using namespace std ;

class Debugger{
    
public:
    // Print function
    void printLayerMsg(string LayerName , map<string,Layer> & LM );
    void printViaMsg(string ViaName , map<string,Via> & VM );
    void printMacroMsg(string MacroName , map<string,Macro> & MM );
    void printAllLayerMsg(map<string,Layer> & LM);
    void printAllViaMsg(map<string,Via> & VM);
    void printAllInnerLayerMsg(map<string , InnerLayer> & IM);
    void printAllMacroMsg( map<string,Macro> & MM );
    void printInnerLayerMsg(string LayerName , map<string , InnerLayer> & IM);
    void printObsMsg(OBS);
    void printBlockPinMsg(string PinName , map<string,BlockPin> & BPM);
    void printAllBlockPinMsg(std::map<std::string,BlockPin> & BPM);
    void printWeightMsg(string MetalName , map<string,string> & WM);
    void printAllWeightMsg(map<string,string> & WM);
    void printVoltageMsg(string PowerSourceName , map<string,string> & VoltageMaps );
    void printAllVoltageMsg(map<string,string> & VoltageMaps);
    void printCurrentMsg(string BlockName ,multimap<string,PowerPin> & CurrnetMaps  );
    void printAllCurrentMsg(multimap<string,PowerPin> & CurrnetMaps);
    void printConstraintMsg(string BlockName , multimap<string,Constraint> & ConstraintMaps);
    void printAllConstraintMsg(multimap<string,Constraint> & ConstraintMaps);
    void printComponentMsg(string BlockName ,  std::map<std::string,Components> & ComponentMaps);
    void printAllComponentMsg(std::map<std::string,Components> & ComponentMaps);
    void printPinMsg(string PinName ,  std::map<std::string,Pin> & PinMaps);
    void printAllPinMsg(std::map<std::string,Pin> & PinMaps);
    void printPortMsg(string MetalName ,std::multimap< std::string, Port > & PortMaps);
    void printAllPortMsg(std::multimap< std::string, Port > & PortMaps);
    
    // for print special nets
    void printSpecialNetMsg(string SourceName , std::map<std::string,SpecialNets> & SpecialNetsMaps);
    void printDestinationMsg(string BlockName , std::multimap<std::string,std::string> & DESTINATIONMAPS);
    void printAllDestinationMsg(std::multimap<std::string,std::string> & DESTINATIONMAPS);
    void printNetsMsg(string MetalName ,std::multimap<std::string,Nets> & NETSMULTIMAPS );
    void printAllNetsMsg(std::multimap<std::string,Nets> & NETSMULTIMAPS );
    void printAllSpecialNetMsg(std::map<std::string,SpecialNets> & SpecialNetsMaps);
    
    // print DIE AREA
    void printDieArea(Diearea & diearea);
    
    template<typename T>
    vector<string> getUniqueKeys(T & Map);
};

#endif /* Debugger_hpp */
