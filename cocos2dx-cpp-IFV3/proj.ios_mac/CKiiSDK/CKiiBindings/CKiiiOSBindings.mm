//
//  CKiiiOSBindings.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/22.
//
//

#include "CKiiiOSBindings.h"
#include "CKiiAuth.h"
#import <KiiSDK/Kii.h>

KiiSite convertSite(kiicloud::CKiiSite csite)
{
    switch(csite)
    {
        case kiicloud::cKiiSiteJP:
            return kiiSiteJP;
        case kiicloud::cKiiSiteUS:
            return kiiSiteUS;
        case kiicloud::cKiiSiteCN:
            return kiiSiteCN;
        case kiicloud::cKiiSiteSG:
            // TODO: update sdk.
            return kiiSiteCN;
    }
    
}

NSString* convertString(std::string stdStr)
{
    return [NSString stringWithCString:stdStr.c_str() encoding:NSUTF8StringEncoding];
}

kiicloud::CKiiiOSBindings::CKiiiOSBindings()
{
};

void kiicloud::CKiiiOSBindings::login(
                                      const std::string& appId,
                                      const std::string& appKey,
                                      const CKiiSite& appSite,
                                      const std::string& username,
                                      const std::string& password,
                                      const picojson::object& data,
                                      const std::function<void (std::shared_ptr<CKiiUser> auth, std::shared_ptr<CKiiError> error)> loginCallback)
{
    NSString * aid = convertString(appId);
    NSString * akey = convertString(appKey);
    [Kii beginWithID:aid andKey:akey andSite:convertSite(appSite)];
    [Kii setLogLevel:3];
    
    NSString * uname = convertString(username);
    NSString * upass = convertString(password);
    
    // TODO: parse data.
    [KiiUser authenticate:uname withPassword:upass andBlock:^(KiiUser *user, NSError *error) {
        if (error != nil)
        {
            NSString* sCode = error.userInfo[@"server_code"];
            std::shared_ptr<CKiiError> errP(new CKiiError(0, [sCode cStringUsingEncoding:NSUTF8StringEncoding]));
            loginCallback(nullptr, errP);
            return;
        }
        // TODO: give some useful properties to CKiiUser.
        std::shared_ptr<CKiiUser> userP(new CKiiUser());
        loginCallback(userP, nullptr);
    }];
}

void kiicloud::CKiiiOSBindings::registerNewUser(
                                                const std::string& appId,
                                                const std::string& appKey,
                                                const CKiiSite& appSite,
                                                const std::string& username,
                                                const std::string& password,
                                                const picojson::object& data,
                                                const std::function<void (std::shared_ptr<CKiiUser> auth, std::shared_ptr<CKiiError> error)> loginCallback)
{
    NSString * aid = convertString(appId);
    NSString * akey = convertString(appKey);
    [Kii beginWithID:aid andKey:akey andSite:convertSite(appSite)];
    [Kii setLogLevel:3];

    NSString * uname = convertString(username);
    NSString * upass = convertString(password);
    KiiUser *kuser = [KiiUser userWithUsername:uname andPassword:upass];

    // TODO: parse data.
    [kuser performRegistrationWithBlock:^(KiiUser *user, NSError *error) {
        if (error != nil)
        {
            NSString* sCode = error.userInfo[@"server_code"];
            std::shared_ptr<CKiiError> errP(new CKiiError(0, [sCode cStringUsingEncoding:NSUTF8StringEncoding]));
            loginCallback(nullptr, errP);
            return;
        }
        std::shared_ptr<CKiiUser> userP(new CKiiUser());
        loginCallback(userP, nullptr);
    }];
    
}