/*
 * CKiiQuery.h
 *
 *  Created on: 2014/02/17
 *      Author: guest
 */

#ifndef CKIIQUERY_H_
#define CKIIQUERY_H_

#include "CKiiClause.h"

#include "CKiiClause.h"
#include "picojson.h"

class CKiiQuery {
public:
	CKiiQuery();
	CKiiQuery(CKiiClause *clause);
	virtual ~CKiiQuery();

	//メソッド
	void sortByDesc(string key);
	void sortByAsc(string key);
	void setLimit(int limit);
	//void setNextPaginationKey(string key);

	picojson::object toJson();
	string toString();
	string toString2();

	//変数
	picojson::object _json;
	string nextPaginationKey;
	int _mLimit;

};

#endif /* CKIIQUERY_H_ */
