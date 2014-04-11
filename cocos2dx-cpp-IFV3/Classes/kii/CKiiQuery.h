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

/**
*　@brief Queryを作成する、_jsonに保存
* Bucketと組み合わせて使用
*/
class CKiiQuery {
public:
	CKiiQuery();
	CKiiQuery(CKiiClause *clause);
	CKiiQuery(std::shared_ptr<CKiiClause>&);
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
	picojson::object _json; //!< @brief 条件文をjson形式で保持する
	string nextPaginationKey;
	int _mLimit;

};

#endif /* CKIIQUERY_H_ */
