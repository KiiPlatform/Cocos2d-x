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
    bind = new CKiicURLBindings();
}

kiicloud::CKiiUserAsyncFactory::CKiiUserAsyncFactory(const CKiiUserAsyncFactory& lv) : bind(new CKiicURLBindings())
{
}

kiicloud::CKiiUserAsyncFactory::CKiiUserAsyncFactory(CKiiUserAsyncFactory&& lv) : bind (new CKiicURLBindings())
{
    lv.bind = nullptr;
    lv.idMap.clear();
    lv.threadPool.clear();
}

kiicloud::CKiiUserAsyncFactory::~CKiiUserAsyncFactory()
{
    delete bind;
}

void kiicloud::CKiiUserAsyncFactory::login(
                  const std::string& appId,
                  const std::string& appKey,
                  const CKiiSite& appSite,
                  const std::string& username,
                  const std::string& password,
                  const picojson::object& data,
                  const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> loginCallback)
{
    int i = ++taskId;
    std::thread *thd = new std::thread([=, &appId, &appKey, &username, &password, &data]() {
        bind->login(appId, appKey, appSite, username, password, data,
                    [=, &appId, &appKey, &username, &password, &data] (CKiiUser *aUser, CKiiError* e) {
                        loginCallback(aUser, e);
                        // TODO: make it thread safe.
                        std::thread::id th_id = idMap[i];
                        threadPool[th_id] -> detach();
                        delete threadPool[th_id];
                    });
    });
    idMap.insert(std::map<int, std::thread::id>::value_type(i, thd->get_id()));
    threadPool.insert(std::map<std::thread::id, std::thread*>::value_type(thd->get_id(), thd));
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
