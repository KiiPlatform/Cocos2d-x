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

kiicloud::CKiiObject::CKiiObject(const CKiiObject& rhs)
:_values(rhs._values),
_id(rhs._id),
_ownerUserId(rhs._ownerUserId),
_modified(rhs._modified),
_created(rhs._created),
_version(rhs._version),
_scopeUri(rhs._scopeUri),
_bucketName(rhs._bucketName)
{
}

kiicloud::CKiiObject::CKiiObject(CKiiObject&& rhs)
:_values(rhs._values),
_id(rhs._id),
_ownerUserId(rhs._ownerUserId),
_modified(rhs._modified),
_created(rhs._created),
_version(rhs._version),
_scopeUri(rhs._scopeUri),
_bucketName(rhs._bucketName)
{
    rhs._values = picojson::object();
    rhs._id = "";
    rhs._ownerUserId = "";
    rhs._modified = 0;
    rhs._created = 0;
    rhs._version = "";
    rhs._scopeUri = "";
    rhs._bucketName = "";
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

ObjFuture kiicloud::CKiiObject::saveNewObjectWithID(
                                                    const CKiiApp &app,
                                                    const std::string &scopeUri,
                                                    const std::string &bucketName,
                                                    const std::string &objectID,
                                                    const picojson::object &values,
                                                    const std::string &accessToken,
                                                    SaveMode saveMode)
{
    auto *p = new std::promise<std::pair<ObjPtr, ErrorPtr>>;
    std::thread th = std::thread([=]() {
        _bind->saveNewObjectWithID(app, scopeUri, bucketName, objectID, values, accessToken, saveMode,
                             [=](picojson::value objValue, std::string& etag, CKiiError* error)
                             {
                                 CKiiObject *obj = nullptr;
                                 if (error == nullptr) {
                                     obj = new CKiiObject(scopeUri, bucketName, objValue.get<picojson::object>());
                                     obj->_id = objectID;
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
                                 if (error == nullptr) {
                                     picojson::object obj = vals.get<picojson::object>();
                                     targetObject.replaceValues(obj);
                                 }
                                 prm->set_value(ret);
                                 delete prm;
                             });
    });
    th.detach();
    return prm->get_future();
}

ErrorFuture kiicloud::CKiiObject::
deleteObject(const kiicloud::CKiiApp &app,
             kiicloud::CKiiObject &targetObject,
             const std::string &accessToken)
{
    auto *prm = new std::promise<ErrorPtr>();
    std::thread th = std::thread([=, &targetObject] {
        _bind->deleteObject(app,
                             targetObject.getUri(),
                             accessToken,
                             [=, &targetObject]
                             (CKiiError* error) {
                                 ErrorPtr ret = ErrorPtr(error);
                                 prm->set_value(ret);
                                 delete prm;
                             });
    });
    th.detach();
    return prm->get_future();
}