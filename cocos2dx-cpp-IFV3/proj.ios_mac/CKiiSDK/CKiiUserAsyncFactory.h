//
//  CKiiUser.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/21.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiUserAsyncFactory__
#define __cocos2dx_cpp_IFV3__CKiiUserAsyncFactory__

#include <iostream>
#include <thread>
#include <set>
#include "KBase.h"
#include "picojson.h"
#include "CKiiBaseBindings.h"
#include "CKiiAuth.h"
#include "CKiiError.h"
#include "CKiiBucket.h"

namespace kiicloud
{

class CKiiUserAsyncFactory
{
public:
    CKiiUserAsyncFactory();
    CKiiUserAsyncFactory(const CKiiUserAsyncFactory& lv);
    CKiiUserAsyncFactory(CKiiUserAsyncFactory&& lv);
    ~CKiiUserAsyncFactory();
    void login(
               const std::string& appId,
               const std::string& appKey,
               const CKiiSite& appSite,
               const std::string& username,
               const std::string& password,
               const picojson::object& data,
               const std::function<void (std::shared_ptr<CKiiUser> authenticatedUser, std::shared_ptr<CKiiError> error)> loginCallback);

    void registerNewUser(
                         const std::string& appId,
                         const std::string& appKey,
                         const CKiiSite& appSite,
                         const std::string& username,
                         const std::string& password,
                         const picojson::object& data,
                         const std::function<void (std::shared_ptr<CKiiUser> authenticatedUser, std::shared_ptr<CKiiError> error)>);

private:
    CKiiBaseBindings *bind;
//    std::set<std::thread*> threadPool;
    std::thread *myThread;
};

} // namespace kiicloud
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiUserAsyncFactory__) */
