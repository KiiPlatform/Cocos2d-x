//
//  CKiiSDKTests.m
//  CKiiSDKTests
//
//  Created by 熊野 聡 on 2014/05/22.
//
//

#import <XCTest/XCTest.h>
#import <vector>
#import "CKiiUser.h"
#import "CKiiError.h"
#import "CKiiApp.h"
#import "CKiiBucket.h"
#import "CKiiQuery.h"
#import "CKiiObject.h"
#import "LatchedExecuter.h"
#import "CKiiSDKTestGlobal.h"

using kiicloud::CKiiUser;
using kiicloud::CKiiError;
using kiicloud::CKiiSite;
using kiicloud::cKiiSiteJP;
using kiicloud::CKiiApp;


@interface CKiiSDKTests : XCTestCase {
    std::shared_ptr<CKiiUser> currUser;
}
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

- (void)testCKiiUser
{
    LatchedExecuter *l = [[LatchedExecuter alloc]init];

    NSString *uname = [[[NSUUID alloc]init]UUIDString];
    std::string username = [uname cStringUsingEncoding:NSUTF8StringEncoding];
    std::string pass("1234");
    

    [l execute:^{
        picojson::object *obj;
        CKiiUser::registerNewUser(app, username, pass, *obj,
                           [& self, l, username] (CKiiUser *authenticatedUser, CKiiError *error) {
                               std::shared_ptr<CKiiUser> uPtr(authenticatedUser);
                               std::shared_ptr<CKiiError> ePtr(error);
                               XCTAssertTrue(authenticatedUser != nullptr, @"user should be passed");

                               picojson::object kvs = authenticatedUser->getKeyValues();
                               std::string _lname = kvs["loginName"].get<std::string>();
                               NSString* _nslname = [NSString stringWithUTF8String:_lname.c_str()];
                               NSString* nsusername = [NSString stringWithUTF8String:username.c_str()];
                               XCTAssertTrue([[nsusername lowercaseString]isEqualToString:[_nslname lowercaseString]], @"username doesn't matches.");
                               XCTAssertTrue(error == nullptr, @"error should be null");
                               [l offTheLatch];
        });
    } withTimeOutSec:5];

    [l execute:^{
        picojson::object *obj;
        CKiiUser::login(app, username, pass, *obj,
                 [& self, l, username] (CKiiUser *user, CKiiError *error) {
                     // Expect failure since no user is registered yet.
                     std::shared_ptr<CKiiError> ePtr(error);
                     currUser = std::shared_ptr<CKiiUser>(user);
                     XCTAssertTrue(user != nullptr, @"user should be passed");

                     picojson::object kvs = user->getKeyValues();
                     std::string _id = kvs["id"].get<std::string>();
                     XCTAssertTrue(_id.length() > 0, @"id should be provided.");
                     XCTAssertTrue(error == nullptr, @"error should be null");
                     [l offTheLatch];
                 });
    } withTimeOutSec:5];

    [l execute:^{
        CKiiUser::refresh(app, *currUser.get(), [&self, l, username] (CKiiUser *user, CKiiError *error) {
            std::shared_ptr<CKiiError> ePtr(error);
            XCTAssert(user != nullptr, @"user must be passed");
            picojson::object kvs = user->getKeyValues();
            std::string _lname = kvs["loginName"].get<std::string>();
            NSString* _nslname = [NSString stringWithUTF8String:_lname.c_str()];
            NSString* nsusername = [NSString stringWithUTF8String:username.c_str()];
            XCTAssertTrue([[nsusername lowercaseString]isEqualToString:[_nslname lowercaseString]], @"username doesn't matches.");
            XCTAssertTrue(error == nullptr);
            [l offTheLatch];
        });
    } withTimeOutSec:5];
}

- (void) testBucketQuery
{
    LatchedExecuter *l = [[LatchedExecuter alloc]init];
    [l execute:^{
        kiicloud::CKiiQuery q;
        std::string accessToken;
        kiicloud::CKiiQueryHandler *qh = kiicloud::CKiiBucket::query(app, app.appUrl(), std::string("myBucket"), q, accessToken);
        qh->nextPage([=, &l] (std::vector<kiicloud::CKiiObject> results, kiicloud::CKiiError *error) {
            XCTAssertEqual(6, results.size(), @"size is different");
            std::vector<kiicloud::CKiiObject>::iterator itr = results.begin();
            while (itr != results.end())
            {
                XCTAssertFalse((*itr).getId().empty());
                XCTAssertFalse((*itr).getVersion().empty());
                XCTAssertFalse((*itr).getOwnerUserId().empty());
                NSLog(@"_created : %lld", (*itr).getCreated());
                NSLog(@"_modified : %lld", (*itr).getModified());
                XCTAssertTrue((*itr).getCreated() > 0);
                XCTAssertTrue((*itr).getModified() > 0);
                XCTAssertTrue(error == nullptr);
                ++itr;
            }
            [l offTheLatch];
        });
    } withTimeOutSec:5];
}

- (void) testBucketQueryWithLimit
{
    LatchedExecuter *l = [[LatchedExecuter alloc]init];
    [l execute:^{
        int *count = new int(0);
        kiicloud::CKiiQuery q(kiicloud::CKiiClause(),1);
        std::string accessToken;
        kiicloud::CKiiQueryHandler *qh = kiicloud::CKiiBucket::query(app, app.appUrl(), std::string("myBucket"), q, accessToken);

        std::function<void (std::vector<kiicloud::CKiiObject> results, kiicloud::CKiiError *error)> *cb1 = new std::function<void (std::vector<kiicloud::CKiiObject>, kiicloud::CKiiError*)>();

        std::function<void (std::vector<kiicloud::CKiiObject> results, kiicloud::CKiiError *error)> cb = [=, &l] (std::vector<kiicloud::CKiiObject> results, kiicloud::CKiiError *error) {
            std::vector<kiicloud::CKiiObject>::iterator itr = results.begin();
            while (itr != results.end())
            {
                ++(*count);
                XCTAssertFalse((*itr).getId().empty());
                XCTAssertFalse((*itr).getVersion().empty());
                XCTAssertFalse((*itr).getOwnerUserId().empty());
                NSLog(@"_created : %lld", (*itr).getCreated());
                NSLog(@"_modified : %lld", (*itr).getModified());
                XCTAssertTrue((*itr).getCreated() > 0);
                XCTAssertTrue((*itr).getModified() > 0);
                XCTAssertTrue(error == nullptr);
                ++itr;
            }
            if (qh->hasNext()) {
                qh->nextPage(*cb1);
            } else {
                XCTAssertTrue(*count == 6, @"count is different : %d", *count);
                delete qh;
                delete cb1;
                delete count;
                [l offTheLatch];
            }
        };
        qh->nextPage(cb);
        cb1->swap(cb);
    } withTimeOutSec:5];
}
@end
