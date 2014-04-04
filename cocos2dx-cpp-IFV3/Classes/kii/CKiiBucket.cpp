/*
 * CKiiBucket.cpp
 *
 *  Created on: 2014/02/07
 *      Author: guest
 */

#include "CKiiBucket.h"

extern void kiiReq2( picojson::object set_pairs
		, KBase* target, SEL_callbackHandler selector ); //kiiリクエストを実行
extern void kiiReq( map<string, string> params, KBase* target
		, SEL_callbackHandler selector ); //kiiリクエストを実行


CKiiBucket::CKiiBucket() {
	// TODO Auto-generated constructor stub
	CCLOG("CKiiBucket::KiiSample");
}

CKiiBucket::~CKiiBucket() {
	// TODO Auto-generated destructor stub
	CCLOG("CKiiBucket::~CKiiBucket");
}

/**
 * @brief
 *
 * @param
 * @return
 */

/**
 * @brief メモリー確保する
 *
 * @param なし
 * @return オブジェクトのポインター
 */
CKiiBucket* CKiiBucket::create(){
	CCLOG("CKiiBucket::create");

	CKiiBucket *pCKiiBucket = new CKiiBucket();
    if (pCKiiBucket && pCKiiBucket->init())
    {
        CCLOG("CKiiBucket::create ok");
        //pCKiiBucket->autorelease();
        //pCKiiBucket->retain();	//ないとCKiiBucket::~CKiiBucketが呼ばれる
        //_backet_key=NULL;

        return pCKiiBucket;
    }
    CCLOG("CKiiBucket::create error");
    //CC_SAFE_DELETE(pCKiiBucket);
    return NULL;
}

/**
 * @brief 初期化処理
 *
 * @param なし
 * @return 成功or失敗
 */
bool CKiiBucket::init(){
	CCLOG("KiiSample::init");
    _backet_key="";
	//処理なし

	return true;
}

void CKiiBucket::callback(const char *json, KBase* target, SEL_callbackHandler selector)
{
    CCLOG("CKiiBucket::callback1");
    if (target && selector)
    {
        CCLOG("CKiiBucket::callback2");
        (target->*selector)(json);  //コールバックを実行する
    } else {
        CCLOG("CKiiBucket::callback3 error! ");
    }
    CCLOG("CKiiBucket::callback4");
}

/**
 * @brief バケットを作成する（Application Scope）
 *
 * @param backet_key バケットを識別する文字列
 * @param target 呼び出し元のクラスのポインタ
 * @param selector コールバックメソッドのポインタ
 * @return backet_key バケットを識別する文字列
 */
void CKiiBucket::createApplicationScopeBucket(string backet_key
		, KBase* target, SEL_callbackHandler selector)
{
	//CCLOG("CKiiBucket::createApplicationScopeBucket %s", backet_key.c_str() );

	target_createApplicationScopeBucket = target;
	selector_createApplicationScopeBucket = selector;

    std::map<string, string> params;
    params.insert( make_pair("cmd", "createApplicationScopeBucket" ));
    params.insert( make_pair("backet_key", backet_key ) );	//mydata
    kiiReq( params, this, callback_selector(CKiiBucket::callBack_createApplicationScopeBucket) );
}
void CKiiBucket::callBack_createApplicationScopeBucket(const char *json){
    //CCLOG("CKiiBucket::callBack_createApplicationScopeBucket %s",json);

    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();    //object
    std::string& backet_key = o["backet_key"].get<std::string>();

    _backet_key = backet_key;
    //char *backet_key = backet_key.c_str();
    //CCLOG("_backet_key=%s ",_backet_key.c_str() );

    callback(json, target_createApplicationScopeBucket,selector_createApplicationScopeBucket);
}

#if 0
/**
 * @brief バケットを作成する（Group Scope）
 *
 * @param backet_key バケットを識別する文字列
 * @param target 呼び出し元のクラスのポインタ
 * @param selector コールバックメソッドのポインタ
 * @return backet_key バケットを識別する文字列
 */
