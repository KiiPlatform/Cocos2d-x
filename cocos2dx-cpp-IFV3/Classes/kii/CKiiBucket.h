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
#include "CKiiQuery.h"
#include "KBase.h"


using namespace std;
using namespace cocos2d;

#define callback_selector(_SELECTOR) (SEL_callbackHandler)(&_SELECTOR)
typedef void (KBase::*SEL_callbackHandler)(const char *json);

//Kii

class CKiiBucket : public KBase
{
public:
	CKiiBucket();
	virtual ~CKiiBucket();
    
	static CKiiBucket* create();
    bool init();
    
    string _backet_key;
    string _uri;
    
    void callback(const char *json, KBase* target, SEL_callbackHandler selector);
    
    //
    void createApplicationScopeBucket(string backet_key
                                      , KBase* target, SEL_callbackHandler selector);
    void callBack_createApplicationScopeBucket(const char *json);
    KBase* target_createApplicationScopeBucket;
    SEL_callbackHandler selector_createApplicationScopeBucket;
    
#if 0
    //
    void createGroupScopeBucket(string backet_key
                                , KObject* target, SEL_callbackHandler selector);
    void callBack_createGroupScopeBucket(const char *json);
    KObject* target_createGroupScopeBucket;
    SEL_callbackHandler selector_createGroupScopeBucket;
    
    //
    void createUserScopeBucket(string backet_key
                               , KObject* target, SEL_callbackHandler selector);
    void callBack_createUserScopeBucket(const char *json);
    KObject* target_createUserScopeBucket;
    SEL_callbackHandler selector_createUserScopeBucket;
#endif
    
    //
    void object_save(picojson::object key_value_pairs
                     , KBase* target, SEL_callbackHandler selector);
    
    void callBack_object_save(const char *json);
    KBase* target_object_save;
    SEL_callbackHandler selector_object_save;
    
    //
    void object_refresh(string uri
                        , KBase* target, SEL_callbackHandler selector);
    void callBack_object_refresh(const char *json);
    KBase* target_object_refresh;
    SEL_callbackHandler selector_object_refresh;
    
    //
    void object_update(string uri, picojson::object key_value_pairs
                       , KBase* target, SEL_callbackHandler selector);
    void callBack_object_update(const char *json);
    KBase* target_object_update;
    SEL_callbackHandler selector_object_update;
    
    void query(std::shared_ptr<CKiiQuery>& query
               , KBase* target, SEL_callbackHandler selector);
    void callBack_query(     const char *json);
    KBase* target_object_query2;
    SEL_callbackHandler selector_object_query2;
    
};

#endif /* CKIIBUCKET_H_ */
