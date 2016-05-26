//
//  thello.hpp
//  tpy
//
//  Created by qiao on 16/5/25.
//
//

#ifndef thello_hpp
#define thello_hpp

#include <stdio.h>


class THello {
private:
    
public:
    THello();
    ~THello();
    
    static THello* create();
    
    static int add(int a, int b){return a + b;};
    
    int sub(int a, int b){return a - b ;};
};


#endif /* thello_hpp */