void CKiiBucket::createGroupScopeBucket(string backet_key
		, KObject* target, SEL_callbackHandler selector)
{
	//CCLOG("CKiiBucket::createApplicationScopeBucket %s", backet_key.c_str() );

	target_createApplicationScopeBucket = target;
	selector_createApplicationScopeBucket = selector;

    std::map<string, string> params;
    params.insert( make_pair("cmd", "createGroupScopeBucket" ));
    params.insert( make_pair("backet_key", backet_key ) );	//mydata
    kiiReq( params, this, callback_selector(CKiiBucket::callBack_createApplicationScopeBucket) );
}
void CKiiBucket::callBack_createGroupScopeBucket(const char *json){
    //CCLOG("CKiiBucket::callBack_createApplicationScopeBucket %s",json);

    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();    //object
    std::string& backet_key = o["backet_key"].get<std::string>();

    _backet_key = backet_key;
    //char *backet_key = backet_key.c_str();
    //CCLOG("_backet_key=%s ",_backet_key.c_str() );

    callback(json, target_createApplicationScopeBucket,selector_createApplicationScopeBucket);
}

/**
 * @brief バケットを作成する（User Scope）
 *
 * @param backet_key バケットを識別する文字列
 * @param target 呼び出し元のクラスのポインタ
 * @param selector コールバックメソッドのポインタ
 * @return backet_key バケットを識別する文字列
 */
void CKiiBucket::createUserScopeBucket(string backet_key
		, KObject* target, SEL_callbackHandler selector)
{
	//CCLOG("CKiiBucket::createApplicationScopeBucket %s", backet_key.c_str() );

	target_createApplicationScopeBucket = target;
	selector_createApplicationScopeBucket = selector;

    std::map<string, string> params;
    params.insert( make_pair("cmd", "createUserScopeBucket" ));
    params.insert( make_pair("backet_key", backet_key ) );	//mydata
    kiiReq( params, this, callback_selector(CKiiBucket::callBack_createApplicationScopeBucket) );
}
void CKiiBucket::callBack_createUserScopeBucket(const char *json){
    //CCLOG("CKiiBucket::callBack_createApplicationScopeBucket %s",json);

    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();    //object
    std::string& backet_key = o["backet_key"].get<std::string>();

    _backet_key = backet_key;
    //char *backet_key = backet_key.c_str();
    //CCLOG("_backet_key=%s ",_backet_key.c_str() );

    callback(json, target_createApplicationScopeBucket,selector_createApplicationScopeBucket);
}
#endif

/**
 * @brief objectを作成する
 *
 * @param key_value_pairs 保存する key-valueのpair
 * @param target 呼び出し元のクラスのポインタ
 * @param selector コールバックメソッドのポインタ
 * @return なし、別途コールバックより返る
 */
void CKiiBucket::object_save(picojson::object key_value_pairs
		, KBase* target, SEL_callbackHandler selector)
{
	//CCLOG("CKiiBucket::object_save");
	target_object_save = target;
	selector_object_save = selector;
	key_value_pairs.insert( make_pair("cmd", picojson::value("object_save") ) );	//object_save
	key_value_pairs.insert( make_pair("backet_key", picojson::value(_backet_key) ) );	//object_save

    kiiReq2( key_value_pairs, this, callback_selector(CKiiBucket::callBack_object_save) );

	//CCLOG("CKiiBucket::object_save end");
}
void CKiiBucket::callBack_object_save(const char *json){
	//CCLOG("CKiiBucket::callBack_object_save");

	//json objctとして処理する
	std::string err;
	picojson::value v;
	picojson::parse(v, json, json + strlen(json), &err);
	picojson::object& o = v.get<picojson::object>();    //object
	std::string& uri = o["uri"].get<std::string>();

	_uri = uri;
	//CCLOG("_uri %s ",_uri.c_str() );
	//CCLOG("CKiiBucket::callBack_object_save end ");

    callback(json, target_object_save,selector_object_save);
}

/**
 * @brief Objectを取得する
 *
 * @param uri オブジェクトを特定するuri文字列
 * @param target 呼び出し元のクラスのポインタ
 * @param selector コールバックメソッドのポインタ
 * @return なし、別途コールバックより返る
 */
