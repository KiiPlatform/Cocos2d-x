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

class MyKBase : public KBase
{
public:
    MyKBase();
    void myCallback(const char *json);
    const char* getCallbackJson();
    void nullifyCallbackJson();
private:
    const char* callbackJson;
};


#endif
