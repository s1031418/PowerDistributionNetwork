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
    cout << "Name:" << MM[MacroName].Name << endl;
    cout << "Width:" << MM[MacroName].Width << endl;
    cout << "Length:" << MM[MacroName].Length << endl;
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
