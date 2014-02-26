//
//  KiiSample.cpp
//  MyApp
//
//  Created by Guest on 2014/01/31.
//
//


#include "KiiSample.h"
#include "picojson.h"

/***
#define menu_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)
typedef void (CCObject::*SEL_MenuHandler)(CCObject*);
 ***/

#define callback_selector(_SELECTOR) (SEL_callbackHandler)(&_SELECTOR)
typedef void (KObject::*SEL_callbackHandler)(const char *json);

extern void kiiReq( map<string, string> params, KObject* target, SEL_callbackHandler selector ); //kiiリクエストを実行

/***
KiiSample::KiiSample(void)
: _backet_key(NULL),
  _userDisplayName(NULL),
  _vScoreData(NULL)
{
	CCLOG("KiiSample::KiiSample");
}
***/

KiiSample::KiiSample(void)
{
	CCLOG("KiiSample::KiiSample");
}

KiiSample::~KiiSample(void)
{
	CCLOG("KiiSample::~KiiSample");
    //CC_SAFE_RELEASE(m_pobTexture);
}

KiiSample* KiiSample::create()
{
	CCLOG("KiiSample::create");

	KiiSample *pSprite = new KiiSample();
    if (pSprite && pSprite->init())
    {
        CCLOG("KiiSample::create ok");
        //pSprite->autorelease();
        //pSprite->retain();	//※ 後に追加した部分 ※　ないとKiiSample::~KiiSampleが呼ばれる

        return pSprite;
    }
    CCLOG("KiiSample::create error");
    //CC_SAFE_DELETE(pSprite);
    return NULL;
}

bool KiiSample::init(){
	CCLOG("KiiSample::init");

    std::map<string, string> params;
    
    string param = "b_ranking02";
    params.insert( make_pair("cmd", "createApplicationScopeBucket" ));
    params.insert( make_pair("backet_key", "b_ranking02" ) );
    kiiReq( params, this, callback_selector(KiiSample::callBack_init) );

	CCLOG("KiiSample::init end");
    return true;
}

//typedef std::map<std::string, picojson::value> object;



// Set key-value pairs
/***
object.set("score", 987);
object.set("mode", "easy");
object.set("premiumUser", false);
👆に似せる
***/

//型を柔軟にするテスト

/***
 *
 */
void KiiSample::object_saveTest1(){
	CCLOG("KiiSample::object_saveTest1");

	//vはmapです
	picojson::object v;
    //v["score"] = picojson::value(9000.0);	//コンストラクタでオブジェクトを作る
    //v["name"] = picojson::value("hoge");	//コンストラクタでオブジェクトを作る

    v.insert( make_pair("score", picojson::value(9000.0) ) );
    v.insert( make_pair("name", picojson::value("hoge") ) );

	object_saveTest2(v);
}

void KiiSample::object_saveTest2(picojson::object v){
	CCLOG("KiiSample::object_saveTest2");

	// 要素を出力する
	string key;
	picojson::value val;
	picojson::object::iterator it = v.begin();
	while( it != v.end() )
	{
		key = (*it).first;
		val = (*it).second;
		CCLOG("key %s",key.c_str() );

		++it;
	}
    string str = picojson::value(v).serialize();
    CCLOG("serialize str = %s",str.c_str());

   // v[key] = picojson::value(val);
}

//Object Bucket の作成
// Create Application Scope Bucket
void KiiSample::createApplicationScopeBucket(){
	CCLOG("KiiSample::createApplicationScopeBucket");

    std::map<string, string> params;

    params.insert( make_pair("cmd", "createApplicationScopeBucket" ));
    params.insert( make_pair("backet_key", "mydata" ) );	//mydata
    kiiReq( params, this, callback_selector(KiiSample::callBack_createApplicationScopeBucket) );

	CCLOG("KiiSample::init end");
}
void KiiSample::callBack_createApplicationScopeBucket(const char *json){
    CCLOG("KiiSample::callBack_createApplicationScopeBucket %s",json);

    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();    //object
    std::string& backet_key = o["backet_key"].get<std::string>();

    //char *backet_key = backet_key.c_str();
    CCLOG("backet_key %s ",backet_key.c_str() );
}

//Object の作成
//Key-Value ペアのセット
void KiiSample::object_save(){
	CCLOG("KiiSample::object_save");

    std::map<string, string> params;

    params.insert( make_pair("cmd", "object_save" ));
    params.insert( make_pair("backet_key", "mydata" ) );	//mydata

    //set
    params.insert( make_pair("set_score", "987" ) );
    params.insert( make_pair("set_mode", "easy" ) );
    params.insert( make_pair("set_premiumUser", "false" ) );

    kiiReq( params, this, callback_selector(KiiSample::callBack_object_save) );

	CCLOG("KiiSample::object_save end");
}
void KiiSample::callBack_object_save(const char *json){
    CCLOG("KiiSample::callBack_object_save %s",json);

    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();    //object
    std::string& uri = o["uri"].get<std::string>();

    _uri = uri;
    CCLOG("_uri %s ",_uri.c_str() );

    CCLOG("KiiSample::callBack_object_save end");

    object_refresh();	//for test
}

