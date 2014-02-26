//
//  KiiCPP.cpp
//  MyApp
//
//  Created by Guest on 2014/01/31.
//
//

#include "KiiCPP.h"
#include "picojson.h"
#include "kii/KObject.h"

#define callback_selector(_SELECTOR) (SEL_callbackHandler)(&_SELECTOR)
typedef void (KObject::*SEL_callbackHandler)(const char *json);

extern void jni_kiiReq(const char *json, int serviceID);

int g_serviceID;
std::map<int, SEL_callbackHandler> selecter_map;
std::map<int, KObject*> target_map;

void serialize() {
    picojson::object v;
    picojson::object inner;
    string val = "tt";

    v["aa"] = picojson::value(val);
    v["bb"] = picojson::value(1.66);
    inner["test"] =  picojson::value(true);
    inner["integer"] =  picojson::value(1.0);
    v["inner"] =  picojson::value(inner);

    string str = picojson::value(v).serialize();
    //printf("serialized content = %s\r\n" ,  str.c_str());
    CCLOG("serialize str %s",str.c_str());
}

void kiiReq( map<string, string> params, KObject* target, SEL_callbackHandler selector ){
	CCLOG("kiiReq");

    g_serviceID++;
    selecter_map.insert( make_pair(g_serviceID, selector ));
    target_map.insert(   make_pair(g_serviceID, target ));
    
    // 要素を出力する
    map<string, string>::iterator it = params.begin();
    int i = 0;
    string key,val;
    picojson::object v;
    while( it != params.end() ) {
    	i++;
    	CCLOG("params %d %s %s",i, (*it).first.c_str(), (*it).second.c_str() );
    	//cout << (*it).first << ":" << (*it).second << endl;
    	key = (*it).first;
    	val = (*it).second;
        v[key] = picojson::value(val);

    	++it;	//ループの最後で++する
    }
    string json = picojson::value(v).serialize();
    //char buff[256];
    jni_kiiReq(json.c_str(), g_serviceID);  //Javaをコールする
}


void kiiReq2( picojson::object set_pairs
		, KObject* target, SEL_callbackHandler selector )
{
    g_serviceID++;
    selecter_map.insert( make_pair(g_serviceID, selector ));
    target_map.insert(   make_pair(g_serviceID, target ));

    string json = picojson::value(set_pairs).serialize();
    jni_kiiReq(json.c_str(), g_serviceID);  //Javaをコールする
}


void kiiRes(const char *json, int serviceID){
	CCLOG("kiiRes %d %s", serviceID, json);

    SEL_callbackHandler selecter = selecter_map[serviceID];
    KObject* target = target_map[serviceID];
    CCLOG("kiiRes2");
    if (target && selecter)
    {
        CCLOG("kiiRes3");
        (target->*selecter)(json);  //コールバックを実行する
    }
    CCLOG("kiiRes4");
}