void CKiiBucket::object_refresh(string uri
		, KBase* target, SEL_callbackHandler selector)
{
	//CCLOG("CKiiBucket::object_refresh");

	target_object_refresh = target;
	selector_object_refresh = selector;

	std::map<string, string> params;
    params.insert( make_pair("cmd", "object_refresh" ));
    params.insert( make_pair("uri", uri ) );
    kiiReq( params, this, callback_selector(CKiiBucket::callBack_object_refresh) );
}
void CKiiBucket::callBack_object_refresh(const char *json){
	//CCLOG("CKiiBucket::callBack_object_refresh");
    callback(json, target_object_refresh,selector_object_refresh);
}


/**
 * @brief Objectの更新
 *
 * @param　uri オブジェクトを特定するuri文字列
 * @param key_value_pairs 更新するkey-valueのpair
 * @param target 呼び出し元のクラスのポインタ
 * @param selector コールバックメソッドのポインタ
 * @return なし、別途コールバックより返る
 */
void CKiiBucket::object_update(string uri, picojson::object key_value_pairs
		, KBase* target, SEL_callbackHandler selector)
{
	//CCLOG("CKiiBucket::object_update");
	//CCLOG("uri=%s", uri.c_str());

	target_object_save = target;
	selector_object_save = selector;
	key_value_pairs.insert( make_pair("cmd", picojson::value("object_update") ) );	//object_update
	key_value_pairs.insert( make_pair("uri", picojson::value(uri) ) );	//uri アップデートのため

    kiiReq2( key_value_pairs, this, callback_selector(CKiiBucket::callBack_object_save) );

	//CCLOG("CKiiBucket::object_update end");
}

#if 0
/**
 * @brief Object の検索
 *
 * @param query ソート条件等
 * @param clause 検索条件
 * @return なし、別途コールバックより返る
 */
void CKiiBucket::query( picojson::object key_value_pairs
		, KObject* target, SEL_callbackHandler selector){
	CCLOG("CKiiBucket::query -----");
	if(_backet_key==""){
		CCLOG("_backet_key NULL error");
		return;
	}
	target_object_query = target;
	selector_object_query = selector;
	key_value_pairs.insert( make_pair("cmd", picojson::value("bucket_query") ) );	//bucket_query
	key_value_pairs.insert( make_pair("backet_key", picojson::value(_backet_key) ) );	//mydata
	//key_value_pairs.insert( make_pair("query", query->_json ) );

    kiiReq2( key_value_pairs, this, callback_selector(CKiiBucket::callBack_query) );

	CCLOG("CKiiBucket::query end");
}
void CKiiBucket::callBack_query(const char *json){
	CCLOG("CKiiBucket::callBack_query");
    callback(json, target_object_query,selector_object_query);
	CCLOG("CKiiBucket::callBack_query end");
}
#endif


/**
 * @brief Object の検索
 *
 * @param query ソート条件等
 * @param clause 検索条件
 * @return なし、別途コールバックより返る
 */
void CKiiBucket::query( std::shared_ptr<CKiiQuery>& query
		, KBase* target, SEL_callbackHandler selector){
	//CCLOG("CKiiBucket::query -----");
	if(_backet_key==""){
		CCLOG("_backet_key NULL error");
		return;
	}
	target_object_query2 = target;
	selector_object_query2 = selector;

	//json作成
    picojson::object key_value_pairs;
    key_value_pairs.insert( make_pair("query", picojson::value(query->_json) ) );

	key_value_pairs.insert( make_pair("cmd", picojson::value("bucket_query") ) );	//bucket_query
	key_value_pairs.insert( make_pair("backet_key", picojson::value(_backet_key) ) );	//mydata
	//key_value_pairs.insert( make_pair("query", query->_json ) );

    kiiReq2( key_value_pairs, this, callback_selector(CKiiBucket::callBack_query) );

	//CCLOG("CKiiBucket::query end");
}
void CKiiBucket::callBack_query(const char *json){
	//CCLOG("CKiiBucket::callBack_query");
    callback(json, target_object_query2,selector_object_query2);
	//CCLOG("CKiiBucket::callBack_query end");
}


