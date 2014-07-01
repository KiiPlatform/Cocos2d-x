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
#include "CKiiObject.h"

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
                             const std::string &scopeUri,
                             const std::string &bucketName,
                             const CKiiQuery &query,
                             const std::string& accessToken,
                             std::function<void (picojson::value result,
                                                 CKiiError* error)> queryCallback) = 0;

    virtual void saveNewObject(const CKiiApp& app,
                               const std::string &scopeUri,
                               const std::string &bucketName,
                               const picojson::object &values,
                               const std::string& accessToken,
                               const std::function<void (picojson::value, std::string &etag, CKiiError*)> saveCallback) = 0;

    virtual void saveNewObjectWithID(const CKiiApp& app,
                                     const std::string &scopeUri,
                                     const std::string &bucketName,
                                     const std::string &objectID,
                                     const picojson::object &values,
                                     const std::string& accessToken,
                                     kiicloud::CKiiObject::SaveMode saveMode,
                                     const std::function<void (picojson::value, std::string &etag, CKiiError*)> saveCallback) = 0;

    virtual void patchObject(const CKiiApp& app,
                             const std::string &objUri,
                             const std::string &objVersion,
                             const picojson::object &values,
                             const std::string& accessToken,
                             bool forceUpdate,
                             const std::function<void (picojson::value, CKiiError*)> patchCallback) = 0;

    virtual void replaceObjectValuesWithNewValues(const CKiiApp &app,
                                          const std::string &objUri,
                                          const std::string &objVersion,
                                          const picojson::object &newValues,
                                          const std::string &accessToken,
                                          bool forceUpdate,
                                          const std::function<void (picojson::value values,
                                                                    std::string &etag,
                                                                    CKiiError *error)> replaceCallback
                                          ) = 0;

    virtual void refreshObject(const CKiiApp &app,
                               const std::string &objUri,
                               const std::string &accessToken,
                               const std::function<void (picojson::value values,
                                                         CKiiError *error)> refreshCallback
                               ) = 0;

    virtual void deleteObject(const CKiiApp &app,
                               const std::string &objUri,
                               const std::string &accessToken,
                               const std::function<void (CKiiError *error)> refreshCallback
                               ) = 0;

};

};
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiBaseBindings__) */
