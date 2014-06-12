//
//  CKiiClause.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/10.
//
//

#include "CKiiClause.h"

kiicloud::CKiiClause::CKiiClause()
{
    picojson::object cls;
    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("all")));
    this->clause = cls;
}

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

kiicloud::CKiiClause kiicloud::CKiiClause::startsWith(const std::string &key,
                                                      const std::string &prefix)
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
    
    std::vector<const kiicloud::CKiiClause>::const_iterator itr = clauses.begin();
    while (itr != clauses.end())
    {
        jsarray.push_back(picojson::value((*itr).clause));
        ++itr;
    }

    cls.insert(std::pair<std::string, picojson::value>("type", picojson::value("or")));
    cls.insert(std::pair<std::string, picojson::value>("clauses", picojson::value(jsarray)));
    return CKiiClause(cls);
}

kiicloud::CKiiClause kiicloud::CKiiClause::andClause(const std::vector<CKiiClause> &clauses)
{
    picojson::object cls;
    picojson::array jsarray;
    
    std::vector<const kiicloud::CKiiClause>::const_iterator itr = clauses.begin();
    while (itr != clauses.end())
    {
        jsarray.push_back(picojson::value((*itr).clause));
        ++itr;
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