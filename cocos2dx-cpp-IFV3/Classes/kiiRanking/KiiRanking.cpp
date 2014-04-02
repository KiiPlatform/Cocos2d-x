//
//  kiiRanking.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by Guest on 2014/03/18.
//
//

#include "kiiRanking.h"

/*
 * CKiiApiTest.cpp
 *
 *  Created on: 2014/02/10
 *      Author: guest
 */

#include "KiiRanking.h"

//実体をここに置く
char kii_label_buff[1024];
char kii_name[256];
char kii_display_name[256];

KiiRanking::KiiRanking() {
    
}

KiiRanking::~KiiRanking() {
    
}

KiiRanking* KiiRanking::create()
{
	CCLOG("KiiRanking::create");
	KiiRanking *pSprite = new KiiRanking();
    if (pSprite && pSprite->init())
    {
        //pSprite->autorelease();
        //pSprite->retain();	//※ 後に追加した部分 ※　ないとKiiSample::~KiiSampleが呼ばれる
        pSprite->_name = kii_name;
        pSprite->_display_name = kii_display_name;
        
        CCLOG("_name=%s", pSprite->_name.c_str());
        CCLOG("_display_name=%s", pSprite->_display_name.c_str());
        
        CCLOG("KiiRanking::create ok");
        return pSprite;
    }
    CCLOG("KiiRanking::create error");
    //CC_SAFE_DELETE(pSprite);
    return NULL;
}

bool KiiRanking::init()
{
    CCLOG("KiiRanking::init");
    _pCKiiBucket = CKiiBucket::create();
    return true;
}

void KiiRanking::createApplicationScopeBucketTest()
{
    CCLOG("KiiRanking::createApplicationScopeBucketTest");
    //b_ranking03
    _pCKiiBucket->createApplicationScopeBucket("b_ranking03",	//03
                                               this, callback_selector(KiiRanking::callBack_createApplicationScopeBucketTest) );
}

void KiiRanking::callBack_createApplicationScopeBucketTest(const char *json)
{
    CCLOG("KiiRanking::callBack_createApplicationScopeBucketTest");
    CCLOG("json %s ",json );
    
    queryALL();
}

void KiiRanking::object_refreshTest()
{
    CCLOG("KiiRanking::object_refreshTest -----");
    _pCKiiBucket->object_refresh(_uri,
                                 this, callback_selector(KiiRanking::callBack_object_refreshTest));
}

void KiiRanking::callBack_object_refreshTest(const char *json)
{
    CCLOG("KiiRanking::callBack_object_refreshTest");
    CCLOG("json %s ",json );
}

//バケットを作成
void KiiRanking::postMyScore(int hiscore){
    CCLOG("KiiRanking::postMyScore ---");
    CCLOG("hiscore = %d", hiscore );
    _hiscore = hiscore;	//クラス変数に保存する
    //_hiscore = 5678;
    CCLOG("_hiscore = %d", _hiscore );
    //バケットを作成
    //b_ranking03
    _pCKiiBucket->createApplicationScopeBucket("b_ranking03",	//03
                                               this, callback_selector(KiiRanking::callBack_postMyScore) );
    CCLOG("KiiRanking::postMyScore end ---");
}
void KiiRanking::callBack_postMyScore(const char *json){
    CCLOG("KiiRanking::callBack_queryMyScore");
    CCLOG("json %s ",json );
    queryMyScore();
}

//自分のスコアを検索
void KiiRanking::queryMyScore(){
    CCLOG("KiiRanking::queryMyScore ---");
    
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
                        this, callback_selector(KiiRanking::callBack_queryMyScore));
    
    CCLOG("KiiRanking::queryMyScore end ---");
}
void KiiRanking::callBack_queryMyScore(const char *json){
    char buff[256];
    std::string err;
    

    CCLOG("KiiRanking::callBack_queryMyScore");
    CCLOG("json %s ",json );
    
    //
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::array& a1 = v.get<picojson::array>();
    
    int index = 0;
    double dscore;
    std::string _uri_, name, display_name, score;
    for (picojson::array::iterator it = a1.begin(); it != a1.end(); it++) {
        picojson::object& o1 = it->get<picojson::object>();
        
        name = o1["name"].get<std::string>();
        CCLOG("name=%s",name.c_str());
        
        display_name = o1["display_name"].get<std::string>();
        CCLOG("display_name=%s",display_name.c_str());
        
        //score = o1["score"].get<std::string>();
        //CCLOG("score=%s",score.c_str());

        dscore = o1["score"].get<double>();
        CCLOG("score=%f",dscore);
        
        _uri_ = o1["_uri_"].get<std::string>();
        CCLOG("_uri_=%s",_uri_.c_str());

        //sprintf(buff,"%d %s %s %s %s\n",
        //		index+1, name.c_str(), display_name.c_str(), score.c_str(), _uri_.c_str() );
        //CCLOG("buff=%s",buff);
        
        index++;
        break;	//１回でループを抜ける
    }
    CCLOG("index = %d",index);
    //
    //CCLOG("_uri_=%s",_uri_.c_str() );
    if(_uri_!=""){
    	CCLOG("score ari");
    	_name = name;
    	//_display_name = display_name;	変更前に戻ってしまうのでここでは代入しない
    	_score = score;
    	_uri = _uri_;
    	CCLOG("_uri=%s", _uri.c_str());
    	updateMyScore(_uri, _hiscore);	//保存していた変数を使ってスコアを更新するupdate
    } else {
    	CCLOG("score nasi");
    	saveMyScore(_hiscore);	//新規作成なのでsave
    }
    CCLOG("KiiRanking::callBack_queryMyScore end ---");
}

