//
//  CKiiUser.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/21.
//
//

#include "CKiiUser.h"

CKiiUser::CKiiUser()
{
    bind = new CKiiUserBind();
}

CKiiUser::~CKiiUser()
{
    delete bind;
}

void CKiiUser::login(
                     const std::string& username,
                     const std::string& password,
                     const std::function<void (picojson::object result)> loginCallback)
{
    bind->login(username, password, loginCallback);
}

void CKiiUser::registerNewUser(
                               const std::string& username,
                               const std::string& password,
                               const std::function<void (picojson::object result)> registerCallback)
{
    bind->registerNewUser(username, password, registerCallback);
}