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

void flute_net::getSteinerPoint(vector<Point<int>> Points)
{
    int d=0;
    int x[MAXD], y[MAXD];
    Tree flutetree;
    int flutewl;
    for( auto pt : Points )
    {
        x[d] = pt.x ;
        y[d] = pt.y ;
        d++ ;
    }
    //    while (!feof(stdin)) {
    //        scanf("%d %d\n", &x[d], &y[d]);
    //        d++;
    //    }
    //    for(int i = 0 ; i < MAXD ; i++)
    //        cout << x[i] << endl;
    //    for(int i = 0 ; i < MAXD ; i++)
    //        cout << y[i] << endl;
    //    cout << x[0] << " " << y[0] << " " << x[1] << " " << y[1] <<" " << endl;
//    x[0] = 1;
//    y[0] = 2;
//    x[1] = 3;
//    y[1] = 4;
//    d = 2 ;
    readLUT();
    
    flutetree = flute(d, x, y, ACCURACY);
    printtree(flutetree);
    printf("FLUTE wirelength = %d\n", flutetree.length);
    
    flutewl = flute_wl(d, x, y, ACCURACY);
    printf("FLUTE wirelength (without RSMT construction) = %d\n", flutewl);
}
