/*
 * CKiiApiTest.cpp
 *
 *  Created on: 2014/02/10
 *      Author: guest
 */

#include "CKiiApiTest.h"

//ログ出力
#define DEBUG_CKiiApiTest
#ifndef DEBUG_kiiRanking
#define MYCCLOG(...)       do {} while (0)
#else
#define MYCCLOG(format, ...)      cocos2d::log(format, ##__VA_ARGS__)
#endif
//--

extern char kii_label_buff[1024];
extern char kii_name[256];
extern char kii_display_name[256];

CKiiApiTest::CKiiApiTest() {
	// TODO Auto-generated constructor stub

}

CKiiApiTest::~CKiiApiTest() {
	// TODO Auto-generated  stub
}

CKiiApiTest* CKiiApiTest::create()
{
	MYCCLOG("CKiiApiTest::create");
	CKiiApiTest *pSprite = new CKiiApiTest();
    if (pSprite && pSprite->init())
    {
        //pSprite->autorelease();
        //pSprite->retain();	//※ 後に追加した部分 ※　ないとKiiSample::~KiiSampleが呼ばれる
        pSprite->_name = kii_name;
        pSprite->_display_name = kii_display_name;

        MYCCLOG("_name=%s", pSprite->_name.c_str());
        MYCCLOG("_display_name=%s", pSprite->_display_name.c_str());

        MYCCLOG("CKiiApiTest::create ok");
        return pSprite;
    }
    MYCCLOG("CKiiApiTest::create error");
    //CC_SAFE_DELETE(pSprite);
    return NULL;
}

bool CKiiApiTest::init()
{
    MYCCLOG("CKiiApiTest::init");
    _pCKiiBucket = CKiiBucket::create();
    return true;
}

void CKiiApiTest::createApplicationScopeBucketTest()
{
    MYCCLOG("CKiiApiTest::createApplicationScopeBucketTest");
    //b_ranking03
    _pCKiiBucket->createApplicationScopeBucket("b_ranking03",	//03
    		this, callback_selector(CKiiApiTest::callBack_createApplicationScopeBucketTest) );
}

void CKiiApiTest::callBack_createApplicationScopeBucketTest(const char *json)
{
    MYCCLOG("CKiiApiTest::callBack_createApplicationScopeBucketTest");
    MYCCLOG("json %s ",json );

    clause4_Test();
}

void CKiiApiTest::object_saveTest()
{
    MYCCLOG("CKiiApiTest::object_saveTest -----");

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
    MYCCLOG("CKiiApiTest::callBack_object_saveTest");

    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();    //object
    std::string& uri = o["uri"].get<std::string>();

    _uri = uri;
    MYCCLOG("_uri %s ",_uri.c_str() );

    //object_refreshTest();	//for test
}

void CKiiApiTest::object_refreshTest()
{
    MYCCLOG("CKiiApiTest::object_refreshTest -----");
    _pCKiiBucket->object_refresh(_uri,
    		this, callback_selector(CKiiApiTest::callBack_object_refreshTest));
}

void CKiiApiTest::callBack_object_refreshTest(const char *json)
{
    MYCCLOG("CKiiApiTest::callBack_object_refreshTest");
    MYCCLOG("json %s ",json );
}

void CKiiApiTest::object_updateTest()
{
    MYCCLOG("CKiiApiTest::object_updateTest -----");
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
    MYCCLOG("CKiiApiTest::callBack_object_updateTest");
    object_refreshTest();	//for test
}

void CKiiApiTest::clause4_Test(){
    MYCCLOG("CKiiApiTest::clause4_Test");
    //auto e1 = CKiiClause::equals("name","43f76824-e92b-4a8f-a34d-8fac92248250");
	//auto q = std::make_shared<CKiiQuery>(e1);	//new
	auto q = std::make_shared<CKiiQuery>();	//new
	q->sortByDesc("score");
	string s = q->toString2();
    MYCCLOG("s = %s", s.c_str() );

    //set
    picojson::object set_pairs;
    set_pairs.insert( make_pair("query", picojson::value(q->_json) ) );	//q or q2

    _pCKiiBucket->query(q,
    		this, callback_selector(CKiiApiTest::callBack_clause4_Test));

    MYCCLOG("CKiiApiTest::clause4_Test end ---");
}
void CKiiApiTest::callBack_clause4_Test(const char *json){
    char buff[256];
    //char kii_label_buff[1024];
    std::string err;

    MYCCLOG("CKiiApiTest::callBack_clause4_Test");
    MYCCLOG("json %s ",json );
    //
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::array& a1 = v.get<picojson::array>();

    int index = 0;
    strcpy(kii_label_buff, "");
    for (picojson::array::iterator it = a1.begin(); it != a1.end(); it++) {
        picojson::object& o1 = it->get<picojson::object>();
        std::string& s1 = o1["display_name"].get<std::string>();
        std::string& s2 = o1["score"].get<std::string>();
        //MYCCLOG("s1 = %s",s1.c_str() );
        //MYCCLOG("s2 = %s",s2.c_str() );
        sprintf(buff,"%d %s : %s \n", index+1, s1.c_str(), s2.c_str() );
        MYCCLOG("buff = %s",buff);
        if(index<7){
        	strcat(kii_label_buff, buff);
        }
        index++;
        //if(index>=7){
        //    break;
        //}
    }
    MYCCLOG("---");
    MYCCLOG("kii_label_buff %s",kii_label_buff);
    //
}

