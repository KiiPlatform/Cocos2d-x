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
#import "LatchedExecuter.h"

using kiicloud::CKiiUser;
using kiicloud::CKiiUserAsyncFactory;
using kiicloud::CKiiError;
using kiicloud::CKiiSite;
using kiicloud::cKiiSiteJP;


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
    [l execute:^{
        picojson::object *obj;
        CKiiUserAsyncFactory *f = new CKiiUserAsyncFactory();
        f->registerNewUser(appId, appKey, appSite, username, "1234", *obj,
                           [& self, l] (std::shared_ptr<CKiiUser> user, std::shared_ptr<CKiiError> error) {
            XCTAssertTrue(user.get() != nullptr, @"user should be passed");
            XCTAssertTrue(error.get() == nullptr, @"error should be null");
            [l offTheLatch];
        });
    } withTimeOutSec:5];

    [l execute:^{
        picojson::object *obj;
        CKiiUserAsyncFactory *f = new CKiiUserAsyncFactory();
        f->login(appId, appKey, appSite, username, "1234", *obj,
                 [& self, l] (std::shared_ptr<CKiiUser> user, std::shared_ptr<CKiiError> error) {
            XCTAssertTrue(user.get() != nullptr, @"user should be passed");
            XCTAssertTrue(error.get() == nullptr, @"error should be null");
            [l offTheLatch];
        });
    } withTimeOutSec:5];

}

@end
