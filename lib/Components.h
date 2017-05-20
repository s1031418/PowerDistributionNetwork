//
//  Components.h
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 26/04/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Components_h
#define Components_h
#include <assert.h>
#include <iostream>
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

template <class T>
class Point
{
    template <class U>
    friend std::ostream &operator<<( std::ostream &, Point< U > & );
    
public:
    T x = -1;
    T y = -1;
    Point(){};
    Point(T X , T Y);
//    Point() (T X , T Y )
//    {
//        this->x = X ;
//        this->y = Y ; 
//    }
    ~Point<T>()
    {
    }
    
    const Point<T> & operator=(const Point<T> & right)
    {
        if( this != &right )
        {
            this->x = right.x;
            this->y = right.y;
        }
        return *this ;
    }
    Point<T> operator*(double number)
    {
        Point<T> temp = *this ;
        temp.x *= number ;
        temp.y *= number ;
        return temp ;
    }
    Point<T> operator+( Point<T> & right)
    {
        Point<T> temp = *this ;
        temp.x += right.x ;
        temp.y += right.y ;
        return temp ;
    }
    
    Point<T> operator+=( Point<T> & right)
    {
        this->x += right.x ;
        this->y += right.y ;
        return *this ;
    }
    bool operator==(const Point<T> & right)
    {
        return (this->x == right.x && this->y == right.y);
    }
    
    
};
// Point function

template <class T>
Point<T>::Point (T X , T Y)
{
    this->x = X ;
    this->y = Y ;
}
template <class T>
std::ostream & operator<<( std::ostream & output , Point< T > & point )
{
    output << "( " << point.x << " " << point.y << " )";
    return output ; 
}



class Line
{
    friend std::ostream &operator<<(  std::ostream & output,  Line & line)
    {
        output << line.pt1 << " " << line.pt2 ;
        return output ;
    };
public:
    //假如水平線，pt1為左邊，pt2為右邊 垂直線pt1為下面，pt2為上面
    Line(){};
    Line(Point<int> X , Point<int> Y)
    {
        pt1 = X ;
        pt2 = Y ;
    };
    ~Line(){};
    
    Point<int> pt1;
    Point<int> pt2;
    std::string MetalName ;
    int layer ; 
    int Width = -1;
    bool isHorizontal ;
    
    inline bool operator< (const Line& right) const
    {
        // 目前還沒寫
        return true ;
//        if( this->pt1.x ==  right.pt1.x &&  this->pt1.y ==  right.pt1.y && this->pt2.x ==  right.pt2.x &&  this->pt2.y ==  right.pt2.y) return  false;
//        if(this->pt1.x == this->pt2.x && right.pt1.x == right.pt2.x) // vertical
//        {
//            return (this->pt1.y <   right.pt2.y);
//        }
//        else if (this->pt1.y == this->pt2.y && right.pt1.y == right.pt2.y) // horizontal
//        {
//            return (this->pt1.x <   right.pt2.x) ;
//        }
//        return true ;

        
    }
    const Line & operator=(const Line & right  )
    {
        if( this != &right )
        {
            this->pt1 = right.pt1 ;
            this->pt2 = right.pt2 ;
            this->MetalName = right.MetalName ;
            this->Width = right.Width ;
            this->isHorizontal = right.isHorizontal ;
        }
        return *this ;
    }
    bool operator==(const Line & right)
    {
        return (this->pt1== right.pt1 && this->pt2 == right.pt2
                && this->MetalName == right.MetalName && this->Width == right.Width && this->isHorizontal == right.isHorizontal);
    }
    
};




class InnerLayer {
public:
    std::string NAME;
    std::string SHAPE = "RECT"; // contant
    Point<float> pt1 ;
    Point<float> pt2 ;
    
    
};
class Via {
public:
    std::string NAME ;
    double RESISTANCE ;
    std::map<std::string , InnerLayer> InnerMaps ; // Layer of Via Map
};




// A cell library LEF file contains the macro and standard cell information for a design

// the components of block.lef is below


class BlockPin
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
    std::string SYMMETRY ; // 還沒寫
    std::string Name ;
    std::string CLASS ; 
    double WIDTH ;
    double LENGTH ;
    std::map<std::string,BlockPin> BlockPinMaps ;
    OBS obs ;// Layer of obstruction Map
};


// the components of def is below
class Components {
    
public:
    std::string NAME ;
    std::string MACROTYPE ; // define in blocks.lef
    Point<int> STARTPOINT ;
    std::string ORIENT ;
    std::string STATE; // fixed or placed
};

class Port
{
public:
    std::string NAME ; 
    Point<int> RELATIVE_POINT1 ;
    Point<int> RELATIVE_POINT2 ;
    Point<int> STARTPOINT ;
    std::string ORIENT ;
};
class Pin
{
public:
    std::string NAME ;
    std::string DIRECTION;
    std::string USE ;
    std::string METALNAME ;
    Point<int> STARTPOINT ;
    Point<int> RELATIVE_POINT1;
    Point<int> RELATIVE_POINT2;
    std::string ORIENT ;
    // key:MetalName , value: Port
    std::multimap< std::string, Port > PortMaps;
    
};
class Diearea{
    
public:
    Point<int> pt1 ;
    Point<int> pt2 ;
};
// sub components
class Nets {
    
public:
    std::string METALNAME ;
    int ROUNTWIDTH ;
    Point<int> ABSOLUTE_POINT1 ;
    Point<int> ABSOLUTE_POINT2 ;
    std::string VIANAME ;
    std::string SHAPE ;
    std::string STATE ; 
};
class SpecialNets {
    
public:
    std::string SOURCENAME;
    // key: BlockName , value:Block Pin
    std::multimap<std::string,std::string> DESTINATIONMAPS;
    // key: MetalName , value:Nets object
    std::multimap<std::string,Nets> NETSMULTIMAPS ;
    
    
    std::string USE ; 
    
};


// the components of initial_files
class PowerPin {
    
    
public:
    std::string NAME ;
    std::string CURRENTDRAWN ;
    
};
class Constraint {
    
    
public:
    std::string NAME ;
    std::string CONSTRAINT ;
    
};




#endif /* Components_h */
