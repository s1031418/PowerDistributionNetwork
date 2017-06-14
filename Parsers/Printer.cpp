//
//  Printer.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 01/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "Printer.hpp"

void Printer::printAllMacroMsg( map<string,Macro> & MM )
{
    for(auto x : MM)
    {
        printMacroMsg(x.first, MM);
    }
}
void Printer::printMacroMsg(string MacroName , map<string,Macro> & MM )
{
    cout << "Class:" << MM[MacroName].CLASS << endl; 
    cout << "Name:" << MM[MacroName].Name << endl;
    cout << "OriginX:" << MM[MacroName].ORIGINX << endl;
    cout << "OriginY:" << MM[MacroName].ORIGINY << endl;
    cout << "Width:" << MM[MacroName].WIDTH << endl;
    cout << "Length:" << MM[MacroName].LENGTH << endl;
    cout << "Symmetry:" << MM[MacroName].SYMMETRY << endl;
    cout << endl;
    cout << "Pins:" << endl;
    printAllBlockPinMsg(MM[MacroName].BlockPinMaps);
    cout << "OBS" << endl;
    printObsMsg(MM[MacroName].obs);
    
}
void Printer::printAllBlockPinMsg(std::map<std::string,BlockPin> & BPM)
{
    for(auto x : BPM)
    {
        printBlockPinMsg(x.first , BPM);
    }
}
void Printer::printObsMsg(OBS obs)
{
    printAllInnerLayerMsg(obs.InnerMaps);
}
void Printer::printBlockPinMsg(string PinName , std::map<std::string,BlockPin> & BPM)
{
    cout << "Name:" << BPM[PinName].Name << endl;
    printAllInnerLayerMsg(BPM[PinName].InnerMaps);
    cout << endl;
}
void Printer::printAllInnerLayerMsg(map<string , InnerLayer> & IM)
{
    for(auto i : IM)
    {
        printInnerLayerMsg(i.first , IM);
    }
}
void Printer::printInnerLayerMsg(string LayerName , map<string , InnerLayer> & IM)
{
    cout << "NAME:" << IM[LayerName].NAME << endl;
    cout << "Point1:(" << IM[LayerName].pt1.x << "," << IM[LayerName].pt1.y << ")" << endl;
    cout << "Point2:(" << IM[LayerName].pt2.x << "," << IM[LayerName].pt2.y << ")" << endl;
    cout << endl ;
}
void Printer::printLayerMsg(string LayerName , map<string,Layer> & LM )
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
    cout << endl;
}
void Printer::printViaMsg(string ViaName , map<string,Via> & VM )
{
    cout << "NAME:" << VM[ViaName].NAME << endl;
    cout << "RESISTANCE:" << VM[ViaName].RESISTANCE << endl;
    cout << endl;
    cout << "LoAs:" << endl;
    printAllInnerLayerMsg(VM[ViaName].InnerMaps);
    cout << endl;
}
void Printer::printAllLayerMsg(map<string,Layer> & LM)
{
    for(auto x : LM)
    {
        printLayerMsg(x.first , LM);
    }
    
    
}
void Printer::printAllViaMsg(map<string,Via> & VM)
{
    for(auto x : VM)
    {
        printViaMsg(x.first , VM);
    }
}
void Printer::printWeightMsg(string MetalName , map<string,string> & WM)
{
    cout << "Name:" << MetalName << endl ;
    cout << "Weights:" << WM[MetalName] << endl; 
}
void Printer::printAllWeightMsg(map<string,string> & WM)
{
    for(auto i : WM)
        printWeightMsg(i.first, WM);
}
void Printer::printVoltageMsg(string PowerSourceName , map<string,string> & VoltageMaps )
{
    cout << "PowerSourceName:"  << PowerSourceName << endl;
    cout << "Voltage:" << VoltageMaps[PowerSourceName] << "(V)"<< endl;
}
void Printer::printAllVoltageMsg(map<string,string> & VM)
{
    for(auto i : VM)
        printVoltageMsg(i.first, VM);
}
void Printer::printCurrentMsg(string BlockName ,multimap<string,PowerPin> & CurrnetMaps  )
{
    cout << "BlockName:" << BlockName << endl ;
    auto begin = CurrnetMaps.lower_bound(BlockName);
    auto end = CurrnetMaps.upper_bound(BlockName);
    while (begin != end) {
        cout << "BlockPinName:"<< begin->second.NAME  << "  Current:" << begin->second.CURRENTDRAWN << "(mA)"<< endl;
        begin++;
    }
    
    
}
void Printer::printAllCurrentMsg(multimap<string,PowerPin> & CurrentMaps)
{
    for( auto it = CurrentMaps.begin(), end = CurrentMaps.end(); it != end;it = CurrentMaps.upper_bound(it->first))
    {
        printCurrentMsg(it->first, CurrentMaps);
    }
}

