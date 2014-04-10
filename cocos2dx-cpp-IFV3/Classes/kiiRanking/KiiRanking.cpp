//
//  kiiRanking.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by Guest on 2014/03/18.
//
//

#include "KiiRanking.h"

//ログ出力
//#define DEBUG_kiiRanking
#ifndef DEBUG_kiiRanking
#define MYCCLOG(...)       do {} while (0)
#else
#define MYCCLOG(format, ...)      cocos2d::log(format, ##__VA_ARGS__)
#endif
//--

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
	MYCCLOG("KiiRanking::create");
	KiiRanking *pSprite = new KiiRanking();
    if (pSprite && pSprite->init())
    {
        pSprite->_name = kii_name;
        pSprite->_display_name = kii_display_name;
        
        MYCCLOG("_name=%s", pSprite->_name.c_str());
        MYCCLOG("_display_name=%s", pSprite->_display_name.c_str());
        
        MYCCLOG("KiiRanking::create ok");
        return pSprite;
    }
    MYCCLOG("KiiRanking::create error");
    return NULL;
}

bool KiiRanking::init()
{
    MYCCLOG("KiiRanking::init");
    _pCKiiBucket = CKiiBucket::create();
    return true;
}

void KiiRanking::createApplicationScopeBucketTest()
{
    MYCCLOG("KiiRanking::createApplicationScopeBucketTest");
    //b_ranking03
    _pCKiiBucket->createApplicationScopeBucket("b_ranking03",	//03
                                               this, callback_selector(KiiRanking::callBack_createApplicationScopeBucketTest) );
}

void KiiRanking::callBack_createApplicationScopeBucketTest(const char *json)
{
    MYCCLOG("KiiRanking::callBack_createApplicationScopeBucketTest");
    MYCCLOG("json %s ",json );
    
    queryALL();
}

void KiiRanking::object_refreshTest()
{
    MYCCLOG("KiiRanking::object_refreshTest -----");
    _pCKiiBucket->object_refresh(_uri,
                                 this, callback_selector(KiiRanking::callBack_object_refreshTest));
}

void KiiRanking::callBack_object_refreshTest(const char *json)
{
    MYCCLOG("KiiRanking::callBack_object_refreshTest");
    MYCCLOG("json %s ",json );
}

//バケットを作成
void KiiRanking::postMyScore(int hiscore){
    MYCCLOG("KiiRanking::postMyScore ---");
    MYCCLOG("hiscore = %d", hiscore );
    _hiscore = hiscore;	//クラス変数に保存する
    //_hiscore = 5678;
    MYCCLOG("_hiscore = %d", _hiscore );
    //バケットを作成
    //b_ranking03
    _pCKiiBucket->createApplicationScopeBucket("b_ranking03",	//03
                                               this, callback_selector(KiiRanking::callBack_postMyScore) );
    MYCCLOG("KiiRanking::postMyScore end ---");
}
void KiiRanking::callBack_postMyScore(const char *json){
    MYCCLOG("KiiRanking::callBack_queryMyScore");
    MYCCLOG("json %s ",json );
    
    //エラーを確認
    string err = check_error(json);
    if(err!=""){
        MYCCLOG("err %s ",err.c_str() );
        return;
    }
    
    queryMyScore();
}

