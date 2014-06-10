//
//  CKiiClause.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/10.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiClause__
#define __cocos2dx_cpp_IFV3__CKiiClause__

#include <iostream>
#include <vector>
#include "picojson.h"

namespace kiicloud {

class CKiiClause {

public:
    CKiiClause();
    CKiiClause(const CKiiClause&);
    CKiiClause(CKiiClause&&);
    CKiiClause(const picojson::object& clause);

    template<typename T>
    static CKiiClause equals(const std::string& key, const T& value)
    {
        picojson::object cls;
        cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("eq")));
        cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
        cls.insert(std::pair<std::string, picojson::value>("value", picojson::value(value)));
        return CKiiClause(cls);
    }

    template<typename T>
    static CKiiClause greaterThan(const std::string& key, const T& value)
    {
        picojson::object cls;
        cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("range")));
        cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
        cls.insert(std::pair<std::string, picojson::value>("lowerLimit", picojson::value(value)));
        cls.insert(std::pair<std::string, picojson::value>("lowerIncluded", picojson::value(false)));
        return CKiiClause(cls);
    }

    template<typename T>
    static CKiiClause greaterThanOrEqual(const std::string& key, const T& value)
    {
        picojson::object cls;
        cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("range")));
        cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
        cls.insert(std::pair<std::string, picojson::value>("lowerLimit", picojson::value(value)));
        cls.insert(std::pair<std::string, picojson::value>("lowerIncluded", picojson::value(true)));
        return CKiiClause(cls);
    }

    template<typename T>
    static CKiiClause lessThan(const std::string& key, const T& value)
    {
        picojson::object cls;
        cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("range")));
        cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
        cls.insert(std::pair<std::string, picojson::value>("upperLimit", picojson::value(value)));
        cls.insert(std::pair<std::string, picojson::value>("upperIncluded", picojson::value(false)));
        return CKiiClause(cls);
    }


    template<typename T>
    static CKiiClause lessThanOrEqual(const std::string& key, const T& value)
    {
        picojson::object cls;
        cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("range")));
        cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
        cls.insert(std::pair<std::string, picojson::value>("upperLimit", picojson::value(value)));
        cls.insert(std::pair<std::string, picojson::value>("upperIncluded", picojson::value(true)));
        return CKiiClause(cls);
    }

    template<typename T>
    static CKiiClause inClause(const std::string& key, const std::vector<T>& values)
    {
        picojson::array jsarray;
        typename std::vector<T>::const_iterator itr = values.begin();
        while (itr != values.end())
        {
            jsarray.push_back(picojson::value((*itr)));
            ++itr;
        }
        picojson::object cls;
        cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("in")));
        cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
        cls.insert(std::pair<std::string, picojson::value>("values", picojson::value(jsarray)));
        return CKiiClause(cls);
    }

    static CKiiClause startsWith(const std::string& key, const std::string& prefix);

    static CKiiClause orClause(const std::vector<CKiiClause> &clauses);
    static CKiiClause andClause(const std::vector<CKiiClause> &clauses);

    CKiiClause operator or (const CKiiClause& lv) const;
    CKiiClause operator and (const CKiiClause& lv) const;

    CKiiClause operator not () const;

    std::string toString() const;
private:
    picojson::object clause;

};

}

#endif /* defined(__cocos2dx_cpp_IFV3__CKiiClause__) */
