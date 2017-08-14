//
//  InnerTreeObj.h
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 13/08/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef InnerTreeObj_h
#define InnerTreeObj_h

class InnerTreeObj {
    
public:
    double current ;
    double critical ;
};

// 先比電流，電流小的放前面(由小到大sort)
// 在比critical ， 越大的放前面(越大代表越不critical )(由大到小sort)
class InnerTreeObjComparator {
public:
    bool operator()(const InnerTreeObj& lhs, const InnerTreeObj& rhs) const
    {
        if( lhs.current < rhs.current)
            return true ;
        else if ( lhs.current > rhs.current )
            return false ;
        else
        {
            if( lhs.critical > rhs.critical )
                return true ;
            else
                return false ;
        }
    }
};

#endif /* InnerTreeObj_h */
