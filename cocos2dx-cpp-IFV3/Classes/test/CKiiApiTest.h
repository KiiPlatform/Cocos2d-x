/*
 * CKiiApiTest.h
 *
 *  Created on: 2014/02/10
 *      Author: guest
 */

#ifndef CKIIAPITEST_H_
#define CKIIAPITEST_H_

#include "cocos2d.h"
#include "picojson.h"
#include "kii/CKiiBucket.h"
#include "kii/CKiiQuery.h"
#include "kii/KBase.h"

using namespace std;
using namespace cocos2d;

class CKiiApiTest : public KBase
{
public:
	CKiiApiTest();
	virtual ~CKiiApiTest();

	static CKiiApiTest* create();
    bool init();

    void createApplicationScopeBucketTest();
    void callBack_createApplicationScopeBucketTest(const char *json);

    void object_saveTest();
    void callBack_object_saveTest(const char *json);

    void object_refreshTest();
    void callBack_object_refreshTest(const char *json);

    void object_updateTest();
    void callBack_object_updateTest(const char *json);

    void clause5_Test();
    void callBack_clause5_Test(const char *json);

    void clause4_Test();
    void callBack_clause4_Test(const char *json);

    void clause3_Test();
    void callBack_clause3_Test(const char *json);

    void clause2_Test();
    
    void postMyScore(int hiscore);
    void callBack_postMyScore(const char *json);
    
    void clause1_Test();
    void callBack_clause1_Test(const char *json);


    CKiiBucket *_pCKiiBucket;
    string _backet_key;
    string _name, _display_name;
    string _uri;

    string _score;
    string _mode;
    string _premiumUser;

    int _hiscore;
};

#endif /* CKIIAPITEST_H_ */