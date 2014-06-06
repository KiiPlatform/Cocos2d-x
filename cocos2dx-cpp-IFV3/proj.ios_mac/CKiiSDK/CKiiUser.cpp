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

void kiicloud::CKiiUser::login(
                               const CKiiApp& app,
                               const std::string& username,
                               const std::string& password,
                               const picojson::object& data,
                               const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> loginCallback)
{
    std::thread *th1 = new std::thread();
    std::thread thd = std::thread([=, &app, &username, &password, &data]() {
        _bind->login(app, username, password, data,
                    [=, &app, &username, &password, &data] (CKiiUser *aUser, CKiiError* e) {
                        aUser->appId = app.appId;
                        aUser->appSite = app.appSite;
                        loginCallback(aUser, e);
                        th1->detach();
                        delete th1;
                    });
    });
    th1->swap(thd);
}

void kiicloud::CKiiUser::registerNewUser(
                                         const kiicloud::CKiiApp& app,
                                         const std::string& username,
                                         const std::string& password,
                                         const picojson::object& data,
                                         const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> registerCallback)
{
    std::thread *th1 = new std::thread();
    std::thread thd = std::thread([=, &app, &username, &password, &data]() {
        _bind->registerNewUser(app, username, password, data,
                              [=, &app, &username, &password, &data] (CKiiUser *aUser, CKiiError* e) {
                                  aUser->appId = app.appId;
                                  aUser->appSite = app.appSite;
                                  registerCallback(aUser, e);
                                  th1->detach();
                                  delete th1;
                              });
    });
    th1->swap(thd);
}

void kiicloud::CKiiUser::refresh(const kiicloud::CKiiApp& app,
                                 kiicloud::CKiiUser& user,
                                 const std::function<void (CKiiUser *refreshedUser, CKiiError *error)> refreshCallback)
{
    std::thread *th1 = new std::thread();
    std::thread thd = std::thread([=, &app, &user]() {
        _bind->refreshUser(app, user, [&] (picojson::value kvs, CKiiError *err) {
            if (kvs.is<picojson::object>()) {
                user.keyValues = kvs.get<picojson::object>();
            }
            refreshCallback(&user, err);
            th1 -> detach();
            delete th1;
        });
    });
    th1->swap(thd);
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
