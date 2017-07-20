//
//  verilog.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 05/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "verilog.hpp"


multimap< string, BlockInfo > Connection ;


verilog::verilog(string FilePath)
{
    Load(FilePath);
}
verilog::~verilog()
{
    
}

void verilog::ParseSubBlock(string BlockName , string BlockMsg)
{
//    cout << BlockName << " ";
    BlockInfo blockinfo ;
    blockinfo.BlockName = BlockName ;
    vector<BlockInfo> results;
    int dot_index = (int)BlockMsg.find(".");
    int left_parenthesis = (int)BlockMsg.find("(" , dot_index + 1);
//    cout << BlockMsg.substr( dot_index + 1 , left_parenthesis - dot_index - 1) << " ";
    blockinfo.BlockPinName = BlockMsg.substr( dot_index + 1 , left_parenthesis - dot_index - 1) ;
    int right_parenthesis = (int)BlockMsg.find(")" , left_parenthesis + 1);
//    cout << BlockMsg.substr( left_parenthesis + 1 , right_parenthesis - left_parenthesis - 1) <<" ";
    string PowerPinName = BlockMsg.substr( left_parenthesis + 1 , right_parenthesis - left_parenthesis - 1);
    Connection.insert(make_pair(PowerPinName, blockinfo));
    while (true) {
        dot_index = (int)BlockMsg.find("." , right_parenthesis + 1);
        if( dot_index == string::npos ) break;
        left_parenthesis = (int)BlockMsg.find("(" , dot_index + 1);
        blockinfo.BlockPinName = BlockMsg.substr( dot_index + 1 , left_parenthesis - dot_index - 1) ;
//        cout << BlockMsg.substr( dot_index + 1 , left_parenthesis - dot_index - 1) << " ";
        right_parenthesis = (int)BlockMsg.find(")" , left_parenthesis + 1);
        PowerPinName = BlockMsg.substr( left_parenthesis + 1 , right_parenthesis - left_parenthesis - 1) ;
//        cout << BlockMsg.substr( left_parenthesis + 1 , right_parenthesis - left_parenthesis - 1) << " ";
        Connection.insert(make_pair(PowerPinName, blockinfo));
    }
//    cout << endl;
}
void verilog::run()
{
    for (int i = 0 ; i < Data.size(); i++)
    {
        if (Data[i].find("block") != string::npos)
        {
            regex_t Regex ; // group1:blockName group3 (...)
            if ( regcomp(&Regex, "[[:space:]]*[[:alnum:]]+[[:space:]]+(([[:alnum:]]|_)+)[[:space:]]+\((.*)\)", REG_EXTENDED) ) cerr << "Compile Error";
            {
                vector<string> results = GetParsingString(Regex, Data[i], vector<int>{1,3});
                ParseSubBlock(results[0] , results[1]);
                
            }
            regfree(&Regex);
        }
    }
//    for( auto it = Connection.begin(), end = Connection.end(); it != end;it = Connection.upper_bound(it->first))
//    {
//        auto first = Connection.lower_bound(it->first);
//        auto last = Connection.upper_bound(it->first);
//        while (first != last) {
//            cout << first->first << " " << first->second.BlockName << " " << first->second.BlockPinName << endl;
//            first++;
//        }
//        
//    }
}

void verilog::Load(string FilePath)
{
    string content ;
    fstream fin(FilePath , ios::in);
    if(!fin) cerr << "Can't open file.";
    while (getline(fin,content)) {
        Data.push_back(content);
    }
    fin.close();
}
