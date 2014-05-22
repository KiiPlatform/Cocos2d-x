//
//  CKiiiOSBindings.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/22.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiiOSBindings__
#define __cocos2dx_cpp_IFV3__CKiiiOSBindings__

#include <iostream>
#include "CKiiBaseBindings.h"

namespace kiicloud {

class CKiiiOSBindings : public CKiiBaseBindings
{
public:
    CKiiiOSBindings();
    void login(
               const std::string& appId,
               const std::string& appKey,
               const CKiiSite& appSite,
               const std::string& username,
               const std::string& password,
               const picojson::object& data,
               const std::function<void (std::shared_ptr<CKiiUser> auth, std::shared_ptr<CKiiError> error)> loginCallback);

    void registerNewUser(
                         const std::string& appId,
                         const std::string& appKey,
                         const CKiiSite& appSite,
                         const std::string& username,
                         const std::string& password,
                         const picojson::object& data,
                         const std::function<void (std::shared_ptr<CKiiUser> auth, std::shared_ptr<CKiiError> error)> registerCallback);
};

};
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiiOSBindings__) */
