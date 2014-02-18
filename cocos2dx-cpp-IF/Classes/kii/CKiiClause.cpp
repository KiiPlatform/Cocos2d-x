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

//int
CKiiClause* CKiiClause::equals(string key, int value){
    CCLOG("CKiiClause::equals int");

	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("eq") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	//json.insert( make_pair("value", picojson::value(value) ) );
	clause->_json = json;
	return clause;
}

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

//string
CKiiClause* CKiiClause::equals(string key, string value){
    CCLOG("CKiiClause::equals string %s %s",key.c_str(),value.c_str());
	CKiiClause *clause = new CKiiClause();
	picojson::object json;
	json.insert( make_pair("type", picojson::value("eq") ) );
	json.insert( make_pair("field", picojson::value(key) ) );
	json.insert( make_pair("value", picojson::value(value) ) );
	clause->_json = json;
	return clause;
}

CKiiClause* CKiiClause::_or(CKiiClause* clauses, ...){
    CCLOG("CKiiClause::_or");

    unsigned int *p;
    p=  (unsigned int *)clauses;
    //CCLOG("p=%08x",*p++);
    //CCLOG("p=%08x",*p++);
    //CCLOG("p=%08x",*p++);
    //CCLOG("p=%08x",*p++);

	CKiiClause *clause = new CKiiClause();
    va_list args;
    va_start(args,clauses);

    CKiiClause *i;
    i = va_arg(args, CKiiClause* );
    CCLOG("i1=%08x",(unsigned int)i);
	picojson::object json;
	json.insert( make_pair("type", picojson::value("or") ) );
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
    clause->_json = json;

    return clause;
}

