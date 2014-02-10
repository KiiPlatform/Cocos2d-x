/*
 * CKiiBucket.h
 *
 *  Created on: 2014/02/07
 *      Author: guest
 */

#ifndef CKIIBUCKET_H_
#define CKIIBUCKET_H_

#include "cocos2d.h"
#include "picojson.h"
#include "CKiiClause.h"

using namespace std;
using namespace cocos2d;

#define callback_selector(_SELECTOR) (SEL_callbackHandler)(&_SELECTOR)
typedef void (cocos2d::CCObject::*SEL_callbackHandler)(const char *json);


class CKiiBucket : public CCObject
{
public:
	CKiiBucket();
	virtual ~CKiiBucket();

	static CKiiBucket* create();
    bool init();

    string _backet_key;
    string _uri;

    void callback(const char *json, CCObject* target, SEL_callbackHandler selector);

    //
    void createApplicationScopeBucket(string backet_key
    		, CCObject* target, SEL_callbackHandler selector);
    void callBack_createApplicationScopeBucket(const char *json);
    CCObject* target_createApplicationScopeBucket;
    SEL_callbackHandler selector_createApplicationScopeBucket;

    //
    void object_save(picojson::object set_pairs
    		, CCObject* target, SEL_callbackHandler selector);

    void callBack_object_save(const char *json);
    CCObject* target_object_save;
     SEL_callbackHandler selector_object_save;

    //
    void object_refresh(string uri
    		, CCObject* target, SEL_callbackHandler selector);
    void callBack_object_refresh(const char *json);
    CCObject* target_object_refresh;
     SEL_callbackHandler selector_object_refresh;

    //
     void object_update(picojson::object set_pairs
     		, CCObject* target, SEL_callbackHandler selector);


    void callBack_object_update(const char *json);
    CCObject* target_object_update;
     SEL_callbackHandler selector_object_update;


    void object_saveAllFields(string uri, picojson::object set_map
    		, CCObject* target, SEL_callbackHandler selector);
    void callBack_object_saveAllFields(const char *json);


    void query(CKiiClause *clause
    		, CCObject* target, SEL_callbackHandler selector);
    void callBack_query(     const char *json);

};

#endif /* CKIIBUCKET_H_ */
