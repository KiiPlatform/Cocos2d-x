//
//  CKiiUser.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/21.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiUser__
#define __cocos2dx_cpp_IFV3__CKiiUser__

#include <iostream>
#include "KBase.h"
#include "picojson.h"
#include "CKiiUserBind.h"

class CKiiUser : public KBase
{
public:
    CKiiUser();
    ~CKiiUser();
    void login(
               const std::string& username,
               const std::string& password,
               const std::function<void (picojson::object result)> loginCallback);

    void registerNewUser(
                         const std::string& username,
                         const std::string& password,
                         const std::function<void (picojson::object result)> registerCallback);

private:
    // TODO: use shared_ptr.
    CKiiUserBind *bind;
};

#endif /* defined(__cocos2dx_cpp_IFV3__CKiiUser__) */
