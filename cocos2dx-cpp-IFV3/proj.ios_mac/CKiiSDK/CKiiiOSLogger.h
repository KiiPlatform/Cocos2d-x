//
//  CKiiiOSLogger.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/27.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiiOSLogger__
#define __cocos2dx_cpp_IFV3__CKiiiOSLogger__

#include <iostream>
#include "CKiiLogger.h"

namespace kiicloud {

class CKiiiOSLogger : public CKiiLogger
{
public:
    CKiiiOSLogger();
    ~CKiiiOSLogger();
    void log(const std::string& message);
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiiOSLogger__) */
