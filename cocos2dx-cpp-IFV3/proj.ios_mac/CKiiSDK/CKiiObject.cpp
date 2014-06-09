//
//  CKiiObject.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#include "CKiiObject.h"
#include <cmath>

kiicloud::CKiiObject::CKiiObject(picojson::object values)
:_values(values)
{
    // Parse contents.
    this->_id = values["_id"].get<std::string>();
    this->_version = values["_version"].get<std::string>();
    this->_ownerUserId = values["_owner"].get<std::string>();

    this->_created = values["_created"].get<double>();
    this->_modified = values["_modified"].get<double>();
}

kiicloud::CKiiObject::CKiiObject(const CKiiObject& lv)
:_values(lv._values),
_id(lv._id),
_ownerUserId(lv._ownerUserId),
_modified(lv._modified),
_created(lv._created),
_version(lv._version)
{
}

kiicloud::CKiiObject::CKiiObject(CKiiObject&& lv)
:_values(lv._values),
_id(lv._id),
_ownerUserId(lv._ownerUserId),
_modified(lv._modified),
_created(lv._created),
_version(lv._version)
{
    lv._values = picojson::object();
    lv._id = "";
    lv._ownerUserId = "";
    lv._modified = 0;
    lv._created = 0;
    lv._version = "";
}

std::string kiicloud::CKiiObject::getId() const
{
    return _id;
}

std::string kiicloud::CKiiObject::getVersion() const
{
    return _version;
}

std::string kiicloud::CKiiObject::getOwnerUserId() const
{
    return _ownerUserId;
}

long long kiicloud::CKiiObject::getModified() const
{
    return _modified;
}

long long kiicloud::CKiiObject::getCreated() const
{
    return _created;
}