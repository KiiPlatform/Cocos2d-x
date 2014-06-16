//
//  CKiiUser.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/23.
//
//

#include "CKiiUser.h"
#include "_CKiiGlobal.h"
#include <thread>

using kiicloud::UserPtr;
using kiicloud::ErrorPtr;
using kiicloud::UserFuture;
using kiicloud::UserAndError;

kiicloud::CKiiUser::CKiiUser()
{
}

kiicloud::CKiiUser::CKiiUser(const picojson::value& keyValues)
{
    
    this->keyValues = keyValues.get<picojson::object>();
    if (this->keyValues["access_token"].is<std::string>()) {
        this->accessToken = this->keyValues["access_token"].get<std::string>();
    }
    if (this->keyValues["id"].is<std::string>()) {
        this->userId = this->keyValues["id"].get<std::string>();
    } else if (this->keyValues["userID"].is<std::string>()) {
        this->userId = this->keyValues["userID"].get<std::string>();
    }
}

kiicloud::CKiiUser::~CKiiUser()
{
}

picojson::object kiicloud::CKiiUser::getKeyValues() const
{
    return keyValues;
}

UserFuture kiicloud::CKiiUser::login(
                                     const CKiiApp& app,
                                     const std::string& username,
                                     const std::string& password)
{
    auto *prm = new std::promise<UserAndError>();
    std::thread th1 = std::thread([=]() {
        picojson::object data;
        _bind->login(app, username, password, data,
                     [=, &app, &username, &password, &data] (CKiiUser *aUser, CKiiError* e) {
                         aUser->appId = app.appId;
                         aUser->appSite = app.appSite;
                         auto pair = UserAndError(UserPtr(aUser), ErrorPtr(e));
                         prm->set_value(pair);
                         delete prm;
                     });
    });
    th1.detach();
    return prm->get_future();
}

UserFuture kiicloud::CKiiUser::registerNewUser(const kiicloud::CKiiApp &app,
                                               const std::string &username,
                                               const std::string &password)
{
    auto *prm = new std::promise<UserAndError>();
    std::thread th1 = std::thread([=]() {
        picojson::object data;
        _bind->registerNewUser(app, username, password, data,
                     [=, &app, &username, &password, &data] (CKiiUser *aUser, CKiiError* e) {
                         aUser->appId = app.appId;
                         aUser->appSite = app.appSite;
                         auto pair = UserAndError(UserPtr(aUser), ErrorPtr(e));
                         prm->set_value(pair);
                         delete prm;
                     });
    });
    th1.detach();
    return prm->get_future();
}

std::future<ErrorPtr> kiicloud::CKiiUser::refresh(const kiicloud::CKiiApp &app,
                                       kiicloud::CKiiUser& user)
{
    auto *prm = new std::promise<ErrorPtr>();
    std::thread th1 = std::thread([=, &app, &user]() {
        _bind->refreshUser(app, user, [=, &app, &user] (picojson::value kvs, CKiiError *err) {
            if (kvs.is<picojson::object>()) {
                user.keyValues = kvs.get<picojson::object>();
            }
            prm->set_value(ErrorPtr(err));
            delete prm;
        });
    });
    th1.detach();
    return prm->get_future();
}

std::string kiicloud::CKiiUser::getId() const
{
    return this->userId;
}

std::string kiicloud::CKiiUser::getAccessToken() const
{
    return this->accessToken;
}

std::string kiicloud::CKiiUser::getUri() const
{
    std::string baseUrl = kiicloud::getBaseUrl(this->appSite);
    return baseUrl + "/users/" + this->userId;
}
