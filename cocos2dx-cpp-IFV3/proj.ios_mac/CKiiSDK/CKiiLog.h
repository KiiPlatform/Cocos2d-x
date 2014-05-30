//
//  CKiiLog.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/27.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiLog__
#define __cocos2dx_cpp_IFV3__CKiiLog__

#include <iostream>
#include "CKiiLogger.h"


namespace kiicloud {

class CKiiLog
{
public:
    ~CKiiLog();
    static std::shared_ptr<CKiiLog> getInstance();
    void log(const std::string& message);

private:
    CKiiLog();
    static std::shared_ptr<CKiiLog> factoryPtr;
    CKiiLogger *logger;
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiLog__) */
