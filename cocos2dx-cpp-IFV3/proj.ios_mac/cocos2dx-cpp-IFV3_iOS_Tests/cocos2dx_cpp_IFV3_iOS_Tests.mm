//
//  cocos2dx_cpp_IFV3_iOS_Tests.m
//  cocos2dx-cpp-IFV3_iOS_Tests
//
//  Created by 熊野 聡 on 2014/05/20.
//
//

#import <XCTest/XCTest.h>
#import "CKiiBucket.h"
#import "MyKBase.h"
#import "picojson.h"
#import "LatchedExecuter.h"

@interface cocos2dx_cpp_IFV3_iOS_Tests : XCTestCase
{
@private
    MyKBase *base;
}
@end

@implementation cocos2dx_cpp_IFV3_iOS_Tests

- (void)setUp
{
    [super setUp];
    base = new MyKBase();
}

- (void)tearDown
{
    delete base;
    [super tearDown];
}

- (void)testExample
{
    LatchedExecuter *l = [[LatchedExecuter alloc]init];
    CKiiBucket *bkt = CKiiBucket::create();
    base->setCompletionFunc([&](const char *compJson)-> void {
        NSLog(@"cbjson %s", compJson);
        XCTAssertTrue(compJson != nil, @"json should not nil");
        [l offTheLatch];
    });
    
    [l execute:^{
        bkt->createApplicationScopeBucket("myBucket", base, callback_selector(MyKBase::myCallback));
    } withTimeOutSec:5];


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
        [l offTheLatch];
    } withTimeOutSec:5];

}

@end
