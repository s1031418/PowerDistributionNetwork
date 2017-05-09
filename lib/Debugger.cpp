//
//  Debugger.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 01/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "Debugger.hpp"

void Debugger::printAllMacroMsg( map<string,Macro> & MM )
{
    for(auto x : MM)
    {
        printMacroMsg(x.first, MM);
    }
}
void Debugger::printMacroMsg(string MacroName , map<string,Macro> & MM )
{
    cout << "Class:" << MM[MacroName].CLASS << endl; 
    cout << "Name:" << MM[MacroName].Name << endl;
    cout << "OriginX:" << MM[MacroName].ORIGINX << endl;
    cout << "OriginY:" << MM[MacroName].ORIGINY << endl;
    cout << "Width:" << MM[MacroName].WIDTH << endl;
    cout << "Length:" << MM[MacroName].LENGTH << endl;
    cout << "Pins:" << endl;
    printAllPinMsg(MM[MacroName].PinMaps);
    cout << "OBS" << endl;
    printObsMsg(MM[MacroName].obs);
}
void Debugger::printAllPinMsg(std::map<std::string,Pin> & PM)
{
    for(auto x : PM)
    {
        printPinMsg(x.first , PM);
    }
}
void Debugger::printObsMsg(OBS obs)
{
    printAllInnerLayerMsg(obs.InnerMaps);
}
void Debugger::printPinMsg(string PinName , std::map<std::string,Pin> & PM)
{
    cout << "Name:" << PM[PinName].Name << endl;
    printAllInnerLayerMsg(PM[PinName].InnerMaps);
}
void Debugger::printAllInnerLayerMsg(map<string , InnerLayer> & IM)
{
    for(auto i : IM)
    {
        printInnerLayerMsg(i.first , IM);
    }
}
void Debugger::printInnerLayerMsg(string LayerName , map<string , InnerLayer> & IM)
{
    cout << "NAME:" << IM[LayerName].NAME << endl;
    cout << "Point1:(" << IM[LayerName].pt1.x << "," << IM[LayerName].pt1.y << ")" << endl;
    cout << "Point2:(" << IM[LayerName].pt2.x << "," << IM[LayerName].pt2.y << ")" << endl;
}
void Debugger::printLayerMsg(string LayerName , map<string,Layer> & LM )
{
    cout << "Name:"<< LM[LayerName].NAME << endl;
    cout << "TYPE:"<< LM[LayerName].TYPE << endl;
    if(LM[LayerName].TYPE == "CUT") return ;
    cout << "WIDTH:"<< LM[LayerName].WIDTH << endl;
    cout << "MAXWIDTH:"<< LM[LayerName].MAXWIDTH << endl;
    cout << "SPACING:"<< LM[LayerName].SPACING << endl;
    cout << "DIRECTION:"<< LM[LayerName].DIRECTION << endl;
    cout << "OFFSET_DISTANCE:"<< LM[LayerName].OFFSET_DISTANCE << endl;
    cout << "PITCH_DISTANCE:"<< LM[LayerName].PITCH_DISTANCE << endl;
    cout << "RESISTANCE_RPERSQ:"<< LM[LayerName].RESISTANCE_RPERSQ << endl;
}
void Debugger::printViaMsg(string ViaName , map<string,Via> & VM )
{
    cout << "NAME:" << VM[ViaName].NAME << endl;
    cout << "RESISTANCE:" << VM[ViaName].RESISTANCE << endl;
    cout << "LoAs:" << endl;
    printAllInnerLayerMsg(VM[ViaName].InnerMaps);
}
void Debugger::printAllLayerMsg(map<string,Layer> & LM)
{
    for(auto x : LM)
    {
        printLayerMsg(x.first , LM);
    }
    
    
}
void Debugger::printAllViaMsg(map<string,Via> & VM)
{
    for(auto x : VM)
    {
        printViaMsg(x.first , VM);
    }
}
void Debugger::printWeightMsg(string MetalName , map<string,string> & WM)
{
    cout << "Name:" << MetalName << endl ;
    cout << "Weights:" << WM[MetalName] << endl; 
}
void Debugger::printAllWeightMsg(map<string,string> & WM)
{
    for(auto i : WM)
        printWeightMsg(i.first, WM);
}
void Debugger::printVoltageMsg(string PowerSourceName , map<string,string> & VoltageMaps )
{
    cout << "PowerSourceName:"  << PowerSourceName << endl;
    cout << "Voltage:" << VoltageMaps[PowerSourceName] << endl; 
}
void Debugger::printAllVoltageMsg(map<string,string> & VM)
{
    for(auto i : VM)
        printVoltageMsg(i.first, VM);
}
void Debugger::printCurrentMsg(string BlockName ,multimap<string,PowerPin> & CurrnetMaps  )
{
    cout << "BlockName:" << BlockName << endl ;
    auto begin = CurrnetMaps.lower_bound(BlockName);
    auto end = CurrnetMaps.upper_bound(BlockName);
    while (begin != end) {
        cout << "BlockPinName:"<< begin->second.NAME  << "  Current:" << begin->second.CURRENTDRAWN << endl;
        begin++;
    }
    
    
}
void Debugger::printAllCurrentMsg(multimap<string,PowerPin> & CurrentMaps)
{
    for(auto i : CurrentMaps)
        printCurrentMsg(i.first, CurrentMaps);
}

void Debugger::printConstraintMsg(string BlockName , multimap<string,Constraint> & ConstraintMaps)
{
    cout << "BlockName:" << BlockName << endl ;
    auto begin = ConstraintMaps.lower_bound(BlockName);
    auto end = ConstraintMaps.upper_bound(BlockName);
    while (begin != end) {
        cout << "BlockPinName:"<< begin->second.NAME  << "  Constraint:" << begin->second.CONSTRAINT << "(%)" << endl;
        begin++;
    }
}
void Debugger::printAllConstraintMsg(multimap<string,Constraint> & ConstraintMaps)
{
    for(auto i : ConstraintMaps)
        printConstraintMsg(i.first, ConstraintMaps);
}
