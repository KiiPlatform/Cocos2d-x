/*
 * CKiiApiTest.cpp
 *
 *  Created on: 2014/02/10
 *      Author: guest
 */

#include "CKiiApiTest.h"

CKiiApiTest::CKiiApiTest() {
	// TODO Auto-generated constructor stub

}

CKiiApiTest::~CKiiApiTest() {
	// TODO Auto-generated destructor stub
}

CKiiApiTest* CKiiApiTest::create()
{
	CCLOG("CKiiApiTest::create");
	CKiiApiTest *pSprite = new CKiiApiTest();
    if (pSprite && pSprite->init())
    {
        CCLOG("CKiiApiTest::create ok");
        pSprite->autorelease();
        pSprite->retain();	//※ 後に追加した部分 ※　ないとKiiSample::~KiiSampleが呼ばれる
        return pSprite;
    }
    CCLOG("CKiiApiTest::create error");
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

bool CKiiApiTest::init()
{
    CCLOG("CKiiApiTest::create error");
    _pCKiiBucket = CKiiBucket::create();
    return true;
}

void CKiiApiTest::createApplicationScopeBucketTest()
{
    CCLOG("CKiiApiTest::createApplicationScopeBucketTest");
    _pCKiiBucket->createApplicationScopeBucket("b_hoge1",
    		this, callback_selector(CKiiApiTest::callBack_createApplicationScopeBucketTest) );
}

void CKiiApiTest::callBack_createApplicationScopeBucketTest(const char *json)
{
    CCLOG("CKiiApiTest::callBack_createApplicationScopeBucketTest");
    CCLOG("json %s ",json );
}

void CKiiApiTest::object_saveTest()
{
    CCLOG("CKiiApiTest::object_saveTest");

    //set
    picojson::object set_pairs;
    set_pairs.insert( make_pair("set_score", picojson::value("987") ) );
    set_pairs.insert( make_pair("set_mode", picojson::value("easy") ) );
    set_pairs.insert( make_pair("set_premiumUser", picojson::value("false") ) );

    _pCKiiBucket->object_save(set_pairs,
    		this, callback_selector(CKiiApiTest::callBack_object_saveTest));
}
void CKiiApiTest::callBack_object_saveTest(const char *json)
{
    CCLOG("CKiiApiTest::callBack_object_saveTest");

    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();    //object
    std::string& uri = o["uri"].get<std::string>();

    _uri = uri;
    CCLOG("_uri %s ",_uri.c_str() );

    //object_refreshTest();	//for test
}

void CKiiApiTest::object_refreshTest()
{
    CCLOG("CKiiApiTest::object_refreshTest");
    _pCKiiBucket->object_refresh(_uri,
    		this, callback_selector(CKiiApiTest::callBack_object_refreshTest));
}

void CKiiApiTest::callBack_object_refreshTest(const char *json)
{
    CCLOG("CKiiApiTest::callBack_object_refreshTest");
    CCLOG("json %s ",json );
}

void CKiiApiTest::object_updateTest()
{
    CCLOG("CKiiApiTest::object_updateTest");
    //set
     picojson::object set_pairs;
     set_pairs.insert( make_pair("set_score", picojson::value("999") ) );
     set_pairs.insert( make_pair("set_mode", picojson::value("easy2") ) );
     set_pairs.insert( make_pair("set_premiumUser", picojson::value("false") ) );

     _pCKiiBucket->object_update("hoge", set_pairs,
     		this, callback_selector(CKiiApiTest::callBack_object_updateTest));
}

void CKiiApiTest::callBack_object_updateTest(const char *json)
{
    CCLOG("CKiiApiTest::callBack_object_updateTest");
    object_refreshTest();	//for test
}

void CKiiApiTest::clause1_Test(){
    CCLOG("CKiiApiTest::clause1_Test");

	CKiiClause* e1 = CKiiClause::equals("key1","val1");
	CKiiClause* e2 = CKiiClause::equals("key2","val2");
	CKiiClause* e3 = CKiiClause::equals("key3","val3");

    CCLOG("e1=%08x",(unsigned int)e1);
    CCLOG("e2=%08x",(unsigned int)e2);
    CCLOG("e3=%08x",(unsigned int)e3);

	CKiiClause* or4 = CKiiClause::_or(e1,e2,e3,NULL);

}

