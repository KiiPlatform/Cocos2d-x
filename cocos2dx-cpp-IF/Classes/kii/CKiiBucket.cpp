/*
 * CKiiBucket.cpp
 *
 *  Created on: 2014/02/07
 *      Author: guest
 */

#include "CKiiBucket.h"

extern void kiiReq2( picojson::object set_pairs
		, CCObject* target, SEL_callbackHandler selector ); //kiiリクエストを実行
extern void kiiReq( map<string, string> params, CCObject* target
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
        pCKiiBucket->autorelease();
        pCKiiBucket->retain();	//ないとCKiiBucket::~CKiiBucketが呼ばれる

        return pCKiiBucket;
    }
    CCLOG("CKiiBucket::create error");
    CC_SAFE_DELETE(pCKiiBucket);
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
	//処理なし

	return true;
}

void CKiiBucket::callback(const char *json, CCObject* target, SEL_callbackHandler selector)
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
 * @brief バケットを作成する（アプリケーションスコープ）
 *
 * @param backet_key バケットを識別する文字列
 * @param target 呼び出し元のクラスのポインタ
 * @param selector コールバックメソッドのポインタ
 * @return backet_key バケットを識別する文字列
 */
void CKiiBucket::createApplicationScopeBucket(string backet_key
		, CCObject* target, SEL_callbackHandler selector)
{
	CCLOG("CKiiBucket::createApplicationScopeBucket %s", backet_key.c_str() );

	target_createApplicationScopeBucket = target;
	selector_createApplicationScopeBucket = selector;

    std::map<string, string> params;
    params.insert( make_pair("cmd", "createApplicationScopeBucket" ));
    params.insert( make_pair("backet_key", backet_key ) );	//mydata
    kiiReq( params, this, callback_selector(CKiiBucket::callBack_createApplicationScopeBucket) );
}

void CKiiBucket::callBack_createApplicationScopeBucket(const char *json){
    CCLOG("CKiiBucket::callBack_createApplicationScopeBucket %s",json);

    //json objctとして処理する
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();    //object
    std::string& backet_key = o["backet_key"].get<std::string>();

    _backet_key = backet_key;
    //char *backet_key = backet_key.c_str();
    CCLOG("_backet_key=%s ",_backet_key.c_str() );

    callback(json, target_createApplicationScopeBucket,selector_createApplicationScopeBucket);
}

/**
 * @brief objectを作成する
 *
 * @param set_pairs 保存するkey,valのpair
 * @param target 呼び出し元のクラスのポインタ
 * @param selector コールバックメソッドのポインタ
 * @return なし、別途コールバックより返る
 */
void CKiiBucket::object_save(picojson::object set_pairs
		, CCObject* target, SEL_callbackHandler selector)
{
	CCLOG("CKiiBucket::object_save");
	target_object_save = target;
	selector_object_save = selector;
	set_pairs.insert( make_pair("cmd", picojson::value("object_save") ) );	//object_save
	set_pairs.insert( make_pair("backet_key", picojson::value(_backet_key) ) );	//object_save

    kiiReq2( set_pairs, this, callback_selector(CKiiBucket::callBack_object_save) );

	CCLOG("CKiiBucket::object_save end");
}

void CKiiBucket::callBack_object_save(const char *json){
	CCLOG("CKiiBucket::callBack_object_save");

	//json objctとして処理する
	std::string err;
	picojson::value v;
	picojson::parse(v, json, json + strlen(json), &err);
	picojson::object& o = v.get<picojson::object>();    //object
	std::string& uri = o["uri"].get<std::string>();

	_uri = uri;
	CCLOG("_uri %s ",_uri.c_str() );
	CCLOG("CKiiBucket::callBack_object_save end ");

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
		, CCObject* target, SEL_callbackHandler selector)
{
	CCLOG("CKiiBucket::object_refresh");

	target_object_refresh = target;
	selector_object_refresh = selector;

	std::map<string, string> params;
    params.insert( make_pair("cmd", "object_refresh" ));
    params.insert( make_pair("uri", uri ) );
    kiiReq( params, this, callback_selector(CKiiBucket::callBack_object_refresh) );
}

void CKiiBucket::callBack_object_refresh(const char *json){
	CCLOG("CKiiBucket::callBack_object_refresh");
    callback(json, target_object_refresh,selector_object_refresh);
}



/**
 * @brief Objectの更新（フルアップデート）
 *
 * @param　uri オブジェクトを特定するuri文字列
 * @param set_pairs 保存するkey,valのpair
 * @param target 呼び出し元のクラスのポインタ
 * @param selector コールバックメソッドのポインタ
 * @return なし、別途コールバックより返る
 */
void CKiiBucket::object_update(picojson::object set_pairs
		, CCObject* target, SEL_callbackHandler selector)
{
	CCLOG("CKiiBucket::object_update");
	target_object_save = target;
	selector_object_save = selector;
	set_pairs.insert( make_pair("cmd", picojson::value("object_update") ) );	//object_update
	set_pairs.insert( make_pair("uri", picojson::value(_uri) ) );	//uri アップデートのため

    kiiReq2( set_pairs, this, callback_selector(CKiiBucket::callBack_object_save) );

	CCLOG("CKiiBucket::object_update end");
}

/**
 * @brief Objectの更新（フルアップデート）
 *
 * @param　uri オブジェクトを特定するuri文字列
 * @param set_pairs 保存するkey,valのpair
 * @param target 呼び出し元のクラスのポインタ
 * @param selector コールバックメソッドのポインタ
 * @return なし、別途コールバックより返る
 */
void CKiiBucket::object_saveAllFields(string uri, picojson::object set_map
		, CCObject* target, SEL_callbackHandler selector)
{

}

void CKiiBucket::callBack_object_saveAllFields(const char *json){

}

/**
 * @brief Object の検索
 *
 * @param clause 検索条件
 * @return なし、別途コールバックより返る
 */
void CKiiBucket::query( CKiiClause *clause
		, CCObject* target, SEL_callbackHandler selector){

}

void CKiiBucket::callBack_query(const char *json){

}

