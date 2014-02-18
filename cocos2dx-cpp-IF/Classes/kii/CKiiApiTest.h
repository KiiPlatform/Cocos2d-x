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
#include "CKiiBucket.h"

using namespace std;
using namespace cocos2d;

class CKiiApiTest : public CCObject
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

    void clause1_Test();

    CKiiBucket *_pCKiiBucket;
    string _backet_key;
    string _userDisplayName;
    string _uri;

    string _score;
    string _mode;
    string _premiumUser;
};

#endif /* CKIIAPITEST_H_ */