//自分のスコアを検索
void KiiRanking::queryMyScore(){
    MYCCLOG("KiiRanking::queryMyScore ---");
    
    //自分のスコアを検索
    //auto e1 = CKiiClause::equals("name","43f76824-e92b-4a8f-a34d-8fac92248250");
    auto e1 = CKiiClause::equals("name",_name);
    auto q = std::make_shared<CKiiQuery>(e1);	//new
	string s = q->toString2();
	q->sortByDesc("score");
    MYCCLOG("s = %s", s.c_str() );
    
    //set
    //picojson::object set_pairs;
    //set_pairs.insert( make_pair("query", picojson::value(q->_json) ) );	//q or q2
    _pCKiiBucket->query(q,
                        this, callback_selector(KiiRanking::callBack_queryMyScore));
    
    MYCCLOG("KiiRanking::queryMyScore end ---");
}
void KiiRanking::callBack_queryMyScore(const char *json){
    char buff[256];
    std::string err;
    

    MYCCLOG("KiiRanking::callBack_queryMyScore");
    MYCCLOG("json %s ",json );
    
    //エラーを確認
    string err1 = check_error(json);
    if(err1!=""){
        MYCCLOG("err %s ",err1.c_str() );
        return;
    }
    
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
        MYCCLOG("name=%s",name.c_str());
        
        display_name = o1["display_name"].get<std::string>();
        MYCCLOG("display_name=%s",display_name.c_str());
        
        //score = o1["score"].get<std::string>();
        //MYCCLOG("score=%s",score.c_str());

        dscore = o1["score"].get<double>();
        MYCCLOG("score=%f",dscore);
        
        _uri_ = o1["_uri_"].get<std::string>();
        MYCCLOG("_uri_=%s",_uri_.c_str());

        //sprintf(buff,"%d %s %s %s %s\n",
        //		index+1, name.c_str(), display_name.c_str(), score.c_str(), _uri_.c_str() );
        //MYCCLOG("buff=%s",buff);
        
        index++;
        break;	//１回でループを抜ける
    }
    MYCCLOG("index = %d",index);
    //
    //MYCCLOG("_uri_=%s",_uri_.c_str() );
    if(_uri_!=""){
    	MYCCLOG("score ari");
    	_name = name;
    	//_display_name = display_name;	変更前に戻ってしまうのでここでは代入しない
    	_score = score;
    	_uri = _uri_;
    	MYCCLOG("_uri=%s", _uri.c_str());
    	updateMyScore(_uri, _hiscore);	//保存していた変数を使ってスコアを更新するupdate
    } else {
    	MYCCLOG("score nasi");
    	saveMyScore(_hiscore);	//新規作成なのでsave
    }
    MYCCLOG("KiiRanking::callBack_queryMyScore end ---");
}

//スコアの更新
void KiiRanking::updateMyScore(string uri, int hiscore){
    MYCCLOG("KiiRanking::updateMyScore -----");
    MYCCLOG("hiscore = %d", hiscore );
	MYCCLOG("uri=%s", uri.c_str());
	MYCCLOG("_display_name=%s", _display_name.c_str());
    
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
    MYCCLOG("KiiRanking::callBack_updateMyScore");
    MYCCLOG("json %s ",json );
    
    //エラーを確認
    string err1 = check_error(json);
    if(err1!=""){
        MYCCLOG("err %s ",err1.c_str() );
        return;
    }
    
    object_refreshTest();
}

//スコアの新規作成
void KiiRanking::saveMyScore(int hiscore){
    MYCCLOG("KiiRanking::saveMyScore -----");
    
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
    
    MYCCLOG("KiiRanking::callBack_saveMyScore");
    MYCCLOG("json %s ",json );
    
    //エラーを確認
    string err1 = check_error(json);
    if(err1!=""){
        MYCCLOG("err %s ",err1.c_str() );
        return;
    }
    
    //_uriの保存
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::object& o = v.get<picojson::object>();
    string uri = o["uri"].get<std::string>();
    if(uri!=""){
    	_uri = uri;
    	MYCCLOG("_uri=%s ",_uri.c_str() );
    } else {
    	MYCCLOG("uri error");
    }
    object_refreshTest();
}