//Object の取得
void KiiSample::object_refresh(){
	CCLOG("KiiSample::object_refresh");

    std::map<string, string> params;

    params.insert( make_pair("cmd", "object_refresh" ));
    params.insert( make_pair("uri", _uri ) );
    kiiReq( params, this, callback_selector(KiiSample::callBack_object_refresh) );

	CCLOG("KiiSample::object_refresh end");
}
void KiiSample::callBack_object_refresh(const char *json){
    CCLOG("KiiSample::callBack_object_refresh %s",json);

    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o      = v.get<picojson::object>();    //object
    std::string& score       = o["score"].get<std::string>();
    std::string& mode        = o["mode"].get<std::string>();
    std::string& premiumUser = o["premiumUser"].get<std::string>();

    //クラス変数へ代入する
    _score = score;
    _mode = mode;
    _premiumUser = premiumUser;

    CCLOG("_score %s ",_score.c_str() );
    CCLOG("_mode %s ",_mode.c_str() );
    CCLOG("_premiumUser %s ",_premiumUser.c_str() );

    CCLOG("KiiSample::callBack_object_refresh end");
    object_saveAllFields();
}

//Object の更新
//フルアップデート
void KiiSample::object_saveAllFields(){
	CCLOG("KiiSample::object_saveAllFields");

    std::map<string, string> params;

    params.insert( make_pair("cmd", "object_saveAllFields" ));
    params.insert( make_pair("uri", _uri ) );	//uriを渡してrefreshする

    //set
    params.insert( make_pair("set_score", "9000" ) );
    params.insert( make_pair("set_mode", _mode ) );
    params.insert( make_pair("set_premiumUser", _premiumUser ) );

    kiiReq( params, this, callback_selector(KiiSample::callBack_object_saveAllFields) );

	CCLOG("KiiSample::init end");
}
void KiiSample::callBack_object_saveAllFields(const char *json){
    CCLOG("KiiSample::callBack_object_saveAllFields %s",json);

    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();    //object
    std::string& uri = o["uri"].get<std::string>();

    _uri = uri.c_str();
    CCLOG("_uri %s ",_uri.c_str() );

    CCLOG("KiiSample::callBack_object_saveAllFields end");
    object_refresh();	//for test
}



void KiiSample::ranking_query_all(){
	CCLOG("KiiSample::ranking_query_all");

    std::map<std::string, std::string> params;

    params.insert( make_pair("cmd", "bucket_query" ));
    params.insert( make_pair("backet_key", _backet_key ));
    params.insert( make_pair("sortByDesc", "score" ));
    kiiReq( params, this, callback_selector(KiiSample::callBack_ranking_query_all) );
}

void KiiSample::ranking_query(){
	CCLOG("KiiSample::ranking_query");

    std::map<std::string, std::string> params;
    params.insert( make_pair("cmd", "bucket_query" ));
    params.insert( make_pair("backet_key", _backet_key ));
    params.insert( make_pair("sortByDesc", "score" ));
    params.insert( make_pair("KiiClause_equals1","name"));
    params.insert( make_pair("KiiClause_equals2",_userDisplayName));
    kiiReq( params, this, callback_selector(KiiSample::callBack_ranking_query_all) );

}


void KiiSample::displayNameUpdate(const char *name){
	CCLOG("KiiSample::displayNameUpdate %s", name);

    std::map<std::string, std::string> params;
    params.insert( make_pair("cmd", "display_name_update"));
    params.insert( make_pair("display_name", name ));
    kiiReq( params, this, callback_selector(KiiSample::callBack_init) );
}

//callback
void KiiSample::callBack_init(const char *json){
    CCLOG("KiiSample::callBack_init %s",json);
    
    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();    //object
    std::string& backet_key = o["backet_key"].get<std::string>();
    
    //CCLOG("s1 = %s",backet_key.c_str() );
    _backet_key = backet_key.c_str();
    CCLOG("_backet_key %s",_backet_key.c_str() );
}

void KiiSample::callBack_ranking_query_all(const char *json){
    CCLOG("KiiSample::callBack_ranking_query_all %s",json);
    
    //json arrayとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::array& a1 = v.get<picojson::array>(); //array
    for (picojson::array::iterator it = a1.begin(); it != a1.end(); it++) {
        picojson::object& o1 = it->get<picojson::object>();
        std::string& s1 = o1["name"].get<std::string>();
        std::string& s2 = o1["score"].get<std::string>();
        CCLOG("s1 = %s",s1.c_str() );
        CCLOG("s2 = %s",s2.c_str() );
    }
}

void KiiSample::callBack_ranking_query(const char *json){
    CCLOG("KiiSample::callBack_ranking_query %s",json);
    
    //json arrayとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::array& a1 = v.get<picojson::array>(); //array
    for (picojson::array::iterator it = a1.begin(); it != a1.end(); it++) {
        picojson::object& o1 = it->get<picojson::object>();
        std::string& s1 = o1["name"].get<std::string>();
        std::string& s2 = o1["score"].get<std::string>();
        CCLOG("s1 = %s",s1.c_str() );
        CCLOG("s2 = %s",s2.c_str() );
    }
    
}

void KiiSample::callBack_displayNameUpdate(const char *json){
    CCLOG("KiiSample::callBack_displayNameUpdate %s",json);
    
    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();    //object
    std::string& display_name = o["display_name"].get<std::string>();
    
    CCLOG("s1 = %s",display_name.c_str() );
    
    _userDisplayName = display_name.c_str();
}



