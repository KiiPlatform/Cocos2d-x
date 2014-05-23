//
//  CKiiUser.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/21.
//
//

#include "CKiiUserAsyncFactory.h"
#include "CKiiiOSBindings.h"

kiicloud::CKiiUserAsyncFactory::CKiiUserAsyncFactory()
{
    bind = new CKiiiOSBindings();
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
                  const std::function<void (std::shared_ptr<CKiiUser> authenticatedUser, std::shared_ptr<CKiiError> error)> loginCallback)
{
    bind->login(appId, appKey, appSite, username, password, data,
                loginCallback);
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
