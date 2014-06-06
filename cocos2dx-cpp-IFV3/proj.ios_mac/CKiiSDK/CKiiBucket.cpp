//
//  CKiiBucket.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#include "CKiiBucket.h"
#include "_CKiiGlobal.h"

kiicloud::QueryHandler* kiicloud::CKiiBucket::query(
                                 const CKiiApp &app,
                                 const std::string &scopeUri,
                                 const std::string &bucketName,
                                 const CKiiQuery &query,
                                 const std::string accessToken)
{
    // TODO: implement it.
    return nullptr;
}

kiicloud::QueryHandler::QueryHandler(
                       const CKiiApp &app,
                       const std::string &scopeUri,
                       const std::string &bucketName,
                       const CKiiQuery &query,
                       const std::string accessToken)
:app (CKiiApp(app.appId, app.appKey, app.appSite)),
scopeUri (scopeUri),
bucketName(bucketName),
query(query),
accessToken(accessToken),
_hasNext (true)
{
}

void kiicloud::QueryHandler::nextPage(const std::function<void (std::vector<CKiiObject> results,
                                                           CKiiError *error)> queryCallback)
{
    // TODO: implement it.
}

bool kiicloud::QueryHandler::hasNext()
{
    // TODO: implement it.
    return false;
}