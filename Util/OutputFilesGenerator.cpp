//
//  OutputFilesGenerator.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 26/07/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "OutputFilesGenerator.hpp"




OutputFilesGenerator::OutputFilesGenerator()
{
    
}
OutputFilesGenerator::~OutputFilesGenerator()
{
    
}
void OutputFilesGenerator::setOutputFilesName(string outputName)
{
    outputfileName = outputName ; 
}
void OutputFilesGenerator::setIRDrop(string BlockName , string BlockPinName , double DropPercentage)
{
    IRDropLine irline ;
    irline.blockName = BlockName ;
    irline.blockPinName = BlockPinName ;
    irline.dropPercentage = DropPercentage ;
    irDropLines.push_back(irline);
}
Rectangle OutputFilesGenerator::getRectangle(multimap<string,Nets>::iterator it1 )
{
    Rectangle rect1;
    if( myhelper.isHorizontal(it1->second.ABSOLUTE_POINT1, it1->second.ABSOLUTE_POINT2) )
    {
        Point<int> left = ( it1->second.ABSOLUTE_POINT1.x < it1->second.ABSOLUTE_POINT2.x ) ? it1->second.ABSOLUTE_POINT1 : it1->second.ABSOLUTE_POINT2 ;
        Point<int> right = ( it1->second.ABSOLUTE_POINT1.x > it1->second.ABSOLUTE_POINT2.x ) ? it1->second.ABSOLUTE_POINT1 : it1->second.ABSOLUTE_POINT2 ;
        rect1.LeftDown = left ;
        rect1.RightUp = right ;
        rect1.LeftDown.y -= (it1->second.ROUNTWIDTH / 2 ) ;
        rect1.RightUp.y += (it1->second.ROUNTWIDTH / 2 ) ;
    }
    else
    {
        Point<int> up = ( it1->second.ABSOLUTE_POINT1.y > it1->second.ABSOLUTE_POINT2.y ) ? it1->second.ABSOLUTE_POINT1 : it1->second.ABSOLUTE_POINT2 ;
        Point<int> down = ( it1->second.ABSOLUTE_POINT1.y < it1->second.ABSOLUTE_POINT2.y ) ? it1->second.ABSOLUTE_POINT1 : it1->second.ABSOLUTE_POINT2 ;
        rect1.LeftDown = down ;
        rect1.RightUp = up ;
        rect1.LeftDown.x -= (it1->second.ROUNTWIDTH / 2 ) ;
        rect1.RightUp.x += (it1->second.ROUNTWIDTH / 2 ) ;
    }
    return rect1 ;
}
void OutputFilesGenerator::calculateMetalUsage()
{
    map<string,double> areaSum ;
    map<string,double> duplicateAreaSum ;
    vector<pair<Rectangle, Rectangle>> historyCrossArea ;
    for( auto specialNet : SpecialNetsMaps )
    {
        for( auto it = specialNet.second.NETSMULTIMAPS.begin() ; it != specialNet.second.NETSMULTIMAPS.end() ; it = specialNet.second.NETSMULTIMAPS.upper_bound(it->first) )
        {
            auto ret = specialNet.second.NETSMULTIMAPS.equal_range(it->first);
            for (auto it1 = ret.first; it1 != ret.second; ++it1)
            {
//                cout << it1->second.ROUNTWIDTH << " " << it1->second.METALNAME << " " << it1->second.ABSOLUTE_POINT1 << " " << it1->second.ABSOLUTE_POINT2 << endl;
                
                if( it1->second.ROUNTWIDTH == 0 ) continue;
                Rectangle rect1 = getRectangle(it1);
                if( areaSum.find(it1->second.METALNAME ) == areaSum.end() ) areaSum.insert(make_pair(it1->second.METALNAME, 0));
                double width = rect1.RightUp.x - rect1.LeftDown.x ;
                double length = rect1.RightUp.y - rect1.LeftDown.y ;
                double area = width / UNITS_DISTANCE * length / UNITS_DISTANCE ;
                areaSum[ it1->second.METALNAME ] += area ;
                for (auto it2 = ret.first; it2 != ret.second; ++it2)
                {
                    if( it1 == it2 ) continue ;
                    if( it2->second.ROUNTWIDTH == 0 ) continue;
                    Rectangle rect2 = getRectangle(it2);
                    
                    // the same layer and cross
                    if( it1->second.METALNAME == it2->second.METALNAME && myhelper.isCross(rect1, rect2) )
                    {
                        bool hasBeenCalculated = false;
                        for( auto crossArea : historyCrossArea )
                        {
                            if( crossArea.first == rect1 && crossArea.second == rect2)
                                hasBeenCalculated = true ;
                            if( crossArea.first == rect2 && crossArea.second == rect1)
                                hasBeenCalculated = true ;
                        }
                        if( duplicateAreaSum.find(it1->second.METALNAME ) == duplicateAreaSum.end() ) duplicateAreaSum.insert(make_pair(it1->second.METALNAME, 0));
                        if( !hasBeenCalculated )
                        {
                            int crossArea = myhelper.getCrossArea(rect1, rect2) / UNITS_DISTANCE / UNITS_DISTANCE ;
                            
                            duplicateAreaSum[it1->second.METALNAME] += crossArea ;
                            historyCrossArea.push_back(make_pair(rect1, rect2));
                        }
                    }
                }
                
            }
        }
    }
    for( auto x : areaSum )
    {
        MetalUsageLine metalUsageLine ;
        metalUsageLine.MetalName = myhelper.getAlias( x.first );
        metalUsageLine.metalUsage = x.second ;
        if( duplicateAreaSum.find(x.first) != duplicateAreaSum.end())
            metalUsageLine.metalUsage -= duplicateAreaSum[x.first];
        metalUsageLines.push_back(metalUsageLine);
    }
}
void OutputFilesGenerator::toOutputFiles()
{
    double TotalMetalUsage = 0 ;
    calculateMetalUsage();
    FILE * pFile ;
    pFile = fopen(outputfileName.c_str(), "w");
    if( NULL == pFile ) printf("Failed to open file\n");
    
    // The metal usage report
    fprintf(pFile, "The metal usage report\n");
    for( auto metalLine : metalUsageLines )
    {
        fprintf(pFile, "%s %g\n" , metalLine.MetalName.c_str() , metalLine.metalUsage);
        TotalMetalUsage += (stod(WeightsMaps[metalLine.MetalName]) * metalLine.metalUsage);
    }
    fprintf(pFile, "Total %f\n" , TotalMetalUsage);
    fprintf(pFile, "\n");
    
    
    // The IR drop report
    fprintf(pFile, "The IR drop of each power pin (%%)\n");
    for(auto irline : irDropLines)
    {
        fprintf(pFile, "%s/%s %g\n" , irline.blockName.c_str() , irline.blockPinName.c_str() , irline.dropPercentage);
    }
    fclose(pFile);
}





















