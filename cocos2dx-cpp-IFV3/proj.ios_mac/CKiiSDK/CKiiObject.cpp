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
using kiicloud::ObjFuture;
using kiicloud::ErrorFuture;

kiicloud::CKiiObject::CKiiObject(const std::string& scopeUri, const std::string& bucketName, picojson::object values)
:_values(values),
_scopeUri(scopeUri),
_bucketName(bucketName)
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
_version(lv._version),
_scopeUri(lv._scopeUri),
_bucketName(lv._bucketName)
{
}

kiicloud::CKiiObject::CKiiObject(CKiiObject&& lv)
:_values(lv._values),
_id(lv._id),
_ownerUserId(lv._ownerUserId),
_modified(lv._modified),
_created(lv._created),
_version(lv._version),
_scopeUri(lv._scopeUri),
_bucketName(lv._bucketName)
{
    lv._values = picojson::object();
    lv._id = "";
    lv._ownerUserId = "";
    lv._modified = 0;
    lv._created = 0;
    lv._version = "";
    lv._scopeUri = "";
    lv._bucketName = "";
}

std::string kiicloud::CKiiObject::getId() const
{
    return _id;
}

std::string kiicloud::CKiiObject::getVersion() const
{
    return _version;
}

std::string kiicloud::CKiiObject::getUri() const
{
    return _scopeUri + "/buckets/" + _bucketName + "/objects/" + _id;
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

ObjFuture kiicloud::CKiiObject::saveNewObject(
                                              const kiicloud::CKiiApp &app,
                                              const std::string &scopeUri,
                                              const std::string &bucketName,
                                              const picojson::object &values,
                                              const std::string &accessToken)
{
    auto *p = new std::promise<std::pair<ObjPtr, ErrorPtr>>;
    std::thread th = std::thread([=]() {
        _bind->saveNewObject(app, scopeUri, bucketName, values, accessToken,
                             [=](picojson::value objValue, CKiiError* error)
                             {
                                 CKiiObject *obj = nullptr;
                                 if (error == nullptr) {
                                     obj = new CKiiObject(scopeUri, bucketName, objValue.get<picojson::object>());
                                 }
                                 std::pair<ObjPtr, ErrorPtr> pr((ObjPtr(obj)), ErrorPtr(error));
                                 p->set_value(pr);
                                 delete p;
                             });
    });
    th.detach();
    return p->get_future();
}

ErrorFuture kiicloud::CKiiObject::patchObject(const kiicloud::CKiiApp &app,
                                              kiicloud::CKiiObject &targetObject,
                                              const picojson::object &patch,
                                              const std::string &accessToken,
                                              bool forceUpdate)
{
    auto *prm = new std::promise<ErrorPtr>();
    std::thread th = std::thread([=, &targetObject]() {
        _bind->patchObject(app,
                           targetObject.getUri(),
                           targetObject.getVersion(),
                           patch,
                           accessToken,
                           forceUpdate,
                           [=, &targetObject](picojson::value vals, std::string& etag, CKiiError *error) {
                               ErrorPtr ret = ErrorPtr(error);
                               
                               // Add values returned from server.
                               picojson::object obj = vals.get<picojson::object>();
                               picojson::object::const_iterator itr = obj.begin();
                               while (itr != obj.end()) {
                                   targetObject._values.insert(*itr);
                               }
                               
                               // Update ETag.
                               if (!etag.empty())
                                   targetObject._version = etag;
                               prm->set_value(ret);
                               delete prm;
                           });
    });
    th.detach();
    return prm->get_future();
}
