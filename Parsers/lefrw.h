//
//  lefrw.h
//  TestLefParser
//
//  Created by 吳峻陞 on 03/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef lefrw_h
#define lefrw_h


#ifdef WIN32
#pragma warning (disable : 4786)
#endif

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>

#ifndef WIN32
#   include <unistd.h>
#else
#   include <windows.h>
#endif /* not WIN32 */
#include "lefrReader.hpp"
#include "lefwWriter.hpp"
#include "lefiDebug.hpp"
#include "lefiEncryptInt.hpp"
#include "lefiUtil.hpp"
#include "Components.h"
#include <string>
#include <map>

extern std::map<std::string,Via> ViaMaps ;
extern std::map<std::string,Layer> LayerMaps ;
// key: macroName , value : Macro
extern std::map<std::string,Macro> MacroMaps ;
extern Macro mymacro ;
extern double MANUFACTURINGGRID ;
class lefrw {
    
    
public:
    lefrw();
    ~lefrw();
    int run(int argc, char ** argv);
    
    
    
private:
    
    
};
#endif /* lefrw_h */
