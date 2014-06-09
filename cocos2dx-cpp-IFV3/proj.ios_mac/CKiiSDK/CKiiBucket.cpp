//
//  CKiiBucket.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#include "CKiiBucket.h"
#include "_CKiiGlobal.h"
#include <thread>

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
    std::thread * th1 = new std::thread();
    std::thread thd([=, &queryCallback, &th1]() {
        _bind->queryBucket(this->app,
                           this->scopeUri,
                           this->bucketName,
                           this->query,
                           this->accessToken,
                           [=, &queryCallback, &th1] (picojson::value jresult, CKiiError* error)
        {
            // TODO: implement.
        });
    });
    th1->swap(thd);
    // TODO: implement it.
}

bool kiicloud::QueryHandler::hasNext()
{
    // TODO: implement it.
    return false;
}