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

kiicloud::CKiiQueryHandler* kiicloud::CKiiBucket::query(
                                 const CKiiApp &app,
                                 const std::string &scopeUri,
                                 const std::string &bucketName,
                                 const CKiiQuery &query,
                                 const std::string accessToken)
{
    CKiiQueryHandler* qh = new CKiiQueryHandler(app, scopeUri, bucketName, query, accessToken);
    return qh;
}

kiicloud::CKiiQueryHandler::CKiiQueryHandler(
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

kiicloud::CKiiQueryHandler::CKiiQueryHandler(const CKiiQueryHandler& lv)
:app (CKiiApp(lv.app.appId, lv.app.appKey, lv.app.appSite)),
scopeUri(lv.scopeUri),
bucketName(lv.bucketName),
query(lv.query),
accessToken(lv.accessToken),
_hasNext(lv._hasNext)
{
}

kiicloud::CKiiQueryHandler::CKiiQueryHandler(CKiiQueryHandler&& lv)
:app (CKiiApp(lv.app.appId, lv.app.appKey, lv.app.appSite)),
scopeUri(lv.scopeUri),
bucketName(lv.bucketName),
query(lv.query),
accessToken(lv.accessToken),
_hasNext(lv._hasNext)
{
    lv.scopeUri = "";
    lv.bucketName = "";
    lv.query = CKiiQuery();
    lv.accessToken = "";
    lv._hasNext = false;
}

void kiicloud::CKiiQueryHandler::nextPage(const std::function<void (std::vector<CKiiObject> results,
                                                           CKiiError *error)> queryCallback)
{
    std::thread * th1 = new std::thread();
    std::thread thd([=]() {
        _bind->queryBucket(this->app,
                           this->scopeUri,
                           this->bucketName,
                           this->query,
                           this->accessToken,
                           [=, &queryCallback] (picojson::value jresult, CKiiError* error)
        {
            std::vector<CKiiObject> res;
            if (error != nullptr) {
                queryCallback(res, error);
                return;
            }
            std::vector<CKiiObject> callbackResult;
            picojson::object resObj = jresult.get<picojson::object>();
            // parse next.
            this->_hasNext = false;
            if (resObj["nextPaginationKey"].is<std::string>()) {
                std::string nPKey = resObj["nextPaginationKey"].get<std::string>();
                if (!nPKey.empty()) {
                    this->_hasNext = true;
                    this->query = CKiiQuery(query, nPKey);
                }
            }

            picojson::array objs = resObj["results"].get<picojson::array>();
            std::vector<picojson::value>::iterator itr = objs.begin();
            while (itr != objs.end())
            {
                picojson::object aObj = (*itr).get<picojson::object>();
                CKiiObject kobj = CKiiObject(aObj);
                callbackResult.push_back(kobj);
                ++itr;
            }
            queryCallback(callbackResult, error);
            th1->detach();
            delete th1;
        });
    });
    th1->swap(thd);
}

bool kiicloud::CKiiQueryHandler::hasNext()
{
    return this->_hasNext;
}
