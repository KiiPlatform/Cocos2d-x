//
//  CKiiError.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/22.
//
//

#include <iostream>
#include <sstream>
#include "CKiiError.h"

kiicloud::CKiiError::~CKiiError()
{
}

kiicloud::CKiiError::CKiiError(int httpErrorCode, std::string kiiErrorCode)
{
    this->httpErrorCode = httpErrorCode;
    this->kiiErrorCode = kiiErrorCode;
}

std::string kiicloud::CKiiError::getKiiErrorCode()
{
    return kiiErrorCode;
}

int kiicloud::CKiiError::getHttpErrorCode()
{
    return httpErrorCode;
}

std::string kiicloud::CKiiError::toString()
{
    std::stringstream ss;
    ss << "CKiiError(" << "httpErrorCode: " << httpErrorCode << ", kiiErrorCode: " << kiiErrorCode << ")";
    return ss.str();
}