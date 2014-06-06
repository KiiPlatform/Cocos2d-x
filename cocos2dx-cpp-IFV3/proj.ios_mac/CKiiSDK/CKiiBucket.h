//
//  CKiiBucket.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiBucket__
#define __cocos2dx_cpp_IFV3__CKiiBucket__

#include <iostream>
#include <vector>
#include "CKiiObject.h"
#include "CKiiError.h"
#include "CKiiQuery.h"
#include "CKiiApp.h"

namespace kiicloud{

class QueryHandler {
public:
    explicit QueryHandler(
                          const CKiiApp &app,
                          const std::string &scopeUri,
                          const std::string &bucketName,
                          const CKiiQuery &query,
                          const std::string accessToken);
    void nextPage(const std::function<void (std::vector<CKiiObject> results,
                                            CKiiError *error)> queryCallback);
    bool hasNext();

private:
    CKiiApp app;
    std::string scopeUri;
    std::string bucketName;
    CKiiQuery query;
    std::string accessToken;
    bool _hasNext;
};

class CKiiBucket
{
public:
    static QueryHandler* query(
                               const CKiiApp &app,
                               const std::string &scopeUri,
                               const std::string &bucketName,
                               const CKiiQuery &query,
                               const std::string accessToken);
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiBucket__) */
