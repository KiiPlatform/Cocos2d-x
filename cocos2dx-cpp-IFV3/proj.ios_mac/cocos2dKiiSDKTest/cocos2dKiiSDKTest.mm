//
//  cocos2dKiiSDKTest.m
//  cocos2dKiiSDKTest
//
//  Created by 熊野 聡 on 2014/05/21.
//
//

#import <XCTest/XCTest.h>
#import "CKiiBucket.h"
#import "CKiiUser.h"
#import "MyKBase.h"
#import "picojson.h"
#import "LatchedExecuter.h"
#import <KiiSDK/Kii.h>
#import <Foundation/Foundation.h>

@interface cocos2dx_cpp_IFV3_iOS_Tests : XCTestCase
{
@private
    MyKBase *base;
}
@end

NSString *APPID = @"551d82a9";
NSString *APPKEY = @"675bb7fbe71d562c5278ed999e61a800";
KiiSite APPSITE = kiiSiteJP;

@implementation cocos2dx_cpp_IFV3_iOS_Tests

- (void)setUp
{
    [super setUp];
    base = new MyKBase();
    [Kii beginWithID:APPID andKey:APPKEY andSite:APPSITE];
    [Kii setLogLevel:3];
    LatchedExecuter *l = [[LatchedExecuter alloc]init];
    
    BOOL fin = [l execute:^{
        CKiiUser *cuser = new CKiiUser();
        NSString *uuid = [[[NSUUID alloc]init]UUIDString];
        std::string uname = std::string([uuid cStringUsingEncoding:NSUTF8StringEncoding]);
        std::string upass = std::string("1234");
        cuser->registerNewUser(uname, upass,
                               [&] (picojson::object error) {
                                   NSLog(@"reguser callback");
                                   picojson::value v = error["_error_"];
                                   if (!v.is<picojson::null>())
                                   {
                                       std::string se = v.get<std::string>();
                                       NSLog(@"error: %s", se.c_str());
                                   }
                                   [l offTheLatch];
                               });
    } withTimeOutSec:5];
    XCTAssertTrue(fin == YES, @"register timed out");

}

- (void)tearDown
{
    delete base;
    NSError *error = nil;
    [[KiiUser currentUser]deleteSynchronous:&error];
    XCTAssertNil(error, @"failed to delete user");
    [super tearDown];
}

- (void)testExample
{
    LatchedExecuter *l = [[LatchedExecuter alloc]init];

    XCTAssertNotNil([KiiUser currentUser], @"user should logged in");
    l = [[LatchedExecuter alloc]init];
    CKiiBucket *bkt = CKiiBucket::create();
    base->setCompletionFunc([&](const char *compJson)-> void {
        NSLog(@"cbjson %s", compJson);
        XCTAssertTrue(compJson != nil, @"json should not nil");
        [l offTheLatch];
    });
    
    [l execute:^{
        bkt->createApplicationScopeBucket("myBucket", base, callback_selector(MyKBase::myCallback));
    } withTimeOutSec:5];
    
    XCTAssertNotNil([KiiUser currentUser], @"user should logged in");
    l = [[LatchedExecuter alloc]init];
    base->setCompletionFunc([&](const char *compJson) -> void {
        NSLog(@"cbjson %s", compJson);
        XCTAssertTrue(compJson != nil, @"json should not nil");
        [l offTheLatch];
    });
    [l execute:^{
        picojson::object myObj;
        myObj.insert(make_pair("key", picojson::value("hoge")));
        bkt->object_save(myObj, base, callback_selector(MyKBase::myCallback));
    } withTimeOutSec:5];

}

@end
