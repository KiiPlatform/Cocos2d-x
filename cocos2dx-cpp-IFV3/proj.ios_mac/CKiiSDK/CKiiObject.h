//
//  CKiiObject.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiObject__
#define __cocos2dx_cpp_IFV3__CKiiObject__

#include <iostream>
#include <future>
#include "picojson.h"
#include "CKiiError.h"
#include "CKiiApp.h"
using kiicloud::ErrorPtr;

namespace kiicloud {
class CKiiObject;
typedef std::shared_ptr<kiicloud::CKiiObject> ObjPtr;

class CKiiObject
{
public:
    CKiiObject(picojson::object values);
    CKiiObject(const CKiiObject& lv);
    CKiiObject(CKiiObject&& lv);

    std::string getId() const;
    std::string getOwnerUserId() const;
    std::string getVersion() const;
    long long getModified() const;
    long long getCreated() const;
    picojson::object getValues() const;

    static std::future<std::pair<ObjPtr, ErrorPtr>>saveNewObject(
                                                                 const CKiiApp &app,
                                                                 const std::string &scopeUri,
                                                                 const std::string &bucketName,
                                                                 const picojson::object values,
                                                                 const std::string &accessToken);

private:
    picojson::object _values;
    std::string _id;
    std::string _ownerUserId;
    long long _modified;
    long long _created;
    std::string _version;
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiObject__) */