void CKiiApiTest::clause3_Test(){
    MYCCLOG("CKiiApiTest::clause3_Test");
#if 1
    /***
	CKiiClause* e1 = CKiiClause::equals("name","43f76824-e92b-4a8f-a34d-8fac92248250");	//43f7
	CKiiQuery* q = new CKiiQuery(e1);
	CKiiQuery* q2 = new CKiiQuery();
	***/

    //auto e1 = std::make_shared<CKiiClause>( CKiiClause::equals("name","43f76824-e92b-4a8f-a34d-8fac92248250") );

    auto e1 = CKiiClause::equals("name","43f76824-e92b-4a8f-a34d-8fac92248250");
    //auto e2 = CKiiClause::equals2("name",1234);

    /***
    string key ="hoge_val";
    double value = 1.234f;
	picojson::object json;
	json.insert( make_pair("type", picojson::value("eq") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("value", picojson::value( (double)value) ) );
	e1->_json = json;
	***/
    //xxCKiiQuery* q = new CKiiQuery(e1);
	//CKiiQuery* q2 = new CKiiQuery();
	auto q = std::make_shared<CKiiQuery>(e1);	//new
	//auto q = std::make_shared<CKiiQuery>(e2);

	q->sortByDesc("score");
	string s = q->toString2();

    MYCCLOG("s = %s", s.c_str() );

    //set
    //picojson::object set_pairs;
    //set_pairs.insert( make_pair("query", picojson::value(q->_json) ) );	//q or q2

    _pCKiiBucket->query(q,
    		this, callback_selector(CKiiApiTest::callBack_clause3_Test));

    MYCCLOG("CKiiApiTest::clause3_Test end ---");
#endif
}
void CKiiApiTest::callBack_clause3_Test(const char *json){
    MYCCLOG("CKiiApiTest::callBack_clause3_Test");
    MYCCLOG("json %s ",json );
}

void CKiiApiTest::clause2_Test(){
    MYCCLOG("CKiiApiTest::clause2_Test");

    auto e1 = CKiiClause::lessThanOrEqual("key1",1);
    auto e2 = CKiiClause::lessThanOrEqual("key2",2.222);
    auto e3 = CKiiClause::lessThanOrEqual("key3",3);
    auto e4 = CKiiClause::lessThanOrEqual("key4","hoge");

    auto or4 = CKiiClause::_and(3,e1.get(),e2.get(),e3.get(),e4.get(),NULL);

	//CKiiQuery* q = new CKiiQuery(or4);
	//string s = q->toString();

    //MYCCLOG("s = %s", s.c_str() );

}

//バケットを作成
void CKiiApiTest::postMyScore(int hiscore){
    MYCCLOG("KiiRanking::postMyScore ---");
    MYCCLOG("hiscore = %d", hiscore );
    //_hiscore = hiscore;	//クラス変数に保存する
    //_hiscore = 5678;
    //MYCCLOG("_hiscore = %d", _hiscore );
    //バケットを作成
    //b_ranking03
    _pCKiiBucket->createApplicationScopeBucket("b_ranking03",	//03
                                               this, callback_selector(CKiiApiTest::callBack_postMyScore) );
    MYCCLOG("KiiRanking::postMyScore end ---");
}
void CKiiApiTest::callBack_postMyScore(const char *json){
    MYCCLOG("CKiiApiTest::callBack_queryMyScore");
    MYCCLOG("json %s ",json );
    clause1_Test();
}

void CKiiApiTest::clause1_Test(){
    MYCCLOG("CKiiApiTest::clause1_Test");

/***
    int val1 = 1;
	//CKiiClause* e1 = CKiiClause::equals("key1",val1);
    auto e1 = CKiiClause::equals("key1",1234);

	double dval = 2.222;
	auto e2 = CKiiClause::equals("key2",2.222);
	auto e3 = CKiiClause::equals("key3",5678);
	auto e4 = CKiiClause::equals("key4","hoge");
***/
    int val1 = 1;
	//CKiiClause* e1 = CKiiClause::equals("key1",val1);
    auto e1 = CKiiClause::equals("key1",1234);
    //auto e1 = CKiiClause::lessThanOrEqual("score",1234);
    
	double dval = 2.222;
	auto e2 = CKiiClause::lessThanOrEqual("score",2222);
	auto e3 = CKiiClause::lessThanOrEqual("score",5678);
	auto e4 = CKiiClause::lessThanOrEqual("score",1111);
    
	/***
    MYCCLOG("e1=%08x",(unsigned int)e1);
    MYCCLOG("e2=%08x",(unsigned int)e2);
    MYCCLOG("e3=%08x",(unsigned int)e3);
    MYCCLOG("e4=%08x",(unsigned int)e4);
    ***/

	auto or4 = CKiiClause::_and(3,e1.get(),e2.get(),e3.get(),e4.get(),NULL);
	auto q = std::make_shared<CKiiQuery>(or4);	//new
	string s = q->toString2();
	//CKiiQuery* q = CKiiQuery::CKiiQuery(or4);
	//string s = q->toString();

    _pCKiiBucket->query(q,
                        this, callback_selector(CKiiApiTest::callBack_clause1_Test));
    
    MYCCLOG("CKiiApiTest::clause1_Test end ---");
}
void CKiiApiTest::callBack_clause1_Test(const char *json){
    //char buff[256];
    //std::string err;
    
    MYCCLOG("KiiRanking::callBack_queryMyScore");
    MYCCLOG("json %s ",json );
}

