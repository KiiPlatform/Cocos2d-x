//
//  CKiiAuth2.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/22.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiAuth__
#define __cocos2dx_cpp_IFV3__CKiiAuth__

#include <iostream>
#include "picojson.h"
#include "CKiiError.h"
#include "CKiiBucket.h"

namespace kiicloud {

enum CKiiSite {
    cKiiSiteJP,
    cKiiSiteUS,
    cKiiSiteCN,
    cKiiSiteSG
};

class CKiiAuth
{
public:
    static void authenticateAsAppAdmin(
                                       const std::string& appId,
                                       const std::string& appKey,
                                       const CKiiSite site,
                                       picojson::object& credentials,
                                       std::function<void (CKiiAuth* kiiAC, CKiiError* error)> authCallback
                                       );
    
    std::string* getAppId();
    std::string* getAppKey();
    CKiiSite getAppSite();
    std::string* getAccessToken();
    
    CKiiBucket* appScopeBucket(const std::string& bucketName);
};

};
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiAuth__) */
