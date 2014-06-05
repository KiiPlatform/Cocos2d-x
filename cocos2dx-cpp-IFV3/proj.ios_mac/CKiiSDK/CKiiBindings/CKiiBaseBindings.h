//
//  CKiiBaseBindings.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/22.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiBaseBindings__
#define __cocos2dx_cpp_IFV3__CKiiBaseBindings__

#include <iostream>
#include "CKiiApp.h"
#include "CKiiUser.h"
#include "CKiiError.h"

namespace kiicloud
{

class CKiiBaseBindings
{
public:
    virtual ~CKiiBaseBindings();
    virtual void login(
                       const std::string& appId,
                       const std::string& appKey,
                       const CKiiSite& appSite,
                       const std::string& username,
                       const std::string& password,
                       const picojson::object& data,
                       const std::function<void (CKiiUser *auth, CKiiError *error)> loginCallback) = 0;
    
    virtual void registerNewUser(
                                 const std::string& appId,
                                 const std::string& appKey,
                                 const CKiiSite& appSite,
                                 const std::string& username,
                                 const std::string& password,
                                 const picojson::object& data,
                                 const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> loginCallback) = 0;
};

};
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiBaseBindings__) */
