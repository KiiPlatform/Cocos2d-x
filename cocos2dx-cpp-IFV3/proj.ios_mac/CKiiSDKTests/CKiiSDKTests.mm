//
//  CKiiSDKTests.m
//  CKiiSDKTests
//
//  Created by 熊野 聡 on 2014/05/22.
//
//

#import <XCTest/XCTest.h>
#import "CKiiUser.h"
#import "CKiiUserAsyncFactory.h"
#import "CKiiError.h"
#import "CKiiAuth.h"
#import "CKiiApp.h"
#import "LatchedExecuter.h"

using kiicloud::CKiiUser;
using kiicloud::CKiiUserAsyncFactory;
using kiicloud::CKiiError;
using kiicloud::CKiiSite;
using kiicloud::cKiiSiteJP;
using kiicloud::CKiiApp;


@interface CKiiSDKTests : XCTestCase

@end

@implementation CKiiSDKTests

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

const static std::string appId = std::string("551d82a9");
const static std::string appKey = std::string("675bb7fbe71d562c5278ed999e61a800");
const static CKiiSite appSite = cKiiSiteJP;

- (void)testExample
{
    LatchedExecuter *l = [[LatchedExecuter alloc]init];

    NSString *uname = [[[NSUUID alloc]init]UUIDString];
    std::string username = [uname cStringUsingEncoding:NSUTF8StringEncoding];
    std::string pass("1234");
    
    CKiiUserAsyncFactory *f = new CKiiUserAsyncFactory();
    std::shared_ptr<CKiiUserAsyncFactory> p(f);

    [l execute:^{
        picojson::object *obj;
        CKiiApp *app = new CKiiApp(appId, appKey, appSite);
        f->registerNewUser(*app, username, pass, *obj,
                           [& self, l, username, app] (CKiiUser *authenticatedUser, CKiiError *error) {
                               delete app;
                               std::shared_ptr<CKiiUser> uPtr(authenticatedUser);
                               std::shared_ptr<CKiiError> ePtr(error);
                               XCTAssertTrue(authenticatedUser != nullptr, @"user should be passed");

                               picojson::value kvs = authenticatedUser->getKeyValues();
                               std::string _lname = kvs.get<picojson::object>()["loginName"].get<std::string>();
                               NSString* _nslname = [NSString stringWithUTF8String:_lname.c_str()];
                               NSString* nsusername = [NSString stringWithUTF8String:username.c_str()];
                               XCTAssertTrue([[nsusername lowercaseString]isEqualToString:[_nslname lowercaseString]], @"username doesn't matches.");
                               XCTAssertTrue(error == nullptr, @"error should be null");
                               [l offTheLatch];
        });
    } withTimeOutSec:5];

    [l execute:^{
        picojson::object *obj;
        CKiiApp *app = new CKiiApp(appId, appKey, appSite);
        f->login(*app, username, pass, *obj,
                 [& self, l, username, app] (CKiiUser *user, CKiiError *error) {
                     delete app;
                     // Expect failure since no user is registered yet.
                     std::shared_ptr<CKiiUser> uPtr(user);
                     std::shared_ptr<CKiiError> ePtr(error);
                     XCTAssertTrue(user != nullptr, @"user should be passed");

                     picojson::value kvs = user->getKeyValues();
                     picojson::object obj = kvs.get<picojson::object>();
                     picojson::value _idv = obj["id"];
                     std::string _ids = _idv.get<std::string>();
                     XCTAssertTrue(_ids.length() > 0, @"id should be provided.");

                     XCTAssertTrue(error == nullptr, @"error should be null");
                     [l offTheLatch];
                 });
    } withTimeOutSec:5];

}

@end
