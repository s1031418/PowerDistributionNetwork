#ifndef PDNNEW_hpp
#define PDNNEW_hpp
#include <utility>   
#include <stdio.h>
#include <vector>
#include <string>
#include <list>
#include "../Parsers/Components.h"
#include "Converter.hpp"
#include "PDNHelper.hpp"
#include <set>
#include <cmath>
#include "Mylineinfor.hpp"
using namespace std ;
class PDNNEW 
{
    public: 
         PDNNEW(){};
        ~PDNNEW(){};  
        PDNHelper HelpMe;
        vector<Mylineinfor* > AllLine;
        map <string , Mylineinfor* >Graph; //hash PinName to Root 
        map <string , vector<Mylineinfor*> > LinesOfLayer;//using metalname to hash that lines of layer ;
        void initGraphRootMap();
        void pushNetsinMyLineVector( vector <Mylineinfor*> &AllLineFormPin , Nets NET);
        void initDataByBFS();
        void initLineInLayer();
        void getRoutingResource();
    private:
};
#endif
