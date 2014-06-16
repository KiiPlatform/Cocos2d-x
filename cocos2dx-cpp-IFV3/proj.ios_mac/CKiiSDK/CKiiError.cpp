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

using kiicloud::CKiiError;

kiicloud::CKiiError::~CKiiError()
{
    delete kiiErrorCode;
}

kiicloud::CKiiError::CKiiError()
{
    httpErrorCode = 0;
    kiiErrorCode = new std::string("");
}

kiicloud::CKiiError::CKiiError(const CKiiError& rhs)
{
    httpErrorCode = rhs.httpErrorCode;
    kiiErrorCode = new std::string(*rhs.kiiErrorCode);
}

kiicloud::CKiiError::CKiiError(CKiiError&& rhs)
{
    httpErrorCode = rhs.httpErrorCode;
    kiiErrorCode = new std::string(*rhs.kiiErrorCode);
    
    delete rhs.kiiErrorCode;
    rhs.kiiErrorCode = nullptr;
    rhs.httpErrorCode = 0;
}

kiicloud::CKiiError::CKiiError(int httpErrorCode, const std::string& kiiErrorCode)
{
    this->httpErrorCode = httpErrorCode;
    this->kiiErrorCode = new std::string(kiiErrorCode);
}

std::string kiicloud::CKiiError::getKiiErrorCode()
{
    return *kiiErrorCode;
}

int kiicloud::CKiiError::getHttpErrorCode()
{
    return httpErrorCode;
}

std::string kiicloud::CKiiError::toString()
{
    std::stringstream ss;
    ss << "CKiiError(" << "httpErrorCode: " << httpErrorCode << ", kiiErrorCode: " << *kiiErrorCode << ")";
    return ss.str();
}