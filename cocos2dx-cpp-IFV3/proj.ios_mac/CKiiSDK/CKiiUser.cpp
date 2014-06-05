//
//  CKiiUser.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/23.
//
//

#include "CKiiUser.h"

kiicloud::CKiiUser::CKiiUser()
{
}

kiicloud::CKiiUser::CKiiUser(const picojson::value& keyValues)
{
    this->keyValues = keyValues.get<picojson::object>();
}

kiicloud::CKiiUser::~CKiiUser()
{
}

picojson::object kiicloud::CKiiUser::getKeyValues()
{
    return keyValues;
}