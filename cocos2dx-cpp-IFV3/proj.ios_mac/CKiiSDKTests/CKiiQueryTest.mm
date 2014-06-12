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

- (void) testOr
{
    kiicloud::CKiiClause c1 = kiicloud::CKiiClause::startsWith("key", "pr1");
    std::vector<std::string> vals;
    vals.push_back("v1");
    vals.push_back("v2");
    kiicloud::CKiiClause c2 = kiicloud::CKiiClause::inClause("key", vals);
    
    std::vector<kiicloud::CKiiClause> cls;
    cls.push_back(c1);
    cls.push_back(c2);
    kiicloud::CKiiClause c3 = kiicloud::CKiiClause::orClause(cls);
    
    kiicloud::CKiiQuery q = kiicloud::CKiiQuery(c3);
    NSLog(@"query :%s", q.toString().c_str());
    
    // TODO: do more testing. right now, Human see the log.
}

- (void) testAnd
{
    kiicloud::CKiiClause c1 = kiicloud::CKiiClause::startsWith("key", "pr1");
    std::vector<std::string> vals;
    vals.push_back("v1");
    vals.push_back("v2");
    kiicloud::CKiiClause c2 = kiicloud::CKiiClause::inClause("key", vals);
    
    std::vector<kiicloud::CKiiClause> cls;
    cls.push_back(c1);
    cls.push_back(c2);
    kiicloud::CKiiClause c3 = kiicloud::CKiiClause::andClause(cls);
    
    kiicloud::CKiiQuery q = kiicloud::CKiiQuery(c3);
    NSLog(@"query :%s", q.toString().c_str());
    
    // TODO: do more testing. right now, Human see the log.
}

- (void) testNotOperator
{
    kiicloud::CKiiClause c1 = kiicloud::CKiiClause::startsWith("key", "pr1");
    kiicloud::CKiiClause c2 = not c1;
    
    kiicloud::CKiiQuery q1 = kiicloud::CKiiQuery(c1);
    kiicloud::CKiiQuery q2 = kiicloud::CKiiQuery(c2);
    
    NSLog(@"query1 :%s", q1.toString().c_str());
    NSLog(@"query2 :%s", q2.toString().c_str());
}

- (void) testOrOperator
{
    kiicloud::CKiiClause c1 = kiicloud::CKiiClause::startsWith("key", "pr1");
    std::vector<std::string> vals;
    vals.push_back("v1");
    vals.push_back("v2");
    kiicloud::CKiiClause c2 = kiicloud::CKiiClause::inClause("key", vals);
    
    std::vector<kiicloud::CKiiClause> cls;
    cls.push_back(c1);
    cls.push_back(c2);
    kiicloud::CKiiClause c3 = c1 or c2;
    
    kiicloud::CKiiQuery q = kiicloud::CKiiQuery(c3);
    NSLog(@"query :%s", q.toString().c_str());
    
    // TODO: do more testing. right now, Human see the log.
}

- (void) testAndOperator
{
    kiicloud::CKiiClause c1 = kiicloud::CKiiClause::startsWith("key", "pr1");
    std::vector<std::string> vals;
    vals.push_back("v1");
    vals.push_back("v2");
    kiicloud::CKiiClause c2 = kiicloud::CKiiClause::inClause("key", vals);
    
    std::vector<kiicloud::CKiiClause> cls;
    cls.push_back(c1);
    cls.push_back(c2);
    kiicloud::CKiiClause c3 = c1 and c2;
    
    kiicloud::CKiiQuery q = kiicloud::CKiiQuery(c3);
    NSLog(@"query :%s", q.toString().c_str());
    
    // TODO: do more testing. right now, Human see the log.
}
@end
