//
//  CKiiObject.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#include "CKiiObject.h"
#include "_CKiiGlobal.h"
#include <thread>

using kiicloud::ObjPtr;
using kiicloud::ErrorPtr;

kiicloud::CKiiObject::CKiiObject(picojson::object values)
:_values(values)
{
    // Parse contents.
    if (values["_id"].is<std::string>())
        this->_id = values["_id"].get<std::string>();
    if (values["objectID"].is<std::string>())
        this->_id = values["objectID"].get<std::string>();

    if (values["_version"].is<std::string>())
        this->_version = values["_version"].get<std::string>();

    if (values["_owner"].is<std::string>())
        this->_ownerUserId = values["_owner"].get<std::string>();

    if (values["_created"].is<double>())
        this->_created = values["_created"].get<double>();
    if (values["createdAt"].is<double>())
        this->_created = values["createdAt"].get<double>();

    if (values["_modified"].is<double>())
        this->_modified = values["_modified"].get<double>();
    if (values["modifiedAt"].is<double>())
        this->_created = values["modifiedAt"].get<double>();
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

picojson::object kiicloud::CKiiObject::getValues() const
{
    return _values;
}

std::future<std::pair<ObjPtr, ErrorPtr>>
kiicloud::CKiiObject::saveNewObject(
                                    const kiicloud::CKiiApp &app,
                                    const std::string &scopeUri,
                                    const std::string &bucketName,
                                    const picojson::object values,
                                    const std::string &accessToken)
{
    auto *p = new std::promise<std::pair<ObjPtr, ErrorPtr>>;
    std::thread th = std::thread([=]() {
        _bind->saveNewObject(app, scopeUri, bucketName, values, accessToken,
                             [=](picojson::value objValue, CKiiError* error)
                             {
                                 CKiiObject *obj = nullptr;
                                 if (error == nullptr) {
                                     obj = new CKiiObject(objValue.get<picojson::object>());
                                 }
                                 std::pair<ObjPtr, ErrorPtr> pr((ObjPtr(obj)), ErrorPtr(error));
                                 p->set_value(pr);
                                 delete p;
                             });
    });
    th.detach();
    return p->get_future();
}