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

/**
* @brief 各種条件のClauseを作成する、作成したClauseはstd::shared_ptrで返るのでdeleteの必要はない
* Clauseの作成はstaticメソッドを使う
*/
class CKiiClause {
public:
	CKiiClause();
	virtual ~CKiiClause();

	picojson::object _json; //!< @brief 条件文をjson形式で保持する

	//equals

	static std::shared_ptr<CKiiClause> equals(const string& key, int value);
	//static std::shared_ptr<CKiiClause> equals2(const string& key, int value);
	static std::shared_ptr<CKiiClause> equals(const string& key, double value);
	static std::shared_ptr<CKiiClause> equals(const string& key, const string& value);

	//notEquals
	static std::shared_ptr<CKiiClause> notEquals(const string& key, int value);
	static std::shared_ptr<CKiiClause> notEquals(const string& key, double value);
	static std::shared_ptr<CKiiClause> notEquals(const string& key, const string& value);

	//greaterThan
	static std::shared_ptr<CKiiClause> greaterThan(const string& key, int value);
	static std::shared_ptr<CKiiClause> greaterThan(const string& key, double value);
	static std::shared_ptr<CKiiClause> greaterThan(const string& key, const string& value);

	//greaterThanOrEqual
	static std::shared_ptr<CKiiClause> greaterThanOrEqual(const string& key, int value);
	static std::shared_ptr<CKiiClause> greaterThanOrEqual(const string& key, double value);
	static std::shared_ptr<CKiiClause> greaterThanOrEqual(const string& key, const string& value);

	//equals
	static std::shared_ptr<CKiiClause> lessThan(const string& key, int value);
	static std::shared_ptr<CKiiClause> lessThan(const string& key, double value);
	static std::shared_ptr<CKiiClause> lessThan(const string& key, const string& value);

	//lessThanOrEqual
	static std::shared_ptr<CKiiClause> lessThanOrEqual(const string& key, int value);
	static std::shared_ptr<CKiiClause> lessThanOrEqual(const string& key, double value);
	static std::shared_ptr<CKiiClause> lessThanOrEqual(const string& key, const string& value);

	//startsWith
	static std::shared_ptr<CKiiClause> startsWith(const string& key, const string& value);

	//or and
	static std::shared_ptr<CKiiClause> _or(int num, ...);
	static std::shared_ptr<CKiiClause> _and(int num, ...);

};

#endif /* CKIICLAUSE_H_ */
