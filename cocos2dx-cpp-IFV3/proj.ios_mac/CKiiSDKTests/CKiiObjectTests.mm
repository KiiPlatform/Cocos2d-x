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
using kiicloud::UserAndError;

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
    
    NSUUID *uuid = [[NSUUID alloc]init];
    NSString *ust = [uuid UUIDString];
    std::string *uname = new std::string([ust cStringUsingEncoding:NSUTF8StringEncoding]);
    std::string *password = new std::string("1234");

    std::shared_ptr<std::string> namePtr(uname);
    std::shared_ptr<std::string> passPtr(password);
    auto regFut = kiicloud::CKiiUser::registerNewUser(app,
                                                      *uname,
                                                      *password);
    UserAndError pair = regFut.get();
    CKiiUser* user = pair.first.get();
    CKiiError* error = pair.second.get();
    XCTAssertTrue(user != nullptr, @"user should be given");
    XCTAssertTrue(error == nullptr, @"no error should be given");
    
    auto loginFut = kiicloud::CKiiUser::login(app, *uname, *password);
    pair = loginFut.get();
    user = pair.first.get();
    error = pair.second.get();
    XCTAssertTrue(user != nullptr, @"user should be given");
    XCTAssertTrue(error == nullptr, @"no error should be given");
    currentUser = pair.first;

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

- (void) testCreateByUser_andPatch
{
    NSUUID *uuid = [[NSUUID alloc]init];
    NSString *ust = [uuid UUIDString];
    std::string *uname = new std::string([ust cStringUsingEncoding:NSUTF8StringEncoding]);
    std::string *password = new std::string("1234");
    
    std::shared_ptr<std::string> namePtr(uname);
    std::shared_ptr<std::string> passPtr(password);
    auto regFut = kiicloud::CKiiUser::registerNewUser(app,
                                                      *uname,
                                                      *password);
    UserAndError pair = regFut.get();
    CKiiUser* user = pair.first.get();
    CKiiError* error = pair.second.get();
    XCTAssertTrue(user != nullptr, @"user should be given");
    XCTAssertTrue(error == nullptr, @"no error should be given");
    
    auto loginFut = kiicloud::CKiiUser::login(app, *uname, *password);
    pair = loginFut.get();
    user = pair.first.get();
    error = pair.second.get();
    XCTAssertTrue(user != nullptr, @"user should be given");
    XCTAssertTrue(error == nullptr, @"no error should be given");
    currentUser = pair.first;
    
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
    
    picojson::object vals2;
    vals2["key2"] = picojson::value("val2");
    
    auto res2 = kiicloud::CKiiObject::patchObject(app, *created, vals2, currentUser->getAccessToken(), false);
    auto erorr2 = res2.get();
    
    XCTAssertTrue(erorr2.get() == nullptr, @"error should be null");
    
    XCTAssertTrue(created->getVersion() == "2", @"version is different");
    XCTAssertTrue(created->getCreated() > 0, @"created time should be given");
    XCTAssertTrue(created->getModified() > created->getCreated(), @"modified time should be greater than created");
    picojson::object custom = created->getValues();
    std::string vl1 = custom["key1"].get<std::string>();
    std::string vl2 = custom["key2"].get<std::string>();
    XCTAssertTrue(vl1 == "val1", @"unexpected value for key1 %s", vl1.c_str());
    XCTAssertTrue(vl2 == "val2", @"unexpected value for key2 %s", vl2.c_str());
}

- (void) testCreateByUser_andReplaceObjectValuesWithNewValues
{
    NSUUID *uuid = [[NSUUID alloc]init];
    NSString *ust = [uuid UUIDString];
    std::string *uname = new std::string([ust cStringUsingEncoding:NSUTF8StringEncoding]);
    std::string *password = new std::string("1234");
    
    std::shared_ptr<std::string> namePtr(uname);
    std::shared_ptr<std::string> passPtr(password);
    auto regFut = kiicloud::CKiiUser::registerNewUser(app,
                                                      *uname,
                                                      *password);
    UserAndError pair = regFut.get();
    CKiiUser* user = pair.first.get();
    CKiiError* error = pair.second.get();
    XCTAssertTrue(user != nullptr, @"user should be given");
    XCTAssertTrue(error == nullptr, @"no error should be given");
    
    auto loginFut = kiicloud::CKiiUser::login(app, *uname, *password);
    pair = loginFut.get();
    user = pair.first.get();
    error = pair.second.get();
    XCTAssertTrue(user != nullptr, @"user should be given");
    XCTAssertTrue(error == nullptr, @"no error should be given");
    currentUser = pair.first;
    
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
    
    picojson::object vals2;
    vals2["key2"] = picojson::value("val2");
    
    auto res2 = kiicloud::CKiiObject::replaceObjectValuesWithNewValues(app, *created, vals2, currentUser->getAccessToken());
    auto erorr2 = res2.get();
    
    XCTAssertTrue(erorr2.get() == nullptr, @"error should be null");
    
    XCTAssertTrue(created->getVersion() == "\"2\"", @"version is different");
    XCTAssertTrue(created->getCreated() > 0, @"created time should be given");
    XCTAssertTrue(created->getModified() > created->getCreated(), @"modified time should be greater than created");
    picojson::object custom = created->getValues();
    XCTAssertFalse(picojson::value(custom).contains("key1"), @"key1 should not be contained.");
    XCTAssertFalse(picojson::value(custom).contains("key2"), @"key2 should not be contained.");
}

@end
