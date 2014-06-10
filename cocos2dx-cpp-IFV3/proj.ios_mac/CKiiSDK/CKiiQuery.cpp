//
//  CKiiQuery.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#include "CKiiQuery.h"

kiicloud::CKiiQuery::CKiiQuery(const CKiiClause& clause, const int bestEffortLimit)
{
    std::string qs = clause.toString();
    picojson::value v;
    std::string err;
    picojson::parse(v, qs.c_str(), qs.c_str()+strlen(qs.c_str()), &err);

    picojson::object jsq;
    picojson::object innerBq;
    innerBq.insert(std::pair<std::string, picojson::value>("clause", v));
    jsq.insert(std::pair<std::string, picojson::value>("bucketQuery", picojson::value(innerBq)));

    this->jsonQuery = jsq;
    if (bestEffortLimit > 0) {
        this->jsonQuery["bestEffortLimit"] = picojson::value((double)bestEffortLimit);
    }
}

kiicloud::CKiiQuery::CKiiQuery(const CKiiQuery &query, const std::string &paginationKey)
:jsonQuery(query.jsonQuery)
{
    picojson::value pvPKey(paginationKey);
    this->jsonQuery["paginationKey"] = pvPKey;
}

void::kiicloud::CKiiQuery::sortByASC(const std::string &sortKey)
{
    picojson::object innerBq = this->jsonQuery["bucketQuery"].get<picojson::object>();
    innerBq.insert(std::pair<std::string, picojson::value>("orderBy", picojson::value(sortKey)));
    innerBq.insert(std::pair<std::string, picojson::value>("descending", picojson::value(false)));
    this->jsonQuery["bucketQuery"] = picojson::value(innerBq);
}

void::kiicloud::CKiiQuery::sortByDSC(const std::string &sortKey)
{
    picojson::object innerBq = this->jsonQuery["bucketQuery"].get<picojson::object>();
    innerBq.insert(std::pair<std::string, picojson::value>("orderBy", picojson::value(sortKey)));
    innerBq.insert(std::pair<std::string, picojson::value>("descending", picojson::value(true)));
    this->jsonQuery["bucketQuery"] = picojson::value(innerBq);
}

std::string kiicloud::CKiiQuery::toString() const
{
    picojson::value v = picojson::value(jsonQuery);
    return v.serialize();
}