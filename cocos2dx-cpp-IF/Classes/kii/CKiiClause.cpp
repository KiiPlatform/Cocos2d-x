/*
 * CKiiClause.cpp
 *
 *  Created on: 2014/02/10
 *      Author: guest
 */

#include "CKiiClause.h"

CKiiClause::CKiiClause() {
	// TODO Auto-generated constructor stub

}

CKiiClause::~CKiiClause() {
	// TODO Auto-generated destructor stub
}

//--- equals ---
//int
/**
 * @brief equals フィールド値が、指定の値に等しい場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::equals(const string& key, int value){
    CCLOG("CKiiClause::equals int %s %d ",key.c_str(), value);

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("eq") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("value", picojson::value( (double)value) ) );
	clause->_json = json;
	return clause;
}

//double
/**
 * @brief equals フィールド値が、指定の値に等しい場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::equals(const string& key, double value){
    CCLOG("CKiiClause::equals double %s %f", key.c_str(), value);

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("eq") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("value", picojson::value( value ) ) );
	clause->_json = json;
	return clause;
}

#if 0
//bool
CKiiClause* CKiiClause::equals(string key, bool value){
    CCLOG("CKiiClause::equals bool");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("eq") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("value", picojson::value(value) ) );
	clause->_json = json;
	return clause;
}
#endif

//string
/**
 * @brief equals フィールド値が、指定の値に等しい場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::equals(const string& key, const string& value){
    CCLOG("CKiiClause::equals string %s %s",key.c_str(), value.c_str());
	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("eq") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("value", picojson::value(value) ) );
	clause->_json = json;
	return clause;
}
//


//--- notEquals ---
//int
/**
 * @brief notEquals フィールド値が、指定の値に等しくない場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::notEquals(const string& key, int value){
    CCLOG("CKiiClause::equals int");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("not") ) );
	//xx json.insert( make_pair("clause", equals(key,value)->_json ) );
	clause->_json = json;
	return clause;
}

//double
/**
 * @brief notEquals フィールド値が、指定の値に等しくない場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::notEquals(const string& key, double value){
    CCLOG("CKiiClause::equals double");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("not") ) );
	//xx json.insert( make_pair("clause", equals(key,value)->_json ) );
	clause->_json = json;
	return clause;
}

//string
/**
 * @brief notEquals フィールド値が、指定の値に等しくない場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::notEquals(const string& key, const string& value){
    CCLOG("CKiiClause::equals string %s %s",key.c_str(),value.c_str());
	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("not") ) );
	json.insert( make_pair("clause", equals(key,value)->_json ) );

	clause->_json = json;
	return clause;
}
//

//--- greaterThan ---
//int
/**
 * @brief notEquals フィールド値が、指定の値より大きい場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::greaterThan(const string& key, int value){
    CCLOG("CKiiClause::equals int");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("lowerLimit", picojson::value( (double)value) ) );
	json.insert( make_pair("lowerIncluded", picojson::value(false) ) );

	clause->_json = json;
	return clause;
}

//double
/**
 * @brief greaterThan フィールド値が、指定の値より大きい場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::greaterThan(const string& key, double value){
    CCLOG("CKiiClause::equals double");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("lowerLimit", picojson::value( value) ) );
	json.insert( make_pair("lowerIncluded", picojson::value(false) ) );
	clause->_json = json;
	return clause;
}

//string
/**
 * @brief フィールド値が、指定の値より大きい場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::greaterThan(const string& key, const string& value){
    CCLOG("CKiiClause::equals string %s %s",key.c_str(),value.c_str());
	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("lowerLimit", picojson::value( value) ) );
	json.insert( make_pair("lowerIncluded", picojson::value(false) ) );
	clause->_json = json;
	return clause;
}
//

//--- greaterThanOrEqual ---
//int
/**
 * @brief フィールド値が、指定の値以上の場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::greaterThanOrEqual(const string& key, int value){
    CCLOG("CKiiClause::equals int");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("lowerLimit", picojson::value( (double)value) ) );
	json.insert( make_pair("lowerIncluded", picojson::value(true) ) );
	clause->_json = json;
	return clause;
}

//double
/**
 * @brief フィールド値が、指定の値以上の場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::greaterThanOrEqual(const string& key, double value){
    CCLOG("CKiiClause::equals double");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("lowerLimit", picojson::value( value) ) );
	json.insert( make_pair("lowerIncluded", picojson::value(true) ) );
	clause->_json = json;
	return clause;
}

//string
/**
 * @brief フィールド値が、指定の値以上の場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::greaterThanOrEqual(const string& key, const string& value){
    CCLOG("CKiiClause::equals string %s %s",key.c_str(),value.c_str());
	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("lowerLimit", picojson::value( value) ) );
	json.insert( make_pair("lowerIncluded", picojson::value(true) ) );
	clause->_json = json;
	return clause;
}
//

//--- lessThan ---
//int
/**
 * @brief フィールド値が、指定の値より小さい場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::lessThan(const string& key, int value){
    CCLOG("CKiiClause::equals int");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("upperLimit", picojson::value( (double)value) ) );
	json.insert( make_pair("upperIncluded", picojson::value(false) ) );
	clause->_json = json;
	return clause;
}

//double
/**
 * @brief フィールド値が、指定の値より小さい場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::lessThan(const string& key, double value){
    CCLOG("CKiiClause::equals double");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("upperLimit", picojson::value( value) ) );
	json.insert( make_pair("upperIncluded", picojson::value(false) ) );
	clause->_json = json;
	return clause;
}

//string
/**
 * @brief フィールド値が、指定の値より小さい場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::lessThan(const string& key, const string& value){
    CCLOG("CKiiClause::equals string %s %s",key.c_str(),value.c_str());
	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("upperLimit", picojson::value( value) ) );
	json.insert( make_pair("upperIncluded", picojson::value(false) ) );
	clause->_json = json;
	return clause;
}
//

//--- lessThanOrEqual ---
//int
/**
 * @brief フィールド値が、指定の値以下の場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::lessThanOrEqual(const string& key, int value){
    CCLOG("CKiiClause::lessThanOrEqual int");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("upperLimit", picojson::value( (double)value) ) );
	json.insert( make_pair("upperIncluded", picojson::value(true) ) );
	clause->_json = json;
	return clause;
}

//double
/**
 * @brief フィールド値が、指定の値以下の場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::lessThanOrEqual(const string& key, double value){
    CCLOG("CKiiClause::lessThanOrEqual double");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("upperLimit", picojson::value( value) ) );
	json.insert( make_pair("upperIncluded", picojson::value(true) ) );
	clause->_json = json;
	return clause;
}

//string
/**
 * @brief フィールド値が、指定の値以下の場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::lessThanOrEqual(const string& key, const string& value){
    CCLOG("CKiiClause::lessThanOrEqual string %s %s",key.c_str(),value.c_str());
	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("range") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("upperLimit", picojson::value( value) ) );
	json.insert( make_pair("upperIncluded", picojson::value(true) ) );
	clause->_json = json;
	return clause;
}
//

//--- startsWith ---
//string
/**
 * @brief フィールド値 (String 型) が、指定の文字列により始まっている場合にマッチ。
 *
 * @param key target of comparison.
 * @param value to be compared.
 * @return KiiClause instance.
 */
