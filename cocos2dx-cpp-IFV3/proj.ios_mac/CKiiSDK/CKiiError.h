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
#include <future>

namespace kiicloud
{

class CKiiError;
typedef std::shared_ptr<CKiiError> ErrorPtr;
typedef std::future<ErrorPtr> ErrorFuture;

class CKiiError
{
public:
    explicit CKiiError();
    explicit CKiiError(int httpErrorCode, const std::string &kiiErrorCode);
    explicit CKiiError(const CKiiError& lv);
    explicit CKiiError(CKiiError&& lv);

    ~CKiiError();
    int getHttpErrorCode();
    std::string getKiiErrorCode();
    std::string toString();
private:
    int httpErrorCode;
    std::string kiiErrorCode;
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiError__) */
