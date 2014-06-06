//
//  CKiiApp.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/05.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiApp__
#define __cocos2dx_cpp_IFV3__CKiiApp__

#include <iostream>

namespace kiicloud {

enum CKiiSite {
    cKiiSiteJP,
    cKiiSiteUS,
    cKiiSiteCN,
    cKiiSiteSG
};

static std::string getBaseUrl(const CKiiSite& appSite)
{
    switch(appSite)
    {
        case kiicloud::cKiiSiteCN:
            return "https://api-cn2.kii.com/api";
        case kiicloud::cKiiSiteJP:
            return "https://api-jp.kii.com/api";
        case kiicloud::cKiiSiteUS:
            return "https://api.kii.com/api";
        case kiicloud::cKiiSiteSG:
            return "https://api-sg.kii.com/api";
    }
}

struct CKiiApp
{

public:
    CKiiApp(const CKiiApp& lv)
    {
        this->appId = std::string(lv.appId);
        this->appKey = std::string(lv.appKey);
        this->appSite = lv.appSite;
    }

    CKiiApp(CKiiApp&& lv)
    {
        this->appId = std::string(lv.appId);
        lv.appId = "";
        this->appKey = std::string(lv.appKey);
        lv.appKey = "";
        this->appSite = lv.appSite;
        lv.appSite = cKiiSiteJP;
    }

    explicit CKiiApp(const std::string& appId, const std::string& appKey, const kiicloud::CKiiSite& appSite)
    {
        this->appId = std::string(appId);
        this->appKey = std::string(appKey);
        this->appSite = appSite;
    }

    std::string appUrl() const {
        std::string baseUrl = getBaseUrl(this->appSite);
        return baseUrl + "/apps/" + this->appId;
    }

    std::string appId;
    std::string appKey;
    kiicloud::CKiiSite appSite;
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiApp__) */
