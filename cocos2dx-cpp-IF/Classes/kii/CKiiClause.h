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

	static CKiiClause* equals(string key, int value);
	static CKiiClause* equals(string key, bool value);
	static CKiiClause* equals(string key, string value);

	static CKiiClause* _or(CKiiClause* clauses, ...);

};

#endif /* CKIICLAUSE_H_ */
