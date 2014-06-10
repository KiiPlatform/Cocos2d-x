//
//  CKiiQueryTest.m
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/10.
//
//

#import <XCTest/XCTest.h>
#import "LatchedExecuter.h"
#import <vector>
#import "CKiiClause.h"
#import "CKiiQuery.h"

using kiicloud::CKiiClause;
using kiicloud::CKiiQuery;

@interface CKiiQueryTest : XCTestCase

@end

@implementation CKiiQueryTest

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

- (void)testQueryEquals
{

    CKiiClause c = CKiiClause::equals(std::string("key"), (double)0);
    NSLog(@"clause: %s" ,c.toString().c_str());
 
    CKiiQuery q = CKiiQuery(c);
    std::string sq = q.toString();
    NSLog(@"query: %s" ,sq.c_str());
    
    const char* cq = sq.c_str();
    picojson::value v;
    std::string err;
    picojson::parse(v, cq, cq + strlen(cq), &err);
    XCTAssertTrue(err.empty(), @"parse should not fail : %s", err.c_str());
    
}

@end
