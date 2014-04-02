/*
 * CKiiApiTest.h
 *
 *  Created on: 2014/02/10
 *      Author: guest
 */

#ifndef CkiiRanking_H_
#define CkiiRanking_H_

#include "cocos2d.h"
#include "picojson.h"
#include "kii/CKiiBucket.h"
#include "kii/CKiiQuery.h"
#include "kii/KBase.h"

using namespace std;
using namespace cocos2d;

class KiiRanking : public KBase
{
public:
	KiiRanking();
	virtual ~KiiRanking();

	static KiiRanking* create();
    bool init();

    void createApplicationScopeBucketTest();
    void callBack_createApplicationScopeBucketTest(const char *json);

    void object_refreshTest();
    void callBack_object_refreshTest(const char *json);

    void postMyScore(int hiscore);
    void callBack_postMyScore(const char *json);

    void queryMyScore();
    void callBack_queryMyScore(const char *json);

    void updateMyScore(string uri, int hiscore);
    void callBack_updateMyScore(const char *json);

    void saveMyScore(int hiscore);
    void callBack_saveMyScore(const char *json);

    void queryALL();
    void callBack_queryALL(const char *json);

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
