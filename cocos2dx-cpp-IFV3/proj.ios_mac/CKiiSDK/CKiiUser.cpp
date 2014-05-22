//
//  CKiiUser.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/21.
//
//

#include "CKiiUser.h"

void kiicloud::CKiiUser::login(
                  const std::string& appId,
                  const std::string& appKey,
                  const CKiiSite& appSite,
                  const std::string& username,
                  const std::string& password,
                  const picojson::object& data,
                  const std::function<void (std::shared_ptr<CKiiUser> authenticatedUser, std::shared_ptr<CKiiError> error)> loginCallback)
{
    // TODO: implement
}

void kiicloud::CKiiUser::registerNewUser(
                                         const std::string& appId,
                                         const std::string& appKey,
                                         const CKiiSite& appSite,
                                         const std::string& username,
                                         const std::string& password,
                                         const picojson::object& data,
                                         const std::function<void (std::shared_ptr<CKiiUser> authenticatedUser, std::shared_ptr<CKiiError> error)> registerCallback)
{
    // TODO: implement
}

kiicloud::CKiiBucket* kiicloud::CKiiUser::appScopeBucket(const std::string& bucketName)
{
    // TODO: implement
    return nullptr;
}