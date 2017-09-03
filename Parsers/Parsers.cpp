//
//  Parsers.cpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 01/05/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#include "Parsers.hpp"
#include <assert.h>




Parsers::Parsers()
{
    
}
Parsers::~Parsers()
{
    
}




bool Parsers::isMatch(regex_t & regex , string data)
{
    regmatch_t IsMatch_match[2] ;
    int ret = regexec(&regex, data.c_str(), 1, IsMatch_match, 0) ;
    return !(ret == REG_NOMATCH) ;
}
vector<string> Parsers::GetParsingString(regex_t & regex , string data , vector<int> GroupIdx)
{
    
    regmatch_t match[100];
    vector<string> tmp ;
    
    if(regexec(&regex, data.c_str(), 100, match, 0) == 0)
    {
        for (int i = 0 ; i < GroupIdx.size(); i++) {
            string str ;
            int index = GroupIdx[i];
            for( long j = match[index].rm_so ; j < match[index].rm_eo ; j++ )
            {
                str +=  data[j];
            }
            tmp.push_back(str);
        }
        
    }
    else
    {
        assert(0);
    }
    return tmp ;
}