void KiiRanking::queryALL(){
    MYCCLOG("KiiRanking::queryALL");
	auto q = std::make_shared<CKiiQuery>();	//new
	q->sortByDesc("score");
	string s = q->toString2();
    MYCCLOG("s = %s", s.c_str() );
    
    //set
    picojson::object set_pairs;
    set_pairs.insert( make_pair("query", picojson::value(q->_json) ) );	//q or q2
    
    _pCKiiBucket->query(q,
                        this, callback_selector(KiiRanking::callBack_queryALL));
    
    MYCCLOG("KiiRanking::queryALL end ---");
}
void KiiRanking::callBack_queryALL(const char *json){
    char buff[256];
    //char kii_label_buff[1024];
    std::string err;
    
    MYCCLOG("KiiRanking::callBack_queryALL");
    MYCCLOG("json %s ",json );
    
    //エラーを確認
    string err1 = check_error(json);
    if(err1!=""){
        MYCCLOG("err %s ",err1.c_str() );
        return;
    }
    
    //
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::array& a1 = v.get<picojson::array>();
    
    int index = 0, iscore;
    double dscore;
    strcpy(kii_label_buff, "");
    for (picojson::array::iterator it = a1.begin(); it != a1.end(); it++) {
        picojson::object& o1 = it->get<picojson::object>();
        std::string& s1 = o1["display_name"].get<std::string>();    //display_nameの取り出し
        //std::string& s2 = o1["score"].get<std::string>();
        
        //socre
        /***
        dscore = o1["score"].get<double>();
        iscore = (int)dscore;
        MYCCLOG("dscore=%6.1f",dscore);
        MYCCLOG("iscore=%06d",iscore);
         ***/
        
        //
        //scoreの取り出し、エラー処理付き
        //エラー処理を例として入れたもの、本来は必要ないが、サーバーから来るJSONの形式が変化した時にエラー判定出来る
        
        bool b;
        int c;
        double dscore = 0;
        string sscore;
        //json_mapに全部入れる
        map<std::string,std::string> json_map;
        //const picojson::value::object& obj = v.get<picojson::object>();
        for (picojson::value::object::const_iterator i = o1.begin(); i != o1.end(); ++i) {
            MYCCLOG("v  %s %s", i->first.c_str(), i->second.to_str().c_str() );
            json_map.insert( make_pair(i->first,i->second.to_str() ) ); //insertする
        }
        
        c = json_map.count("score");
        MYCCLOG("score c=%d",c);
        if(c>0){
            //scoreあり
            
            //スコアの型判定とデコード double　が正しい
            b = o1["score"].is<double>();
            MYCCLOG("score is double b=%d",b);
            if(b){
                dscore = o1["score"].get<double>();  //double
                iscore = (int)dscore;
                MYCCLOG("dscore=%f",dscore);
            }
            
            //スコアの型判定とデコード string こちらはエラー
            b = o1["score"].is<std::string>();
            MYCCLOG("score is string b=%d",b);
            if(b){
                sscore = o1["score"].get<std::string>(); //string
                iscore = -1; //エラー時
                MYCCLOG("sscore=%s",sscore.c_str() );
            }
        } else {
            MYCCLOG("score nai");
            iscore = -1; //エラー時
        }
        //
        
        
        //表示文字列の作成
        //sprintf(buff,"%d %s : %s \n", index+1, s1.c_str(), s2.c_str() );
        sprintf(buff,"%d %12s : %06d \n", index+1, s1.c_str(), iscore );

        MYCCLOG("buff = %s",buff);
        if(index<7){
        	strcat(kii_label_buff, buff);//ランキング表示用文字列に追加する
        }
        index++;
        //if(index>=7){
        //    break;
        //}
    }
    MYCCLOG("---");
    MYCCLOG("kii_label_buff %s",kii_label_buff);
    //
}

string KiiRanking::check_error(const char *json){
    MYCCLOG("KiiRanking::check_error");
    MYCCLOG("json %s ",json );
          
    std::string err;
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    
    string _error_;
    bool b;
    b = v.is<picojson::object>();
    if(b){
        picojson::object& o = v.get<picojson::object>();
        
        b = o["_error_"].is<std::string>();
        if(b){
            _error_ = o["_error_"].get<std::string>();
        }
        
    } else {
        _error_  ="";
    }
    MYCCLOG("_error_=%s",_error_.c_str() );
    
    return _error_;
    
}
