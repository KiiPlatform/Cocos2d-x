//
//  KiiSample.h
//  MyApp
//
//  Created by Guest on 2014/01/31.
//
//

#ifndef MyApp_KiiSample_h
#define MyApp_KiiSample_h

#include "cocos2d.h"
#include "picojson.h"
#include "kii/KObject.h"

using namespace std;
using namespace cocos2d;

class ScoreData {
public:
    
	std::string name;
	std::string score;
    
	ScoreData(std::string a, std::string b){ name = a; score=b;}
};

class KiiSample : public KObject
{
public:
	KiiSample(void);
	virtual ~KiiSample(void);

	static KiiSample* create();
    bool init();

    void ranking_query_all();
    void ranking_query();
    void displayNameUpdate(const char *buff);
    
    void callBack_init(              const char *json);
    void callBack_ranking_query_all( const char *json);
    void callBack_ranking_query(     const char *json);
    void callBack_displayNameUpdate( const char *json);
    
    void createApplicationScopeBucket();
    void callBack_createApplicationScopeBucket(const char *json);

    void object_save();
    void callBack_object_save(const char *json);

    void object_refresh();
    void callBack_object_refresh(const char *json);

    void object_saveAllFields();
    void callBack_object_saveAllFields(const char *json);

    void object_saveTest1();
    void object_saveTest2(picojson::object v);

    string _backet_key;
    string _userDisplayName;
    string _uri;
    vector<ScoreData> _vScoreData;

    string _score;
    string _mode;
    string _premiumUser;

};

#endif
