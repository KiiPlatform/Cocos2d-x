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
    base->nullifyCallbackJson();
    delete base;
    [super tearDown];
}

- (void)testExample
{
    CKiiBucket *bkt = CKiiBucket::create();
    bkt->createApplicationScopeBucket("myBucket", base, callback_selector(MyKBase::myCallback));

    const char* cbJson = base->getCallbackJson();
    NSLog(@"cbjson %s", cbJson);
    XCTAssertTrue(cbJson != nil, @"json should not nil");

    base->nullifyCallbackJson();
    picojson::object myObj;
    myObj.insert(make_pair("key", picojson::value("hoge")));
    
    bkt->object_save(myObj, base, callback_selector(MyKBase::myCallback));
    cbJson = base->getCallbackJson();
    NSLog(@"cbjson %s", cbJson);
    XCTAssertTrue(cbJson != nil, @"json should not nil");
}

@end
