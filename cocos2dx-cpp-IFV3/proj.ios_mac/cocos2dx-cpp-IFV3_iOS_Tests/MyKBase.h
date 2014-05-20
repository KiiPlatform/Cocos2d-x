//
//  MyBase.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/20.
//
//

#ifndef cocos2dx_cpp_IFV3_MyBase_h
#define cocos2dx_cpp_IFV3_MyBase_h


#include "KBase.h"
#include <functional>

class MyKBase : public KBase
{
public:
    MyKBase();
    void setCompletionFunc(std::function<void (const char*)> completionFunc);
    void myCallback(const char *json);

private:
    std::function<void (const char*)> completionFunc;
};


#endif
