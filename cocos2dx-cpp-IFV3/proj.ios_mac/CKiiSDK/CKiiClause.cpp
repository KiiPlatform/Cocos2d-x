//
//  CKiiClause.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/10.
//
//

#include "CKiiClause.h"

kiicloud::CKiiClause::CKiiClause(const CKiiClause& lv)
:clause(lv.clause)
{
}

kiicloud::CKiiClause::CKiiClause(CKiiClause&& lv)
:clause(lv.clause)
{
    lv.clause = picojson::object();
}

kiicloud::CKiiClause::CKiiClause(const picojson::object &clause)
:clause(clause)
{
}

template<typename T>
kiicloud::CKiiClause kiicloud::CKiiClause::equals(const std::string& key, const T& value)
{
    picojson::object cls;
    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("eq")));
    cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
    cls.insert(std::pair<std::string, picojson::value>("value", picojson::value(value)));
    return CKiiClause(cls);
}

template<typename T>
kiicloud::CKiiClause kiicloud::CKiiClause::greaterThan(const std::string& key, const T& value)
{
    picojson::object cls;
    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("range")));
    cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
    cls.insert(std::pair<std::string, picojson::value>("lowerLimit", picojson::value(value)));
    cls.insert(std::pair<std::string, picojson::value>("lowerIncluded", picojson::value(false)));
    return CKiiClause(cls);
}

template<typename T>
kiicloud::CKiiClause kiicloud::CKiiClause::greaterThanOrEqual(const std::string& key, const T& value)
{
    picojson::object cls;
    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("range")));
    cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
    cls.insert(std::pair<std::string, picojson::value>("lowerLimit", picojson::value(value)));
    cls.insert(std::pair<std::string, picojson::value>("lowerIncluded", picojson::value(true)));
    return CKiiClause(cls);
}

template<typename T>
kiicloud::CKiiClause kiicloud::CKiiClause::lessThan(const std::string& key, const T& value)
{
    picojson::object cls;
    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("range")));
    cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
    cls.insert(std::pair<std::string, picojson::value>("upperLimit", picojson::value(value)));
    cls.insert(std::pair<std::string, picojson::value>("upperIncluded", picojson::value(false)));
    return CKiiClause(cls);
}

template<typename T>
kiicloud::CKiiClause kiicloud::CKiiClause::lessThanOrEqual(const std::string& key, const T& value)
{
    picojson::object cls;
    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("range")));
    cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
    cls.insert(std::pair<std::string, picojson::value>("upperLimit", picojson::value(value)));
    cls.insert(std::pair<std::string, picojson::value>("upperIncluded", picojson::value(true)));
    return CKiiClause(cls);
}

template<typename T>
kiicloud::CKiiClause kiicloud::CKiiClause::inClause(const std::string& key, const std::vector<T>& values)
{
    picojson::array jsarray;
    typename std::vector<T>::iterator itr = values.begin();
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

template<typename T>
kiicloud::CKiiClause kiicloud::CKiiClause::startsWith(const std::string& key, const std::string& prefix)
{
    picojson::object cls;
    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("prefix")));
    cls.insert(std::pair<std::string, picojson::value>("field", picojson::value(key)));
    cls.insert(std::pair<std::string, picojson::value>("prefix", picojson::value(prefix)));
    return CKiiClause(cls);
}

kiicloud::CKiiClause kiicloud::CKiiClause::orClause(const std::vector<CKiiClause> &clauses)
{
    picojson::object cls;
    picojson::array jsarray;
    
    std::vector<const kiicloud::CKiiClause>::iterator itr = clauses.begin();
    while (itr != clauses.end())
    {
        jsarray.push_back(picojson::value((*itr).clause));
    }

    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("or")));
    cls.insert(std::pair<std::string, picojson::value>("clauses", picojson::value(jsarray)));
    return CKiiClause(cls);
}

kiicloud::CKiiClause kiicloud::CKiiClause::andClause(const std::vector<CKiiClause> &clauses)
{
    picojson::object cls;
    picojson::array jsarray;
    
    std::vector<const kiicloud::CKiiClause>::iterator itr = clauses.begin();
    while (itr != clauses.end())
    {
        jsarray.push_back(picojson::value((*itr).clause));
    }
    
    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("and")));
    cls.insert(std::pair<std::string, picojson::value>("clauses", picojson::value(jsarray)));
    return CKiiClause(cls);
}

kiicloud::CKiiClause kiicloud::CKiiClause::operator or (const CKiiClause& lv) const
{
    picojson::object cls;
    picojson::array jsarray;
    jsarray.push_back(picojson::value(this->clause));
    jsarray.push_back(picojson::value(lv.clause));

    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("or")));
    cls.insert(std::pair<std::string, picojson::value>("clauses", picojson::value(jsarray)));
    return CKiiClause(cls);
}

kiicloud::CKiiClause kiicloud::CKiiClause::operator and (const CKiiClause& lv) const
{
    picojson::object cls;
    picojson::array jsarray;
    jsarray.push_back(picojson::value(this->clause));
    jsarray.push_back(picojson::value(lv.clause));
    
    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("and")));
    cls.insert(std::pair<std::string, picojson::value>("clauses", picojson::value(jsarray)));
    return CKiiClause(cls);
}

kiicloud::CKiiClause kiicloud::CKiiClause::operator not() const
{
    picojson::object cls;
    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("not")));
    cls.insert(std::pair<std::string, picojson::value>("clause", picojson::value(this->clause)));
    return CKiiClause(cls);
}

std::string kiicloud::CKiiClause::toString() const
{
    return picojson::value(clause).serialize();
}