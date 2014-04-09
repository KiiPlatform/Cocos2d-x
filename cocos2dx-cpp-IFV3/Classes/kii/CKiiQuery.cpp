/*
 * CKiiQuery.cpp
 *
 *  Created on: 2014/02/17
 *      Author: guest
 */

#include "CKiiQuery.h"

#define MYCCLOG(...)       do {} while (0)
//#define MYCCLOG(format, ...)      cocos2d::log(format, ##__VA_ARGS__)

//コンストラクタ

/**
 * @brief クエリインスタンスの作成 全Objectを取得する
 *
 * @param なし
 */
CKiiQuery::CKiiQuery() {
	// TODO Auto-generated constructor stub
    //CCLOG("CKiiQuery::CKiiQuery");

	picojson::object json1;
	json1.insert( make_pair("type", picojson::value("all") ) );
	_json.insert( make_pair("clause", picojson::value(json1) ) );
}

/**
 * @brief クエリインスタンスの作成 clause付き
 *
 * @param clause KiiClauseインスタンス
 */
CKiiQuery::CKiiQuery(CKiiClause *clause) {
    //CCLOG("CKiiQuery::CKiiQuery　*clause");
	if(clause==NULL){
		_json.insert( make_pair("type", picojson::value("all") ) );
	} else {
		//_json.insert( make_pair("clauses", clause->_json ) );
		_json.insert( make_pair("clause", picojson::value(clause->_json) ) );
	}
}

/**
 * @brief クエリインスタンスの作成 clause付き
 *
 * @param std::shared_ptr<CKiiClause>
 */
CKiiQuery::CKiiQuery(std::shared_ptr<CKiiClause>& clause) {
    //CCLOG("CKiiQuery::CKiiQuery　&clause");

	if(clause==NULL){
		_json.insert( make_pair("type", picojson::value("all") ) );
	} else {
		//_json.insert( make_pair("clauses", clause->_json ) );
		_json.insert( make_pair("clause", picojson::value(clause->_json) ) );
	}

}

CKiiQuery::~CKiiQuery() {
	// TODO Auto-generated destructor stub
    //CCLOG("CKiiQuery::~CKiiQuery");

	//CKiiQuery(NULL);	なぜこれが？？
}

//メソッド

/**
 * @brief 指定フィールドをキーに降順ソート
 *
 * @param key 指定フィールド
 * @return なし
 */
void CKiiQuery::sortByDesc(string key){
	_json.insert( make_pair("orderBy", picojson::value(key) ) );
	_json.insert( make_pair("descending", picojson::value(true) ) );
}

/**
 * @brief 指定フィールドをキーに昇順ソート
 *
 * @param key 指定フィールド
 * @return なし
 */
void CKiiQuery::sortByAsc(string key){
	_json.insert( make_pair("orderBy", picojson::value(key) ) );
	_json.insert( make_pair("descending", picojson::value(false) ) );
}

/**
 * @brief 一度に返す検索結果の件数上限を指定値に設定
 *
 * @param limit 件数上限値
 * @return
 */
void CKiiQuery::setLimit(int limit){
	_mLimit = limit;
}

#if 0
/**
 * @brief
 *
 * @param
 * @return
 */
void CKiiQuery::setNextPaginationKey(string key){

}
#endif


picojson::object CKiiQuery::toJson(){
	picojson::object json;
	json.insert( make_pair("bucketQuery", picojson::value(_json) ) );	//今あるものを入れる
	if(_mLimit > 0){
		json.insert( make_pair("bestEffortLimit", picojson::value((double)_mLimit) ) );
	}

	return json;
}

string CKiiQuery::toString(){
	picojson::object json;
	json = toJson();
    string str = picojson::value(json).serialize();
    //CCLOG("CKiiQuery::toString str %s",str.c_str());

    return str;
}

string CKiiQuery::toString2(){
	picojson::object json;
	json = _json;
    string str = picojson::value(json).serialize();
    //CCLOG("CKiiQuery::toString2 str %s",str.c_str());

    return str;
}

