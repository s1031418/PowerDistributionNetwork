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
using namespace std ;
class Debugger{
    
public:
    void printLayerMsg(string LayerName , map<string,Layer> & LM );
    void printViaMsg(string ViaName , map<string,Via> & VM );
    void printMacroMsg(string MacroName , map<string,Macro> & MM );
    void printAllLayerMsg(map<string,Layer> & LM);
    void printAllViaMsg(map<string,Via> & VM);
    void printAllInnerLayerMsg(map<string , InnerLayer> & IM);
    void printAllMacroMsg( map<string,Macro> & MM );
    void printInnerLayerMsg(string LayerName , map<string , InnerLayer> & IM);
    void printObsMsg(OBS);
    void printPinMsg(string PinName , map<string,Pin> & PM);
    void printAllPinMsg(std::map<std::string,Pin> & PM);
};

#endif /* Debugger_hpp */