//スコアの更新
void KiiRanking::updateMyScore(string uri, int hiscore){
    CCLOG("KiiRanking::updateMyScore -----");
    CCLOG("hiscore = %d", hiscore );
	CCLOG("uri=%s", uri.c_str());
	CCLOG("_display_name=%s", _display_name.c_str());
    
    //set
    picojson::object set_pairs;
    set_pairs.insert( make_pair("set_score", picojson::value((double)hiscore) ) );
    set_pairs.insert( make_pair("set_name", picojson::value(_name) ) );
    set_pairs.insert( make_pair("set_display_name", picojson::value(_display_name) ) );
    
    string uri2 = uri;
    _pCKiiBucket->object_update(uri2, set_pairs,
                                this, callback_selector(KiiRanking::callBack_updateMyScore));
}
void KiiRanking::callBack_updateMyScore(const char *json){
    CCLOG("KiiRanking::callBack_updateMyScore");
    CCLOG("json %s ",json );
    
    object_refreshTest();
}

//スコアの新規作成
void KiiRanking::saveMyScore(int hiscore){
    CCLOG("KiiRanking::saveMyScore -----");
    
    //set
    picojson::object set_pairs;
    set_pairs.insert( make_pair("set_score", picojson::value((double)hiscore) ) );
    set_pairs.insert( make_pair("set_name", picojson::value(_name) ) );
    set_pairs.insert( make_pair("set_display_name", picojson::value(_display_name) ) );
    
    _pCKiiBucket->object_save(set_pairs,
                              this, callback_selector(KiiRanking::callBack_saveMyScore));
}
void KiiRanking::callBack_saveMyScore(const char *json){
    std::string err;
    
    CCLOG("KiiRanking::callBack_saveMyScore");
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

void KiiRanking::queryALL(){
    CCLOG("KiiRanking::queryALL");
	auto q = std::make_shared<CKiiQuery>();	//new
	q->sortByDesc("score");
	string s = q->toString2();
    CCLOG("s = %s", s.c_str() );
    
    //set
    picojson::object set_pairs;
    set_pairs.insert( make_pair("query", picojson::value(q->_json) ) );	//q or q2
    
    _pCKiiBucket->query(q,
                        this, callback_selector(KiiRanking::callBack_queryALL));
    
    CCLOG("KiiRanking::queryALL end ---");
}
void KiiRanking::callBack_queryALL(const char *json){
    char buff[256];
    //char kii_label_buff[1024];
    std::string err;
    
    CCLOG("KiiRanking::callBack_queryALL");
    CCLOG("json %s ",json );
    //
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::array& a1 = v.get<picojson::array>();
    
    int index = 0, iscore;
    double dscore;
    strcpy(kii_label_buff, "");
    for (picojson::array::iterator it = a1.begin(); it != a1.end(); it++) {
        picojson::object& o1 = it->get<picojson::object>();
        std::string& s1 = o1["display_name"].get<std::string>();
        //std::string& s2 = o1["score"].get<std::string>();
        
        dscore = o1["score"].get<double>();
        iscore = (int)dscore;
        CCLOG("dscore=%6.1f",dscore);
        CCLOG("iscore=%06d",iscore);
        
        //sprintf(buff,"%d %s : %s \n", index+1, s1.c_str(), s2.c_str() );
        sprintf(buff,"%d %12s : %06d \n", index+1, s1.c_str(), iscore );

        CCLOG("buff = %s",buff);
        if(index<7){
        	strcat(kii_label_buff, buff);//ランキング表示用文字列に追加する
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