void Printer::printConstraintMsg(string BlockName , multimap<string,Constraint> & ConstraintMaps)
{
    cout << "BlockName:" << BlockName << endl ;
    auto begin = ConstraintMaps.lower_bound(BlockName);
    auto end = ConstraintMaps.upper_bound(BlockName);
    while (begin != end) {
        cout << "BlockPinName:"<< begin->second.NAME  << "  Constraint:" << begin->second.CONSTRAINT << "(%)" << endl;
        begin++;
    }
}
void Printer::printAllConstraintMsg(multimap<string,Constraint> & ConstraintMaps)
{
    for( auto it = ConstraintMaps.begin(), end = ConstraintMaps.end(); it != end;it = ConstraintMaps.upper_bound(it->first))
    {
        printConstraintMsg(it->first , ConstraintMaps);
    }
}
void Printer::printComponentMsg(string BlockName ,  std::map<std::string,Components> & ComponentMaps)
{
    cout << BlockName << " " << ComponentMaps[BlockName].MACROTYPE << " " << ComponentMaps[BlockName].STATE << " ";
    cout << "( " << ComponentMaps[BlockName].STARTPOINT.x << " " << ComponentMaps[BlockName].STARTPOINT.y << " ) ";
    cout << ComponentMaps[BlockName].ORIENT << endl;
}
void Printer::printAllComponentMsg(std::map<std::string,Components> & ComponentMaps)
{
    for(auto i : ComponentMaps)
        printComponentMsg(i.first, ComponentMaps);
}
void Printer::printPinMsg(string PinName ,  std::map<std::string,Pin> & PinMaps)
{
    cout << PinName << " " << "Direction " << PinMaps[PinName].DIRECTION << " USE " << PinMaps[PinName].USE  << endl;
    if(PinMaps[PinName].PortMaps.empty())
    {
        
        cout << " Layer " << PinMaps[PinName].METALNAME << " ( " << PinMaps[PinName].RELATIVE_POINT1.x << " " << PinMaps[PinName].RELATIVE_POINT1.y << " )";
        cout << " ( " << PinMaps[PinName].RELATIVE_POINT2.x << " " << PinMaps[PinName].RELATIVE_POINT2.y << " ) " << endl;
        cout << "Fixed " << "( " << PinMaps[PinName].STARTPOINT.x << " " << PinMaps[PinName].STARTPOINT.y << " )";
        cout << PinMaps[PinName].ORIENT << endl;
    }
    else
    {
        printAllPortMsg(PinMaps[PinName].PortMaps);
    }
    cout << endl;
}
void Printer::printAllPinMsg(std::map<std::string,Pin> & PinMaps)
{
    for(auto x : PinMaps)
    {
        printPinMsg(x.first , PinMaps);
    }
}
void Printer::printPortMsg(string MetalName ,std::multimap< std::string, Port > & PortMaps)
{
    auto begin = PortMaps.lower_bound(MetalName);
    auto end = PortMaps.upper_bound(MetalName);
    while (begin != end) {
        cout << "LAYER "<< MetalName << " ( " << begin->second.RELATIVE_POINT1.x  << " " << begin->second.RELATIVE_POINT1.y << " ) ";
        cout << " ( " << begin->second.RELATIVE_POINT2.x  << " " << begin->second.RELATIVE_POINT2.y << " ) " << endl;
        cout << "FIXED ( " << begin->second.STARTPOINT.x << " " << begin->second.STARTPOINT.y << " ) ";
        cout << " " << begin->second.ORIENT << endl;
        begin++;
    }
    
}
void Printer::printAllPortMsg(std::multimap< std::string, Port > & PortMaps)
{
    for( auto it = PortMaps.begin(), end = PortMaps.end(); it != end;it = PortMaps.upper_bound(it->first))
    {
        printPortMsg(it->first , PortMaps);
    }
}
void Printer::printSpecialNetMsg(string SourceName , std::map<std::string,SpecialNets> & SpecialNetsMaps)
{
    
    cout << SourceName ;
    printAllDestinationMsg(SpecialNetsMaps[SourceName].DESTINATIONMAPS);
    printAllNetsMsg(SpecialNetsMaps[SourceName].NETSMULTIMAPS);
    cout << "USE " << SpecialNetsMaps[SourceName].USE << endl;
    
    
}
void Printer::printDestinationMsg(string BlockName , std::multimap<std::string,std::string> & DESTINATIONMAPS)
{
    auto begin = DESTINATIONMAPS.lower_bound(BlockName);
    auto end = DESTINATIONMAPS.upper_bound(BlockName);
    while (begin != end) {
        cout << " ( " << BlockName << " " << begin->second << " )";
        begin++;
    }
    
}
void Printer::printAllDestinationMsg(std::multimap<std::string,std::string> & DESTINATIONMAPS)
{
    for( auto it = DESTINATIONMAPS.begin(), end = DESTINATIONMAPS.end(); it != end;it = DESTINATIONMAPS.upper_bound(it->first))
    {
        printDestinationMsg(it->first , DESTINATIONMAPS);
    }
    cout << endl;
}
void Printer::printNetsMsg(string MetalName ,std::multimap<std::string,Nets> & NETSMULTIMAPS )
{
    auto begin = NETSMULTIMAPS.lower_bound(MetalName);
    auto end = NETSMULTIMAPS.upper_bound(MetalName);
    while (begin != end) {
        cout << begin->second.STATE << " " << MetalName << " " << begin->second.ROUNTWIDTH ;
        cout << " SHAPE " << begin->second.SHAPE << " ( " << begin->second.ABSOLUTE_POINT1.x ;
        cout << " " << begin->second.ABSOLUTE_POINT1.y << " )";
        if(begin->second.ROUNTWIDTH != 0 )
        {
            cout << " ( " << begin->second.ABSOLUTE_POINT2.x ;
            cout << " " << begin->second.ABSOLUTE_POINT2.y << " )" << endl;
        }
        else
        {
            cout << begin->second.VIANAME << endl;
        }
        begin++;
    }
}
void Printer::printAllNetsMsg(std::multimap<std::string,Nets> & NETSMULTIMAPS )
{
    for( auto it = NETSMULTIMAPS.begin(), end = NETSMULTIMAPS.end(); it != end;it = NETSMULTIMAPS.upper_bound(it->first))
    {
        printNetsMsg(it->first , NETSMULTIMAPS);
    }
}
void Printer::printAllSpecialNetMsg(std::map<std::string,SpecialNets> & SpecialNetsMaps)
{
    for( auto it = SpecialNetsMaps.begin(), end = SpecialNetsMaps.end(); it != end;it = SpecialNetsMaps.upper_bound(it->first))
    {
        printSpecialNetMsg(it->first , SpecialNetsMaps);
    }
}
void Printer::printDieArea(Diearea & diearea)
{
    cout << "DIEAREA" << " ( " << diearea.pt1.x << " " << diearea.pt1.y << " )";
    cout << " ( " << diearea.pt2.x << " " << diearea.pt2.y << " )" << endl;
}

template<typename T >
vector<string> Printer::getUniqueKeys(T & Maps)
{
    vector<string> temp ;
    for( auto it = Maps.begin(), end = Maps.end(); it != end;it = Maps.upper_bound(it->first))
    {
        temp.push_back(it->first);
    }
    return temp ;
}
