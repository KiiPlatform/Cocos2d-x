//
//  CKiiUser.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/23.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiUser__
#define __cocos2dx_cpp_IFV3__CKiiUser__

#include <iostream>
#include "CKiiAuth.h"
#include "picojson.h"

namespace kiicloud {

class CKiiUser : public CKiiAuth {
public:
    CKiiUser();
    explicit CKiiUser(const picojson::value& keyValues);
    ~CKiiUser();
    // TODO: encapsulate picojson.
    picojson::object getKeyValues();

private:
    picojson::object keyValues;
};

};

#endif /* defined(__cocos2dx_cpp_IFV3__CKiiUser__) */
