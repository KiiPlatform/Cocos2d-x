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
    
    // TODO: do more testing. right now, Human see the log.
}

- (void) testQueryGT
{
    CKiiClause c = CKiiClause::greaterThan(std::string("key"), (double)0);
    NSLog(@"clause: %s" ,c.toString().c_str());

    CKiiQuery q = CKiiQuery(c);
    std::string sq = q.toString();
    NSLog(@"query: %s" ,sq.c_str());

    // TODO: do more testing. right now, Human see the log.
}

- (void) testQueryGTOE
{
    CKiiClause c = CKiiClause::greaterThanOrEqual(std::string("key"), (double)0);
    NSLog(@"clause: %s" ,c.toString().c_str());
    
    CKiiQuery q = CKiiQuery(c);
    std::string sq = q.toString();
    NSLog(@"query: %s" ,sq.c_str());

    // TODO: do more testing. right now, Human see the log.
}

- (void) testQueryLT
{
    CKiiClause c = CKiiClause::lessThan(std::string("key"), (double)0);
    NSLog(@"clause: %s" ,c.toString().c_str());
    
    CKiiQuery q = CKiiQuery(c);
    std::string sq = q.toString();
    NSLog(@"query: %s" ,sq.c_str());
    
    // TODO: do more testing. right now, Human see the log.
}

- (void) testQueryLTOE
{
    CKiiClause c = CKiiClause::lessThanOrEqual(std::string("key"), (double)0);
    NSLog(@"clause: %s" ,c.toString().c_str());

    CKiiQuery q = CKiiQuery(c);
    std::string sq = q.toString();
    NSLog(@"query: %s" ,sq.c_str());
    
    // TODO: do more testing. right now, Human see the log.
}

- (void) testQueryIn
{
    std::vector<double> vals;
    vals.push_back(1);
    vals.push_back(2);
    CKiiClause c = CKiiClause::inClause(std::string("key"), vals);
    NSLog(@"clause: %s" ,c.toString().c_str());
    
    CKiiQuery q = CKiiQuery(c);
    std::string sq = q.toString();
    NSLog(@"query: %s" ,sq.c_str());
    
    // TODO: do more testing. right now, Human see the log.
}

- (void) testQueryStartsWith
{
    CKiiClause c = CKiiClause::startsWith("key", "prefix");
    NSLog(@"clause: %s" ,c.toString().c_str());

    CKiiQuery q = CKiiQuery(c);
    std::string sq = q.toString();
    NSLog(@"query: %s" ,sq.c_str());

    // TODO: do more testing. right now, Human see the log.
}


@end
