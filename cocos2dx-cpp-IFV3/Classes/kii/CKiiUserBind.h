//
//  CKiiUserBind.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/21.
//
//

#include "picojson.h"
#include <iostream>
#include <functional>

class CKiiUserBind
{
public:
    void login(const std::string& username,
               const std::string& password,
               const std::function<void (picojson::object result)> loginCallback);

    void registerNewUser(const std::string& username,
                         const std::string& password,
                         const std::function<void (picojson::object result)> registerCallback);
};

