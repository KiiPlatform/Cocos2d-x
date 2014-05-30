//
//  CKiiLog.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/27.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiLogger__
#define __cocos2dx_cpp_IFV3__CKiiLogger__

#include <iostream>

namespace kiicloud {

class CKiiLogger
{
public:
    virtual void log(const std::string& log) = 0;
    virtual ~CKiiLogger();
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiLog__) */
