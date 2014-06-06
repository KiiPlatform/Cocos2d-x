//
//  _CKiiGlobal.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#ifndef cocos2dx_cpp_IFV3__CKiiGlobal_h
#define cocos2dx_cpp_IFV3__CKiiGlobal_h

#include "CKiiBaseBindings.h"
#include "CKiicURLBindings.h"

namespace kiicloud
{
    static CKiiBaseBindings * _bind = new CKiicURLBindings();
};

#endif
