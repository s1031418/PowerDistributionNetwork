//
//  Converter.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 10/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Converter_hpp
#define Converter_hpp

#include <stdio.h>
#include <string>
#include "Components.h"
using namespace std;
class Converter {
    
public:
    Converter();
    ~Converter();
    void toDebugMsg();
    void toSpice();
private:
    // return 左下角座標及右上角座標
    pair<Point<int>, Point<int>> getBlockCoordinate(int x , int y , int width , int length  , string orient ) ;
    
    pair<int,int> getDirection(string orient , int  width, int  length );
    bool isHorizontal(Point<int> pt1 , Point<int> pt2);
    
    
};
#endif /* Converter_hpp */
