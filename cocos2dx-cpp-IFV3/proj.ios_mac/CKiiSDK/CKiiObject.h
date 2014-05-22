//
//  CKiiObject.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/22.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiObject__
#define __cocos2dx_cpp_IFV3__CKiiObject__

#include <iostream>
#include "KBase.h"
#include "picojson.h"
#include "CKiiError.h"

namespace kiicloud
{
class CKiiObject
{
public:
    void refresh(std::function<void ()> refreshCallback);
    void patch(picojson::object& patchData, std::function<void (CKiiError* error)> patchCallback, bool force);
    void replace(picojson::object& replacementData, std::function<void(CKiiError *error)> replaceCallback, bool force);
};
}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiObject__) */