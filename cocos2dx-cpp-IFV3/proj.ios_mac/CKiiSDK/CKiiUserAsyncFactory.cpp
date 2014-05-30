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
#include <thread>

kiicloud::CKiiUserAsyncFactory::CKiiUserAsyncFactory()
{
//    bind = new CKiiiOSBindings();
    bind = new CKiicURLBindings();
}

kiicloud::CKiiUserAsyncFactory::CKiiUserAsyncFactory(const CKiiUserAsyncFactory& lv) :  bind { new CKiicURLBindings() }
{
}

kiicloud::CKiiUserAsyncFactory::CKiiUserAsyncFactory(CKiiUserAsyncFactory&& lv) : bind { new CKiicURLBindings() }
{
    lv.bind = nullptr;
}

kiicloud::CKiiUserAsyncFactory::~CKiiUserAsyncFactory()
{
    delete bind;
}

void hoge(std::shared_ptr<kiicloud::CKiiUser> user, std::shared_ptr<kiicloud::CKiiError> error)
{
    
}

void kiicloud::CKiiUserAsyncFactory::login(
                  const std::string& appId,
                  const std::string& appKey,
                  const CKiiSite& appSite,
                  const std::string& username,
                  const std::string& password,
                  const picojson::object& data,
                  const std::function<void (std::shared_ptr<CKiiUser> authenticatedUser, std::shared_ptr<CKiiError> error)> loginCallback)
{
    std::thread *thd = new std::thread([&]() {
        bind->login(appId, appKey, appSite, username, password, data,
                    [&] (std::shared_ptr<CKiiUser> aUser, std::shared_ptr<CKiiError> error) {
                        loginCallback(aUser, error);
                        myThread->detach();
                        delete myThread;
                    });
    });
    myThread = thd;
}

void kiicloud::CKiiUserAsyncFactory::registerNewUser(
                                         const std::string& appId,
                                         const std::string& appKey,
                                         const CKiiSite& appSite,
                                         const std::string& username,
                                         const std::string& password,
                                         const picojson::object& data,
                                         const std::function<void (std::shared_ptr<CKiiUser> authenticatedUser, std::shared_ptr<CKiiError> error)> registerCallback)
{
    bind->registerNewUser(appId, appKey, appSite, username, password, data, registerCallback);
}
