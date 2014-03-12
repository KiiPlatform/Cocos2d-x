/*
 * CKiiApiTest.cpp
 *
 *  Created on: 2014/02/10
 *      Author: guest
 */

#include "CKiiApiTest.h"
#include "KObject.h"

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
	CCLOG("CKiiApiTest::create");
	CKiiApiTest *pSprite = new CKiiApiTest();
    if (pSprite && pSprite->init())
    {
        //pSprite->autorelease();
        //pSprite->retain();	//※ 後に追加した部分 ※　ないとKiiSample::~KiiSampleが呼ばれる
        pSprite->_name = kii_name;
        pSprite->_display_name = kii_display_name;

        CCLOG("_name=%s", pSprite->_name.c_str());
        CCLOG("_display_name=%s", pSprite->_display_name.c_str());

        CCLOG("CKiiApiTest::create ok");
        return pSprite;
    }
    CCLOG("CKiiApiTest::create error");
    //CC_SAFE_DELETE(pSprite);
    return NULL;
}

bool CKiiApiTest::init()
{
    CCLOG("CKiiApiTest::init");
    _pCKiiBucket = CKiiBucket::create();
    return true;
}

void CKiiApiTest::createApplicationScopeBucketTest()
{
    CCLOG("CKiiApiTest::createApplicationScopeBucketTest");
    //b_ranking03
    _pCKiiBucket->createApplicationScopeBucket("b_ranking03",	//03
    		this, callback_selector(CKiiApiTest::callBack_createApplicationScopeBucketTest) );
}

void CKiiApiTest::callBack_createApplicationScopeBucketTest(const char *json)
{
    CCLOG("CKiiApiTest::callBack_createApplicationScopeBucketTest");
    CCLOG("json %s ",json );

    clause4_Test();
}

void CKiiApiTest::object_saveTest()
{
    CCLOG("CKiiApiTest::object_saveTest -----");

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
    CCLOG("CKiiApiTest::object_refreshTest -----");
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
    CCLOG("CKiiApiTest::object_updateTest -----");
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

//バケットを作成
void CKiiApiTest::queryMyScore(int hiscore){
    CCLOG("CKiiApiTest::queryMyScore -----");
    CCLOG("hiscore = %d", hiscore );
    _hiscore = hiscore;	//クラス変数に保存する
    //_hiscore = 5678;
    CCLOG("_hiscore = %d", _hiscore );
    //バケットを作成
    //b_ranking02
    _pCKiiBucket->createApplicationScopeBucket("b_ranking03",	//03
    		this, callback_selector(CKiiApiTest::callBack_queryMyScore) );
    CCLOG("CKiiApiTest::queryMyScore end ---");
}
void CKiiApiTest::callBack_queryMyScore(const char *json){
    CCLOG("CKiiApiTest::callBack_queryMyScore");
    CCLOG("json %s ",json );
    queryMyScore2();
}

//自分のスコアを検索
void CKiiApiTest::queryMyScore2(){
    CCLOG("CKiiApiTest::queryMyScore2 -----");

    //自分のスコアを検索
    //auto e1 = CKiiClause::equals("name","43f76824-e92b-4a8f-a34d-8fac92248250");
    auto e1 = CKiiClause::equals("name",_name);
    auto q = std::make_shared<CKiiQuery>(e1);	//new
	string s = q->toString2();
	q->sortByDesc("score");
    CCLOG("s = %s", s.c_str() );

    //set
    //picojson::object set_pairs;
    //set_pairs.insert( make_pair("query", picojson::value(q->_json) ) );	//q or q2
    _pCKiiBucket->query(q,
    		this, callback_selector(CKiiApiTest::callBack_queryMyScore2));

    CCLOG("CKiiApiTest::queryMyScore end ---");
}
void CKiiApiTest::callBack_queryMyScore2(const char *json){
    char buff[256];
    std::string err;

    CCLOG("CKiiApiTest::callBack_queryMyScore2");
    CCLOG("json %s ",json );

    //
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::array& a1 = v.get<picojson::array>();

    int index = 0;
    std::string _uri_, name, display_name, score;
    for (picojson::array::iterator it = a1.begin(); it != a1.end(); it++) {
        picojson::object& o1 = it->get<picojson::object>();
        name = o1["name"].get<std::string>();
        display_name = o1["display_name"].get<std::string>();
        score = o1["score"].get<std::string>();
        _uri_ = o1["_uri_"].get<std::string>();
        sprintf(buff,"%d %s %s %s %s\n",
        		index+1, name.c_str(), display_name.c_str(), score.c_str(), _uri_.c_str() );
        CCLOG("buff=%s",buff);
        index++;
        break;	//１回でループを抜ける
    }
    CCLOG("index = %d",index);
    //
    //CCLOG("_uri_=%s",_uri_.c_str() );
    if(_uri_!=""){
    	CCLOG("score ari");
    	_name = name;
    	_display_name = display_name;
    	_score = score;
    	_uri = _uri_;
    	CCLOG("_uri=%s", _uri.c_str());
    	updateMyScore(_uri, _hiscore);	//保存していた変数を使ってスコアを更新するupdate
    } else {
    	CCLOG("score nasi");
    	saveMyScore(_hiscore);	//新規作成なのでsave
    }
    CCLOG("CKiiApiTest::callBack_queryMyScore2 end ---");
}

//スコアの更新
void CKiiApiTest::updateMyScore(string uri, int hiscore){
    CCLOG("CKiiApiTest::updateMyScore -----");
    CCLOG("hiscore = %d", hiscore );
	CCLOG("uri=%s", uri.c_str());
    //set
     picojson::object set_pairs;
     set_pairs.insert( make_pair("set_score", picojson::value((double)hiscore) ) );
     set_pairs.insert( make_pair("set_name", picojson::value(_name) ) );
     set_pairs.insert( make_pair("set_display_name", picojson::value(_display_name) ) );

     string uri2 = uri;
     _pCKiiBucket->object_update(uri2, set_pairs,
     		this, callback_selector(CKiiApiTest::callBack_updateMyScore));
}
void CKiiApiTest::callBack_updateMyScore(const char *json){
    CCLOG("CKiiApiTest::callBack_updateMyScore");
    CCLOG("json %s ",json );

    object_refreshTest();
}

//スコアの新規作成
void CKiiApiTest::saveMyScore(int hiscore){
    CCLOG("CKiiApiTest::saveMyScore -----");

    //set
    picojson::object set_pairs;
    set_pairs.insert( make_pair("set_score", picojson::value((double)hiscore) ) );
    set_pairs.insert( make_pair("set_name", picojson::value(_name) ) );
    set_pairs.insert( make_pair("set_display_name", picojson::value(_display_name) ) );

    _pCKiiBucket->object_save(set_pairs,
    		this, callback_selector(CKiiApiTest::callBack_saveMyScore));
}
void CKiiApiTest::callBack_saveMyScore(const char *json){
    std::string err;

    CCLOG("CKiiApiTest::callBack_saveMyScore");
    CCLOG("json %s ",json );
    //_uriの保存
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();
    string uri = o["uri"].get<std::string>();
    if(uri!=""){
    	_uri = uri;
    	CCLOG("_uri=%s ",_uri.c_str() );
    } else {
    	CCLOG("uri error");
    }
    object_refreshTest();
}

void CKiiApiTest::clause4_Test(){
    CCLOG("CKiiApiTest::clause4_Test");
    //auto e1 = CKiiClause::equals("name","43f76824-e92b-4a8f-a34d-8fac92248250");
	//auto q = std::make_shared<CKiiQuery>(e1);	//new
	auto q = std::make_shared<CKiiQuery>();	//new
	q->sortByDesc("score");
	string s = q->toString2();
    CCLOG("s = %s", s.c_str() );

    //set
    picojson::object set_pairs;
    set_pairs.insert( make_pair("query", picojson::value(q->_json) ) );	//q or q2

    _pCKiiBucket->query(q,
    		this, callback_selector(CKiiApiTest::callBack_clause4_Test));

    CCLOG("CKiiApiTest::clause4_Test end ---");
}
void CKiiApiTest::callBack_clause4_Test(const char *json){
    char buff[256];
    //char kii_label_buff[1024];
    std::string err;

    CCLOG("CKiiApiTest::callBack_clause4_Test");
    CCLOG("json %s ",json );
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
        //CCLOG("s1 = %s",s1.c_str() );
        //CCLOG("s2 = %s",s2.c_str() );
        sprintf(buff,"%d %s : %s \n", index+1, s1.c_str(), s2.c_str() );
        CCLOG("buff = %s",buff);
        if(index<7){
        	strcat(kii_label_buff, buff);
        }
        index++;
        //if(index>=7){
        //    break;
        //}
    }
    CCLOG("---");
    CCLOG("kii_label_buff %s",kii_label_buff);
    //
}

