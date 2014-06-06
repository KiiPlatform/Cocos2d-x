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
#include "CKiiApp.h"
#include "CKiiError.h"
#include "picojson.h"

namespace kiicloud {

class CKiiUser {
public:
    explicit CKiiUser();
    explicit CKiiUser(const picojson::value& keyValues);
    ~CKiiUser();
    picojson::object getKeyValues() const;
    std::string getAccessToken() const;
    std::string getId() const;
    std::string getUri() const;

    static void login(
                      const CKiiApp& app,
                      const std::string& username,
                      const std::string& password,
                      const picojson::object& data,
                      const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> loginCallback);

    static void registerNewUser(
                                const CKiiApp& app,
                                const std::string& username,
                                const std::string& password,
                                const picojson::object& data,
                                const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)>);

    static void refresh(const kiicloud::CKiiApp& app,
                        kiicloud::CKiiUser& user,
                        const std::function<void (CKiiUser *refreshedUser, CKiiError *error)>);

private:
    picojson::object keyValues;
    std::string appId;
    CKiiSite appSite;
    std::string accessToken;
    std::string userId;
};

};

#endif /* defined(__cocos2dx_cpp_IFV3__CKiiUser__) */
