//
//  CKiiError.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/22.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiError__
#define __cocos2dx_cpp_IFV3__CKiiError__

#include <iostream>
namespace kiicloud
{

class CKiiError
{
public:
    CKiiError(int httpErrorCode, std::string kiiErrorCode);
    int getHttpErrorCode();
    std::string* getKiiErrorCode();
private:
    int httpErrorCode;
    std::string* kiiErrorCode;
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiError__) */
