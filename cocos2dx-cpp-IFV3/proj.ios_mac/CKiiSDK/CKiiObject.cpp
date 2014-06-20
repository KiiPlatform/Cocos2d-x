//
//  CKiiObject.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#include "CKiiObject.h"
#include "_CKiiGlobal.h"
#include "CKiiLog.h"
#include <thread>

using kiicloud::ObjPtr;
using kiicloud::ErrorPtr;
using kiicloud::ObjFuture;
using kiicloud::ErrorFuture;

kiicloud::CKiiObject::CKiiObject(const std::string& scopeUri, const std::string& bucketName, const picojson::object &values)
:_values(values),
_scopeUri(scopeUri),
_bucketName(bucketName)
{
    this->updateValues(values);
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

void kiicloud::CKiiObject::updateValues(const picojson::object &values)
{
    picojson::object::const_iterator itr = values.begin();
    while (itr != values.end()) {
        auto v = (*itr);
        if (v.first == "_id" || v.first == "objectID") {
            if (v.second.is<std::string>())
                this->_id = v.second.get<std::string>();
        }
        if (v.first == "_version") {
            if (v.second.is<std::string>())
                this->_version = v.second.get<std::string>();
        }
        if (v.first == "_owner") {
            if (v.second.is<std::string>())
                this->_ownerUserId = v.second.get<std::string>();
        }
        if (v.first == "_created" || v.first == "createdAt") {
            if (v.second.is<double>())
                this->_created = v.second.get<double>();
        }
        if (v.first == "_modified" || v.first == "modifiedAt") {
            if (v.second.is<double>())
                this->_modified = v.second.get<double>();
        }
        this->_values.insert(v);
        ++itr;
    }
}

void kiicloud::CKiiObject::replaceValues(const picojson::object &values)
{
    picojson::object::const_iterator itr = values.begin();
    while (itr != values.end()) {
        auto v = (*itr);
        if (v.first == "_id" || v.first == "objectID") {
            if (v.second.is<std::string>())
                this->_id = v.second.get<std::string>();
        }
        if (v.first == "_version") {
            if (v.second.is<std::string>())
                this->_version = v.second.get<std::string>();
        }
        if (v.first == "_owner") {
            if (v.second.is<std::string>())
                this->_ownerUserId = v.second.get<std::string>();
        }
        if (v.first == "_created" || v.first == "createdAt") {
            if (v.second.is<double>())
                this->_created = v.second.get<double>();
        }
        if (v.first == "_modified" || v.first == "modifiedAt") {
            if (v.second.is<double>())
                this->_modified = v.second.get<double>();
        }
        ++itr;
    }
    this->_values = values;
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
                             [=](picojson::value objValue, std::string& etag, CKiiError* error)
                             {
                                 CKiiObject *obj = nullptr;
                                 if (error == nullptr) {
                                     obj = new CKiiObject(scopeUri, bucketName, objValue.get<picojson::object>());
                                     obj->_version = etag;
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
                           [=, &targetObject](picojson::value vals, CKiiError *error) {
                               ErrorPtr ret = ErrorPtr(error);
                               if (error == nullptr) {
                                   // Add values returned from server.
                                   picojson::object obj = vals.get<picojson::object>();
                                   targetObject.updateValues(obj);
                               }

                               prm->set_value(ret);
                               delete prm;
                           });
    });
    th.detach();
    return prm->get_future();
}

ErrorFuture kiicloud::CKiiObject::replaceObjectValuesWithNewValues(const CKiiApp &app,
                                                    CKiiObject &targetObject,
                                                    const picojson::object &newValues,
                                                    const std::string &accessToken,
                                                    bool forceUpdate)
{
    auto *prm = new std::promise<ErrorPtr>();
    std::thread th = std::thread([=, &targetObject] {
        _bind->replaceObjectValuesWithNewValues(app,
                                                targetObject.getUri(),
                                                targetObject.getVersion(),
                                                newValues, accessToken,
                                                forceUpdate,
                                                [=, &targetObject](picojson::value vals, std::string &etag, CKiiError *error) {
                                                    ErrorPtr ret = ErrorPtr(error);
                                                    if (error == nullptr) {
                                                        // REPLACE values with returned from server.
                                                        picojson::object obj = vals.get<picojson::object>();
                                                        targetObject.replaceValues(vals.get<picojson::object>());
                                                        targetObject._version = etag;
                                                    }
                                                    prm->set_value(ret);
                                                    delete prm;
        });
    });
    th.detach();
    return prm->get_future();
}

ErrorFuture kiicloud::CKiiObject::refreshObject(const CKiiApp &app,
                                 CKiiObject &targetObject,
                                 const std::string& accessToken)
{
    auto *prm = new std::promise<ErrorPtr>();
    std::thread th = std::thread([=, &targetObject] {
        _bind->refreshObject(app,
                             targetObject.getUri(),
                             accessToken,
                             [=, &targetObject]
                             (picojson::value vals, CKiiError* error) {
                                 ErrorPtr ret = ErrorPtr(error);
                                 picojson::object obj = vals.get<picojson::object>();
                                 targetObject.replaceValues(obj);
                                 prm->set_value(ret);
                                 delete prm;
                             });
    });
    th.detach();
    return prm->get_future();
}
