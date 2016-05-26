//
//  thello.cpp
//  tpy
//
//  Created by qiao on 16/5/25.
//
//

#include "thello.h"

THello::THello()
{
    CCLOG("THello ctor");
}

THello::~THello()
{
    
}

THello* THello::create()
{
    return new THello();
}