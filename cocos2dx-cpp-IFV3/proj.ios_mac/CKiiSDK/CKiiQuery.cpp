//
//  CKiiQuery.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#include "CKiiQuery.h"

kiicloud::CKiiQuery::CKiiQuery()
{
    static const std::string aq = std::string("{\"bucketQuery\": { \"clause\": { \"type\" : \"all\"}}}");
    picojson::value v;
    std::string err;
    picojson::parse(v, aq.c_str(), aq.c_str() + strlen(aq.c_str()), &err);
    if (!err.empty())
    {
        abort();
    }
    this->jsonQuery = v.get<picojson::object>();
}

std::string kiicloud::CKiiQuery::toString() const
{
    picojson::value v = picojson::value(jsonQuery);
    return v.serialize();
}