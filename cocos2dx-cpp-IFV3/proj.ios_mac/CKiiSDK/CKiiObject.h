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
    //! For internal use.
    explicit CKiiObject(const std::string& scopeUri, const std::string& bucketName, const picojson::object &values);

    //! Copy constructor
    CKiiObject(const CKiiObject& rhs);
    
    //! Move constructor
    CKiiObject(CKiiObject&& rhs);

    //! Create object instance from scope uri, bucktname and object id.
    //! After the instantiation, it doesn't have any info other than uri of the object.
    //! Make sure to call CKiiObject::refreshObject before accessing key-values in the object.
    explicit CKiiObject(const std::string& scopeUri, const std::string& bucketName, const std::string objectID);


    //! Create object instance from object uri.
    //! Object uri can be obtained from getUri().
    //! After the instantiation, it doesn't have any info other than uri of the object.
    //! Make sure to call CKiiObject::refreshObject before accessing key-values in the object.
    explicit CKiiObject(const std::string& objectUri);

    //! Returns ID of the object.
    std::string getId() const;
    
    //! Returns user ID of the object.
    std::string getOwnerUserId() const;

    //! Returns version string of the object.
    std::string getVersion() const;

    //! returns Uri of the object.
    std::string getUri() const;

    //! returns last modified time.
    //! milliseconds from epoch
    long long getModified() const;

    //! returns created time.
    //! milliseconds from epoch
    long long getCreated() const;

    //! get key-values of object.
    //! modifying this object won't be synced with Kii Cloud.
    //! to update object, user CKiiObject::patchObject or
    //! CKiiObject::replaceObjectValuesWithNewValues
    //! to retrieve latest key-values from Kii Cloud,
    //! use CKiiObject::refreshObject.
    picojson::object getValues() const;

    //! Save new object in Kii Cloud
    //! Unique Object ID is assigned by server.
    
    //! @param app represents application in KiiCloud
    //! @param scopeUri Uri of the scope in which bucket/ object will be created.
    //! It must be Uri of whose Application, Group or User.
    //! @param bucketName specify bucket name where object should be saved.
    //! @param values object values.
    /*! @param accessToken used for authentication.
     * Required if bucket ACL doesn't allows anonymous user to create object.
     */
    static ObjFuture saveNewObject(
                                   const CKiiApp &app,
                                   const std::string &scopeUri,
                                   const std::string &bucketName,
                                   const picojson::object &values,
                                   const std::string &accessToken);

    //! Used to determine the behavior of saveNewObjectWithID() when there is object which
    //! has same ID exists in cloud.
    enum SaveMode {
        FAIL_IF_EXIST,
        REPLACE_IF_EXIST
    };

    //! Save new object in Kii Cloud
    //! Unique Object ID is should be assinged in client side.
    
    //! @param app represents application in KiiCloud
    //! @param scopeUri Uri of the scope in which bucket/ object will be created.
    //! It must be Uri of whose Application, Group or User.
    //! @param bucketName specify bucket name where object should be saved.
    //! @param objectID unique object ID.
    //! @param values object values.
    /*! @param accessToken used for authentication.
     * Required if bucket ACL doesn't allows anonymous user to create object.
     */
    //! @param saveMode define behavior when the object
    //! which has specified ID is already exists in Kii Cloud.
    //! If the mode is is REPLACE_IF_EXIST, existing object will be overwritten with specified values.
    //! existing object values will be replaced with specified values.
    //! (i.e.) key-value pair which is not in the specified values will be removed.
    //! If the mode is FAILE_IF_EXIST, save Object will be failed if there is object which has same ID.
    //! Uniqueness of object ID is guranteed in the same scope uri and same bucket bucket name.
    static ObjFuture saveNewObjectWithID(
                                         const CKiiApp &app,
                                         const std::string &scopeUri,
                                         const std::string &bucketName,
                                         const std::string &objectID,
                                         const picojson::object &values,
                                         const std::string &accessToken,
                                         SaveMode saveMode = SaveMode::FAIL_IF_EXIST);

    //! Apply patch to the existing object.
    
    //! @param app represents application in KiiCloud
    //! @param targetObject patch will be applied
    //! @param patch to apply
    /*! @param accessToken used for authentication.
     * Required if object ACL doesn't allows anonymous user to update object.
     */
    /*! @param forceUpdate if false, patch would be failed if the object has been updated
        on server after obtained or refreshed targetObject instance.
        if true, apply patch regardless of updates on server. */
    static ErrorFuture patchObject(const CKiiApp &app,
                                   CKiiObject &targetObject,
                                   const picojson::object &patch,
                                   const std::string &accessToken,
                                   bool forceUpdate = true);

    //! Update object with specified data.
    //! Exiting data in the object will be replaced with the specified data.
    //! key-values not exists in specified data would be removed.

    //! @param app represents application in KiiCloud
    //! @param targetObject would be replaced.
    //! @param newValues replacement data.
    /*! @param accessToken used for authentication.
     * Required if object ACL doesn't allows anonymous user to update object.
     */
    /*! @param forceUpdate if false, replace would be failed if the object has been updated
        on server after obtained or refreshed targetObject instance.
        if true, replace object with new data regardless of updates on server. */
    static ErrorFuture replaceObjectValuesWithNewValues(const CKiiApp &app,
                                   CKiiObject &targetObject,
                                   const picojson::object &newValues,
                                   const std::string &accessToken,
                                   bool forceUpdate = true);

    //! Refresh object.
    //! It retrieves the latest key-values of target object from Kii Cloud.

    //! @param app represents application in KiiCloud
    //! @param targetObject would be refreshed.
    /*! @param accessToken used for authentication.
     * Required if object ACL doesn't allows anonymous user to read object. */
    static ErrorFuture refreshObject(const CKiiApp &app,
                                     CKiiObject &targetObject,
                                     const std::string& accessToken);

    
    //! Delete object.
    
    //! @param app represents application in KiiCloud
    //! @param targetObject would be deleted.
    /*! @param accessToken used for authentication.
     * Required if object ACL doesn't allows anonymous user to write object. */
    static ErrorFuture deleteObject(const CKiiApp &app,
                                     CKiiObject &targetObject,
                                     const std::string& accessToken);

private:
    void updateValues(const picojson::object &values);
    void replaceValues(const picojson::object &values);
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
