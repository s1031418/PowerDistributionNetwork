//
//  Components.h
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 26/04/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Components_h
#define Components_h

#include <string>
#include <map>
// A technology LEF file contains all of the LEF technology information for a design,
// such as placement and routing design rules, and process information for layers.

// the components of tech.lef is below
// Define Layer(Routing)
class Layer {
    
public:
    // Specifies the name for the layer.
    std::string NAME  ;
    
    
    std::string TYPE ;
    
    //Specifies the default routing width to use for all regular wiring on the layer.
    //    Type: Float
    double WIDTH ;
    
    
    //Specifies the maximum wire width, in microns, allowed on the layer.
    //Maximum wire width is defined as the smaller value of the width and height of the maximum enclosed rectangle.
    //For example, MAXWIDTH 10.0 specifies that the width of every wire on the layer must be less than or equal to 10.0 μm.
    //Type: Float
    double MAXWIDTH;
    
    
    
    
    // Specifies the default minimum spacing, in microns, allowed between two geometries on different nets.
    // Type: Float
    double SPACING  ;
    // Routing TYPE DEF:
    
    // PITCH DEF:Specifies the required routing pitch for the layer
    // Pitch is used to generate the routing grid (the DEF TRACKS).
    
    // PITCH_DISTANCE DEF:Specifies one pitch value that is used for both the x and y pitch.
    double PITCH_DISTANCE  ;
    // Routing TYPE DEF:
    // OFF SET DEF:Specifies the offset for the routing grid from the design origin for the layer.
    // This value is used to align routing tracks with standard cell boundaries, which helps routers get good on-grid access to the cell pin shapes.
    // For best routing results, most standard cells have a 1/2 pitch offset between the MACRO SIZE boundary and the center of cell pins that should be aligned with the routing grid.
    // Normally, it is best to not set the OFFSET value, so the software can analyze the library to determine the best offset values to use, but in some cases it is necessary to force a specific offset.
    // Generally, it is best for all of the horizontal layers to have the same offset and all of the vertical layers to have the same offset, so that routing grids on different layers align with each other.
    // Higher layers can have a larger pitch, but for best results, they should still align with a lower layer routing grid every few tracks to make stacked-vias more efficient.
    // Default: The software is allowed to determine its own offset values for preferred and non-preferred routing tracks.
    // Type: Float, specified in microns
    
    // OFFSET_DISTANCE DEF: Specifies the offset value that is used for the preferred direction routing tracks.
    double OFFSET_DISTANCE  ;
    //Specifies the preferred routing direction.
    //Automatic routing tools attempt to route in the preferred direction on a layer.
    //A typical case is to route horizontally on layers metal1 and metal3, and vertically on layer metal2.
    std::string DIRECTION  ;
    
    // Routing TYPE DEF:
    // Specifies the resistance for a square of wire, in ohms per square.
    // The resistance of a wire can be defined as RPERSQU * wire length/wire width
    double RESISTANCE_RPERSQ;
};

class Point
{
public:
    float x;
    float y;
};

class InnerLayer {
public:
    std::string NAME;
    std::string SHAPE = "RECT"; // contant
    Point pt1 ;
    Point pt2 ;
    
    
};
class Via {
public:
    std::string NAME ;
    double RESISTANCE ;
    std::map<std::string , InnerLayer> InnerMaps ; // Layer of Via Map
};




// A cell library LEF file contains the macro and standard cell information for a design

// the components of block.lef is below


class Pin
{
    
    
public:
    std::string Name ;
    std::string DIRECTION ;
    std::string USE ; 
    std::map<std::string , InnerLayer> InnerMaps ;// Layer of Pin Map
};
class OBS // obstruction
{
    
public:
    std::map<std::string , InnerLayer> InnerMaps ;// Layer of obstruction Map
};

class Macro {
    
public:
    double ORIGINX ;
    double ORIGINY ;
    std::string Name ;
    std::string CLASS ; 
    double WIDTH ;
    double LENGTH ;
    std::map<std::string,Pin> PinMaps ; // Pin Map
    OBS obs ;// Layer of obstruction Map
};


// the components of def is below
class Nets {
    
public:
    std::string NAME ;
    std::string MACROTYPE ; // define in blocks.lef
    
    Point pt ;
    std::string orient ;
};


// the components of initial_files
class PowerPin {
    
    
public:
    
    
};




#endif /* Components_h */
