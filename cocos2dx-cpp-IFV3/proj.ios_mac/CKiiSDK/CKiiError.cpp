//
//  CKiiError.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/22.
//
//

#include "CKiiError.h"

kiicloud::CKiiError::CKiiError(int httpErrorCode, std::string kiiErrorCode)
{
    this->httpErrorCode = httpErrorCode;
    this->kiiErrorCode = &kiiErrorCode;
}