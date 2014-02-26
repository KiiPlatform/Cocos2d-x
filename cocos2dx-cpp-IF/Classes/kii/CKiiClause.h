/*
 * CKiiClause.h
 *
 *  Created on: 2014/02/10
 *      Author: guest
 */

#ifndef CKIICLAUSE_H_
#define CKIICLAUSE_H_

#include "cocos2d.h"
#include "picojson.h"

using namespace std;
using namespace cocos2d;

class CKiiClause {
public:
	CKiiClause();
	virtual ~CKiiClause();

	picojson::object _json;

	//equals
/***
	static CKiiClause* equals(string key, int value);
	static CKiiClause* equals(string key, double value);
	static CKiiClause* equals(string key, string value);
***/
	static CKiiClause* equals(const string& key, int value);
	static CKiiClause* equals(const string& key, double value);
	static CKiiClause* equals(const string& key, string value);


	//notEquals
	static CKiiClause* notEquals(const string& key, int value);
	static CKiiClause* notEquals(const string& key, double value);
	static CKiiClause* notEquals(const string& key, string value);

	//greaterThan
	static CKiiClause* greaterThan(const string& key, int value);
	static CKiiClause* greaterThan(const string& key, double value);
	static CKiiClause* greaterThan(const string& key, string value);

	//greaterThanOrEqual
	static CKiiClause* greaterThanOrEqual(const string& key, int value);
	static CKiiClause* greaterThanOrEqual(const string& key, double value);
	static CKiiClause* greaterThanOrEqual(const string& key, string value);

	//equals
	static CKiiClause* lessThan(const string& key, int value);
	static CKiiClause* lessThan(const string& key, double value);
	static CKiiClause* lessThan(const string& key, string value);

	//lessThanOrEqual
	static CKiiClause* lessThanOrEqual(const string& key, int value);
	static CKiiClause* lessThanOrEqual(const string& key, double value);
	static CKiiClause* lessThanOrEqual(const string& key, string value);

	//startsWith
	static CKiiClause* startsWith(const string& key, string value);

	//or and
	static CKiiClause* _or(int num, ...);
	static CKiiClause* _and(int num, ...);

};

#endif /* CKIICLAUSE_H_ */
