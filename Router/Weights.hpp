//
//  Weights.hpp
//  EDA_Contest2017(PDN)
//
//  Created by 吳峻陞 on 12/06/2017.
//  Copyright © 2017 吳峻陞. All rights reserved.
//

#ifndef Weights_hpp
#define Weights_hpp

#include <stdio.h>
#include <map>
#include <string>
using namespace std;
class Weights
{
    // 電阻計算公式：resistance = rpsq * length / width
    // region = length * width
    // 考慮因子：面積、電阻、initial files、IR-Drop 的權重
    // Metal Weight 計算公式： alpha * rpsq +  weights
    // Via Weight 計算公式：
public:
    Weights(); // defalut constrctor
    ~Weights();
    void SetWidth();
    map<string,double> metal_weights ;
    map<string,double> via_weights ;
private:
    void Init();
    
    unsigned width ;
};




#endif /* Weights_hpp */
