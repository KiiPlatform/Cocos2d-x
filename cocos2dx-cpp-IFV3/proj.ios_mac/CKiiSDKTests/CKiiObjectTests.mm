//
//  CKiiObjectTests.m
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/13.
//
//

#import <XCTest/XCTest.h>
#import "CKiiSDKTestGlobal.h"
#import "CKiiObject.h"
#import "CKiiError.h"
#import "LatchedExecuter.h"
#import "CKiiUser.h"

using kiicloud::CKiiObject;
using kiicloud::CKiiError;
using kiicloud::CKiiUser;
using std::future;
using std::pair;
using kiicloud::ObjPtr;
using kiicloud::ErrorPtr;

@interface CKiiObjectTests : XCTestCase

@end

@implementation CKiiObjectTests

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

static std::shared_ptr<kiicloud::CKiiUser> currentUser;

- (void) testCreateByUser
{
    LatchedExecuter *l = [[LatchedExecuter alloc]init];
    
    NSUUID *uuid = [[NSUUID alloc]init];
    NSString *ust = [uuid UUIDString];
    std::string *uname = new std::string([ust cStringUsingEncoding:NSUTF8StringEncoding]);
    std::string *password = new std::string("1234");
    
    [l execute:^{
        kiicloud::CKiiUser::registerNewUser(app,
                                            *uname,
                                            *password,
                                            picojson::object(),
                                            [self, l, &uname, &password] (kiicloud::CKiiUser *user, kiicloud::CKiiError *error)
                                            {
                                                currentUser = std::shared_ptr<kiicloud::CKiiUser>(user);
                                                XCTAssertTrue(error == nullptr, @"error should be null");
                                                kiicloud::CKiiUser::login(app,
                                                                          *uname,
                                                                          *password, picojson::object(),
                                                                          [self, l, &uname, &password] (kiicloud::CKiiUser* user, kiicloud::CKiiError *error)
                                                                          {
                                                                              currentUser = std::shared_ptr<kiicloud::CKiiUser>(user);
                                                                              XCTAssertTrue(error == nullptr, @"error should be null");
                                                                              delete uname;
                                                                              delete password;
                                                                              [l offTheLatch];
                                                                          });
                                            });
    } withTimeOutSec:5];

    picojson::object vals;
    vals["key1"] = picojson::value("val1");
    auto ft = CKiiObject::saveNewObject(app, app.appUrl(),
                                        std::string("bk1"),
                                        vals,
                                        currentUser->getAccessToken());
    auto res = ft.get();

    auto created = res.first.get();
    auto erorr = res.second.get();

    XCTAssertTrue(created != nullptr, @"object should be given");
    XCTAssertTrue(created->getCreated() > 0, @"created time should be given");
    XCTAssertTrue(!(created->getId().empty()), @"id should be given");
    XCTAssertTrue(erorr == nullptr, @"error should be null");
}

@end
