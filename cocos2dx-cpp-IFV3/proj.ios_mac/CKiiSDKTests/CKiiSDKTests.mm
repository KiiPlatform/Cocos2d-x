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
using kiicloud::UserAndError;


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

    NSString *uname = [[[NSUUID alloc]init]UUIDString];
    std::string username = [uname cStringUsingEncoding:NSUTF8StringEncoding];
    std::string pass("1234");

    // Register
    auto regFut = kiicloud::CKiiUser::registerNewUser(app, username, pass);
    UserAndError pair = regFut.get();
    kiicloud::CKiiUser *authenticatedUser = pair.first.get();
    kiicloud::CKiiError *error =pair.second.get();
    XCTAssertTrue(authenticatedUser != nullptr, @"user should be passed");
    
    picojson::object kvs = authenticatedUser->getKeyValues();
    std::string _lname = kvs["loginName"].get<std::string>();
    NSString* _nslname = [NSString stringWithUTF8String:_lname.c_str()];
    NSString* nsusername = [NSString stringWithUTF8String:username.c_str()];
    XCTAssertTrue([[nsusername lowercaseString]isEqualToString:[_nslname lowercaseString]], @"username doesn't matches.");
    XCTAssertTrue(error == nullptr, @"error should be null");

    // Login
    auto loginFut = kiicloud::CKiiUser::login(app, username, pass);
    pair = loginFut.get();
    authenticatedUser = pair.first.get();
    error =pair.second.get();

    XCTAssertTrue(authenticatedUser != nullptr, @"user should be passed");
    
    kvs = authenticatedUser->getKeyValues();
    std::string _id = kvs["id"].get<std::string>();
    XCTAssertTrue(_id.length() > 0, @"id should be provided.");

    XCTAssertTrue(error == nullptr, @"error should be null");

    // Refresh
    auto refFut = kiicloud::CKiiUser::refresh(app, *authenticatedUser);
    kiicloud::ErrorPtr e = refFut.get();
    authenticatedUser = pair.first.get();
    error = pair.second.get();

    XCTAssert(authenticatedUser != nullptr, @"user must be passed");
    kvs = authenticatedUser->getKeyValues();
    _lname = kvs["loginName"].get<std::string>();
    _nslname = [NSString stringWithUTF8String:_lname.c_str()];
    nsusername = [NSString stringWithUTF8String:username.c_str()];
    XCTAssertTrue([[nsusername lowercaseString]isEqualToString:[_nslname lowercaseString]], @"username doesn't matches.");
    XCTAssertTrue(error == nullptr);

}

- (void) testBucketQuery
{
    kiicloud::CKiiQuery q;
    std::string accessToken;
    kiicloud::CKiiQueryHandler *qh = kiicloud::CKiiBucket::query(app, app.appUrl(), std::string("myBucket"), q, accessToken);
    auto queryFut = qh->nextPage();
    auto resPair = queryFut.get();
    std::vector<kiicloud::CKiiObject> results = resPair.first;
    
    kiicloud::CKiiError* error =resPair.second.get();
    XCTAssertTrue(error == nullptr, @"error should not be given.");
    
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
}

- (void) testBucketQueryWithLimit
{

    int count = 0;
    kiicloud::CKiiQuery q(kiicloud::CKiiClause(),1);
    std::string accessToken;
    kiicloud::CKiiQueryHandler *qh = kiicloud::CKiiBucket::query(app, app.appUrl(), std::string("myBucket"), q, accessToken);
    
    while (qh->hasNext()) {
        auto queryFut = qh->nextPage();
        auto resultsAndError = queryFut.get();

        auto error = resultsAndError.second.get();
        XCTAssertTrue(error == nullptr);
        auto results = resultsAndError.first;
        std::vector<kiicloud::CKiiObject>::iterator itr = results.begin();
        
        while (itr != results.end())
        {
            ++count;
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
    }
    XCTAssertTrue(count == 6, @"count is different : %d", count);

}
@end
