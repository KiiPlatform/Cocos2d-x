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
                     std::string* username,
                     std::string* password,
                     std::function<void (picojson::object result)> loginCallback)
{
    bind->login(username, password, loginCallback);
}

void CKiiUser::registerNewUser(
                               std::string* username,
                               std::string* password,
                               std::function<void (picojson::object result)> registerCallback)
{
    bind->registerNewUser(username, password, registerCallback);
}