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
    void login(std::string* username,
               std::string* password,
               std::function<void (picojson::object result)> loginCallback);
    
    void registerNewUser(std::string* username,
                         std::string* password,
                         std::function<void (picojson::object result)> registerCallback);
};

