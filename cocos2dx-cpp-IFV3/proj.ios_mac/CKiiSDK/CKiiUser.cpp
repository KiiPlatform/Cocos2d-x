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
        _bind->login(app.appId, app.appKey, app.appSite, username, password, data,
                    [=, &app, &username, &password, &data] (CKiiUser *aUser, CKiiError* e) {
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
        _bind->registerNewUser(app.appId, app.appKey, app.appSite, username, password, data,
                              [=, &app, &username, &password, &data] (CKiiUser *aUser, CKiiError* e) {
                                  registerCallback(aUser, e);
                                  th1->detach();
                                  delete th1;
                              });
    });
    th1->swap(thd);
}

void kiicloud::CKiiUser::refresh(const kiicloud::CKiiApp& app,
                                 kiicloud::CKiiUser& user,
                                 const std::function<void (CKiiUser *refreshedUser, CKiiError *error)>)
{
    
    
}