//
//  Converter.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 10/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Converter_hpp
#define Converter_hpp
#include <list>
#include <stdio.h>
#include <string>
#include "Components.h"
using namespace std;

class Line
{
    friend std::ostream &operator<<( std::ostream & output, Line & line)
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
    string MetalName ;
    int Width = -1;
    bool isHorizontal ;
    Line * llink = nullptr ;
    Line * rlink = nullptr;
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




class LineNode
{
public:
    Line line ;
    LineNode * left = nullptr;
    LineNode * right = nullptr;
//    int Width = -1 ; 
//    int V_Width = -1; // Vertical Width
//    int H_Width = -1; // Horizontal Width
};


class Root
{
public:
    int line_count = 0 ; 
    Line * link = nullptr ;
};



typedef Line * Lptr;

class Converter {
    
public:
    Converter();
    ~Converter();
    void toLocationFile();
    void toSpice();
private:
    Root * root ;
    Lptr lptr; 
    
    void Build();
    
    // return 左下角座標及右上角座標
    pair<Point<int>, Point<int>> getBlockCoordinate(int x , int y , int width , int length  , string orient ) ;
    void insert(Lptr line);
    vector<Line> getCrossSets(Line OriginLine, vector<Line>& line_vec);
    void createSubStree(std::multimap<std::string,Nets> , string PinName);
    Line * getStartLine(string PinNames,vector<Line> & line_vec);
    void initLineVec(std::multimap<std::string,Nets> NetsMM , vector<Line>& line_vec);
    pair<int,int> getDirection(string orient , int  width, int  length );
    bool isHorizontal(Point<int> pt1 , Point<int> pt2);
    bool isCross(Line line1 , Line line2);
    // (pt1、pt2) 或 (pt3、pt4) 放入水平兩端的點或垂直兩端的點 ，不要放入將水平或垂直兩端的點放入(pt1、pt3)等等
    bool isIntersaction(Point<int> pt1 , Point<int> pt2 , Point<int> pt3 , Point<int> pt4 , int VerticalLineWidth , int HorizontalLineWidth );
    bool isIntersaction(Point<int> pt1 , Point<int> pt2 , Point<int> pt3 , Point<int> pt4 );
    double calculateResistance(double rpsq , int width , double length );
    void inorder(Lptr lptr);
    
    void eraseline_vec(vector<Line>& , Line line);
    
};
#endif /* Converter_hpp */
