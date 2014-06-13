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
#include "CKiiQuery.h"

namespace kiicloud
{

class CKiiBaseBindings
{
public:
    virtual ~CKiiBaseBindings();
    virtual void login(
                       const CKiiApp& app,
                       const std::string& username,
                       const std::string& password,
                       const picojson::object& data,
                       const std::function<void (CKiiUser *auth, CKiiError *error)> loginCallback) = 0;
    
    virtual void registerNewUser(
                                 const CKiiApp& app,
                                 const std::string& username,
                                 const std::string& password,
                                 const picojson::object& data,
                                 const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> loginCallback) = 0;
    
    virtual void refreshUser(const CKiiApp& app,
                             CKiiUser& user,
                             std::function<void (picojson::value keyValues, CKiiError* error)> refreshCallback) = 0;

    virtual void queryBucket(const CKiiApp& app,
                             const std::string& scopeUri,
                             const std::string& bucketName,
                             const CKiiQuery& query,
                             const std::string& accessToken,
                             std::function<void (picojson::value result,
                                                 CKiiError* error)> queryCallback) = 0;

    virtual void saveNewObject(const CKiiApp& app,
                       const std::string &scopeUri,
                       const std::string &bucketName,
                       const picojson::object values,
                       const std::string& accessToken,
                       const std::function<void (picojson::value, CKiiError *error)> saveCallback) = 0;
};

};
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiBaseBindings__) */
