//
//  CKiicURLBindings.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/27.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiicURLBindings__
#define __cocos2dx_cpp_IFV3__CKiicURLBindings__

#include <iostream>
#include <map>
#include "CKiiBaseBindings.h"
#include "CKiiObject.h"

namespace kiicloud {

// TODO: unify api style of Bindings.
class CKiicURLBindings : public CKiiBaseBindings
{

public:
    enum Method{
        POST,
        PUT,
        GET,
        DELETE,
    };
    CKiicURLBindings();
    CKiicURLBindings(const CKiicURLBindings& lv);
    CKiicURLBindings(CKiicURLBindings&& lv);
    ~CKiicURLBindings();
    void login(
               const CKiiApp& app,
               const std::string& username,
               const std::string& password,
               const picojson::object& data,
               const std::function<void (CKiiUser *auth, CKiiError *error)> loginCallback);
    
    void registerNewUser(
                         const CKiiApp& app,
                         const std::string& username,
                         const std::string& password,
                         const picojson::object& data,
                         const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> registerCallback);
    
    void refreshUser(const kiicloud::CKiiApp& app,
                     kiicloud::CKiiUser& user,
                     std::function<void (picojson::value keyValues, CKiiError* error)> refreshCallback);

    void queryBucket(const CKiiApp& app,
                     const std::string& scopeUri,
                     const std::string& bucketName,
                     const CKiiQuery& query,
                     const std::string& accessToken,
                     const std::function<void (picojson::value result,
                                         CKiiError* error)> queryCallback);
    
    void saveNewObject(const CKiiApp& app,
                       const std::string &scopeUri,
                       const std::string &bucketName,
                       const picojson::object &values,
                       const std::string& accessToken,
                       const std::function<void (picojson::value, std::string& etag, CKiiError *error)> saveCallback);

    void patchObject(const CKiiApp& app,
                     const std::string &objUri,
                     const std::string &objVersion,
                     const picojson::object &values,
                     const std::string& accessToken,
                     bool forceUpdate,
                     const std::function<void (picojson::value values,
                                               CKiiError *error)> patchCallback
                     );

    void replaceObjectValuesWithNewValues(const CKiiApp &app,
                                          const std::string &objUri,
                                          const std::string &objVersion,
                                          const picojson::object &newValues,
                                          const std::string &accessToken,
                                          bool forceUpdate,
                                          const std::function<void (picojson::value values,
                                                                    std::string& etag,
                                                                    CKiiError *error)> replaceCallback
                                          );

private:
    void request(
                 const Method& method,
                 const std::string& requestUrl,
                 const std::map<std::string, std::string>& requestHeaders,
                 const std::string& requestBody,
                 std::string** responseBody,
                 std::map<std::string, std::string>** responseHeaders,
                 kiicloud::CKiiError** outError
    );
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiicURLBindings__) */