CKiiClause* CKiiClause::startsWith(const string& key, const string& value){
    CCLOG("CKiiClause::equals string %s %s",key.c_str(),value.c_str());
	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("prefix") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("prefix", picojson::value(value) ) );
	clause->_json = json;
	return clause;
}
//


//--- or ---
/**
 * @brief 複数の検索条件を OR で結合。
 *
 * @param int orで渡す引数の数
 * @param orの条件
 * @return given clause.
 */
CKiiClause* CKiiClause::_or(int num, ...){
    CCLOG("CKiiClause::_or");

	CKiiClause *clause = new CKiiClause();
    va_list args;
    va_start(args,num);

    CKiiClause *i;
    i = va_arg(args, CKiiClause* );
    CCLOG("i1=%08x",(unsigned int)i);
	picojson::object json;
	json.insert( make_pair("type", picojson::value("or") ) );
	picojson::object o;
	picojson::array v;
	CCLOG("CKiiClause::_or2");
	int count = 0;
    while(i)
    {
    	count++;
        v.push_back( picojson::value( i->_json ) );
        i = va_arg(args, CKiiClause*);
        CCLOG("i2=%08x",(unsigned int)i);
    }
    va_end(args);
	CCLOG("CKiiClause::equals _or3");
	json.insert( make_pair("clauses", v ) );
    clause->_json = json;

    string str = picojson::value(json).serialize();
    CCLOG("serialize str %s",str.c_str());


    return clause;
}

//--- and ---
/**
 * @brief 複数の検索条件を AND で結合。
 *
 * @param int andで渡す引数の数
 * @param andの条件
 * @return given clause.
 */
CKiiClause* CKiiClause::_and(int num, ...){
    CCLOG("CKiiClause::_and");

	CKiiClause *clause = new CKiiClause();
    va_list args;
    va_start(args,num);

    CKiiClause *i;
    i = va_arg(args, CKiiClause* );
    CCLOG("i1=%08x",(unsigned int)i);
	picojson::object json;
	json.insert( make_pair("type", picojson::value("and") ) );
	picojson::object o;
	picojson::array v;
	CCLOG("CKiiClause::_or2");
    while(i)
    {
        v.push_back( picojson::value( i->_json ) );
        i = va_arg(args, CKiiClause*);
        CCLOG("i2=%08x",(unsigned int)i);
    }
    va_end(args);
	CCLOG("CKiiClause::equals _or3");
	json.insert( make_pair("clauses", v ) );
    clause->_json = json;

    string str = picojson::value(json).serialize();
    CCLOG("serialize str %s",str.c_str());


    return clause;
}

