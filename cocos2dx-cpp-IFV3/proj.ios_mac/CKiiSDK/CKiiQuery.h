//
//  CKiiQuery.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiQuery__
#define __cocos2dx_cpp_IFV3__CKiiQuery__

#include <iostream>
#include "picojson.h"
#include "CKiiClause.h"

namespace kiicloud {

class CKiiQuery
{
public:
    explicit CKiiQuery(const CKiiClause& clause = CKiiClause(), const int bestEffortLimit = 0);
    explicit CKiiQuery(const CKiiQuery &query, const std::string &paginationKey);
    void sortByASC(const std::string& sortKey);
    void sortByDSC(const std::string& sortKey);
    std::string toString() const;
private:
    picojson::object jsonQuery;
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiQuery__) */