void CKiiApiTest::clause3_Test(){
    CCLOG("CKiiApiTest::clause3_Test");
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

    CCLOG("s = %s", s.c_str() );

    //set
    //picojson::object set_pairs;
    //set_pairs.insert( make_pair("query", picojson::value(q->_json) ) );	//q or q2

    _pCKiiBucket->query(q,
    		this, callback_selector(CKiiApiTest::callBack_clause3_Test));

    CCLOG("CKiiApiTest::clause3_Test end ---");
#endif
}
void CKiiApiTest::callBack_clause3_Test(const char *json){
    CCLOG("CKiiApiTest::callBack_clause3_Test");
    CCLOG("json %s ",json );
}

void CKiiApiTest::clause2_Test(){
    CCLOG("CKiiApiTest::clause2_Test");

    auto e1 = CKiiClause::lessThanOrEqual("key1",1);
    auto e2 = CKiiClause::lessThanOrEqual("key2",2.222);
    auto e3 = CKiiClause::lessThanOrEqual("key3",3);
    auto e4 = CKiiClause::lessThanOrEqual("key4","hoge");

    auto or4 = CKiiClause::_and(3,e1.get(),e2.get(),e3.get(),e4.get(),NULL);

	//CKiiQuery* q = new CKiiQuery(or4);
	//string s = q->toString();

    //CCLOG("s = %s", s.c_str() );

}

void CKiiApiTest::clause1_Test(){
    CCLOG("CKiiApiTest::clause1_Test");

    int val1 = 1;
	//CKiiClause* e1 = CKiiClause::equals("key1",val1);
    auto e1 = CKiiClause::equals("key1",1234);

	double dval = 2.222;
	auto e2 = CKiiClause::equals("key2",2.222);
	auto e3 = CKiiClause::equals("key3",5678);
	auto e4 = CKiiClause::equals("key4","hoge");

	/***
    CCLOG("e1=%08x",(unsigned int)e1);
    CCLOG("e2=%08x",(unsigned int)e2);
    CCLOG("e3=%08x",(unsigned int)e3);
    CCLOG("e4=%08x",(unsigned int)e4);
    ***/

	auto or4 = CKiiClause::_or(3,e1.get(),e2.get(),e3.get(),e4.get(),NULL);
	auto q = std::make_shared<CKiiQuery>(or4);	//new
	string s = q->toString2();
	//CKiiQuery* q = CKiiQuery::CKiiQuery(or4);
	//string s = q->toString();

    CCLOG("CKiiApiTest::clause1_Test end ---");
}

