//
//  CKiiUser.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/23.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiUser__
#define __cocos2dx_cpp_IFV3__CKiiUser__

#include <iostream>
#include <future>
#include "CKiiApp.h"
#include "CKiiError.h"
#include "picojson.h"

using kiicloud::ErrorPtr;
using kiicloud::ErrorFuture;

namespace kiicloud {
class CKiiUser;
typedef std::shared_ptr<CKiiUser> UserPtr;
typedef std::pair<UserPtr, ErrorPtr> UserAndError;
typedef std::future<UserAndError> UserFuture;

class CKiiUser {
public:
    explicit CKiiUser();
    explicit CKiiUser(const picojson::value& keyValues);
    ~CKiiUser();
    picojson::object getKeyValues() const;
    std::string getAccessToken() const;
    std::string getId() const;
    std::string getUri() const;

    static UserFuture login(
                            const CKiiApp& app,
                            const std::string& username,
                            const std::string& password);

    static UserFuture registerNewUser(const CKiiApp& app,
                                      const std::string& username,
                                      const std::string& password);

    static ErrorFuture refresh(const kiicloud::CKiiApp& app,
                              kiicloud::CKiiUser& user);

private:
    picojson::object keyValues;
    std::string appId;
    CKiiSite appSite;
    std::string accessToken;
    std::string userId;
};

};

#endif /* defined(__cocos2dx_cpp_IFV3__CKiiUser__) */
