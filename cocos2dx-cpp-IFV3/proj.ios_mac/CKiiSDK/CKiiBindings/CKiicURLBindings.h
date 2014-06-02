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
#include "CKiiBaseBindings.h"

namespace kiicloud {

class CKiicURLBindings : public CKiiBaseBindings
{

public:
    CKiicURLBindings();
    CKiicURLBindings(const CKiicURLBindings& lv);
    CKiicURLBindings(CKiicURLBindings&& lv);
    ~CKiicURLBindings();
    void login(
               const std::string& appId,
               const std::string& appKey,
               const CKiiSite& appSite,
               const std::string& username,
               const std::string& password,
               const picojson::object& data,
               const std::function<void (CKiiUser *auth, CKiiError *error)> loginCallback);
    
    void registerNewUser(
                         const std::string& appId,
                         const std::string& appKey,
                         const CKiiSite& appSite,
                         const std::string& username,
                         const std::string& password,
                         const picojson::object& data,
                         const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> registerCallback);
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiicURLBindings__) */
