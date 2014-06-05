//
//  CKiiUser.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/21.
//
//

#include "CKiiUserAsyncFactory.h"
#include "CKiiiOSBindings.h"
#include "CKiicURLBindings.h"
#include "CKiiLog.h"
#include <thread>

kiicloud::CKiiUserAsyncFactory::CKiiUserAsyncFactory()
{
    bind = new CKiicURLBindings();
}

kiicloud::CKiiUserAsyncFactory::CKiiUserAsyncFactory(const CKiiUserAsyncFactory& lv) : bind(new CKiicURLBindings())
{
}

kiicloud::CKiiUserAsyncFactory::CKiiUserAsyncFactory(CKiiUserAsyncFactory&& lv) : bind (new CKiicURLBindings())
{
    lv.bind = nullptr;
}

kiicloud::CKiiUserAsyncFactory::~CKiiUserAsyncFactory()
{
    delete bind;
}

void kiicloud::CKiiUserAsyncFactory::login(
                  const kiicloud::CKiiApp& app,
                  const std::string& username,
                  const std::string& password,
                  const picojson::object& data,
                  const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> loginCallback)
{
    std::thread *th1 = new std::thread();
    std::thread thd = std::thread([=, &app, &username, &password, &data]() {
        bind->login(app.appId, app.appKey, app.appSite, username, password, data,
                    [=, &app, &username, &password, &data] (CKiiUser *aUser, CKiiError* e) {
                        loginCallback(aUser, e);
                        th1->detach();
                        delete th1;
                    });
    });
    th1->swap(thd);
}

void kiicloud::CKiiUserAsyncFactory::registerNewUser(
                                         const kiicloud::CKiiApp& app,
                                         const std::string& username,
                                         const std::string& password,
                                         const picojson::object& data,
                                         const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> registerCallback)
{
    std::thread *th1 = new std::thread();
    std::thread thd = std::thread([=, &app, &username, &password, &data]() {
        bind->registerNewUser(app.appId, app.appKey, app.appSite, username, password, data,
                              [=, &app, &username, &password, &data] (CKiiUser *aUser, CKiiError* e) {
                                  registerCallback(aUser, e);
                                  th1->detach();
                                  delete th1;
                              });
    });
    th1->swap(thd);
}
