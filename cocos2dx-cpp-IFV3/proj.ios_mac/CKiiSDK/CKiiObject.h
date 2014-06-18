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

//! namespace for kiicloud sdk.
namespace kiicloud {
class CKiiObject;
typedef std::shared_ptr<kiicloud::CKiiObject> ObjPtr;
typedef std::future<std::pair<ObjPtr, ErrorPtr>> ObjFuture;

class CKiiObject
{
public:
    explicit CKiiObject(const std::string& scopeUri, const std::string& bucketName, const picojson::object &values);
    CKiiObject(const CKiiObject& lv);
    CKiiObject(CKiiObject&& lv);

    std::string getId() const;
    std::string getOwnerUserId() const;
    std::string getVersion() const;
    std::string getUri() const;

    long long getModified() const;
    long long getCreated() const;
    picojson::object getValues() const;

    static ObjFuture saveNewObject(
                                   const CKiiApp &app,
                                   const std::string &scopeUri,
                                   const std::string &bucketName,
                                   const picojson::object &values,
                                   const std::string &accessToken);

    //! Apply patch to the existing object.
    
    //! @param app represents application in KiiCloud
    //! @param targetObject patch will be applied
    //! @param patch to apply
    /*! @param accessToken used for authentication.
     * Required if object ACL doesn't allows anonymous user to update object.
     */
    /*! @param forceUpdate if false, patch would be failed if the object has been updated on server after obtained or refreshed targetObject instance.
        if true, apply patch regardless of updates on server. default is true. */
    static ErrorFuture patchObject(const CKiiApp &app,
                                   CKiiObject &targetObject,
                                   const picojson::object &patch,
                                   const std::string &accessToken,
                                   bool forceUpdate = true);

private:
    void updateValues(const picojson::object &values);
    picojson::object _values;
    std::string _id;
    std::string _scopeUri;
    std::string _bucketName;
    std::string _ownerUserId;
    long long _modified;
    long long _created;
    std::string _version;
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiObject__) */
