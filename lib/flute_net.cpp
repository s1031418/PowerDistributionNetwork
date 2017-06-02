//
//  flute_net.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 02/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "flute_net.hpp"




flute_net::flute_net()
{
    
}

flute_net::~flute_net()
{
    
}
void flute_net::printSteinerTree()
{
    for(int i = 0 ; i < SteinerTree.size() ; i++)
    {
        if(SteinerTree[i].isSteiner) cout << "s" ;
        cout << i << ":" << setw(3) <<  "x=" << SteinerTree[i].x << setw(3) << "y=" << SteinerTree[i].y << setw(3)<< "e=" << SteinerTree[i].target << endl;
    }
    printf("FLUTE wirelength = %d\n", length);
    printf("FLUTE wirelength (without RSMT construction) = %d\n", length_noRSMT);
}
void flute_net::getSteinerTree(vector<Point<int>> Points)
{
    SteinerTree.clear();
    int d=0;
    int x[MAXD], y[MAXD];
    Tree flutetree;
    for( auto pt : Points )
    {
        x[d] = pt.x ;
        y[d] = pt.y ;
        d++ ;
    }
    readLUT();
    
    flutetree = flute(d, x, y, ACCURACY);
    gettree(flutetree, SteinerTree);
    length = flutetree.length;
    length_noRSMT = flute_wl(d, x, y, ACCURACY);
    
}
