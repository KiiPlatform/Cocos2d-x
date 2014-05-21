//
//  CKiiUserBind.m
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/21.
//
//

#include "CKiiUserBind.h"
#import <KiiSDK/Kii.h>

void CKiiUserBind::login(
                     const std::string& username,
                     const std::string& password,
                     const std::function<void (picojson::object result)> loginCallback)
{
    NSString *uname = [NSString stringWithUTF8String:username.c_str()];
    NSString *upass = [NSString stringWithUTF8String:password.c_str()];

    [KiiUser authenticate:uname withPassword:upass andBlock:^(KiiUser *user, NSError *error) {
        picojson::object retError;
        if (error != nil) {
            NSDictionary *errorDict = @{@"code": [NSNumber numberWithInt:error.code]};
            const std::string sErrorDict = std::string([[errorDict description]cStringUsingEncoding:NSUTF8StringEncoding]);
            retError["_error_"] = picojson::value(sErrorDict);
        } else {
            // Success.
        }
        loginCallback(retError);
    }];
    
}

void CKiiUserBind::registerNewUser(
                               const std::string& username,
                               const std::string& password,
                               const std::function<void (picojson::object result)> registerCallback)
{
    NSLog(@"CKiiUserBind::registerNewUser");
    NSString *uname = [NSString stringWithUTF8String:username.c_str()];
    NSString *upass = [NSString stringWithUTF8String:password.c_str()];
    KiiUser *kuser = [KiiUser userWithUsername:uname andPassword:upass];
    [kuser performRegistrationWithBlock:^(KiiUser *user, NSError *error) {
        NSLog(@"CKiiUserBind::registerNewUser callback.");
        picojson::object retError;
        if (error != nil) {
            NSDictionary *errorDict = @{@"code": [NSNumber numberWithInt:error.code]};
            const std::string sErrorDict = std::string([[errorDict description]cStringUsingEncoding:NSUTF8StringEncoding]);
            retError["_error_"] = picojson::value(sErrorDict);
        } else {
            // Success
        }
        registerCallback(retError);
    }];
}
