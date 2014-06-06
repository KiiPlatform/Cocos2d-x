//
//  CKiiBucket.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/06.
//
//

#include "CKiiBucket.h"
#include "_CKiiGlobal.h"

void kiicloud::CKiiBucket::query(
                                 const CKiiApp &app,
                                 const std::string &scopeUri,
                                 const std::string &bucketName,
                                 const CKiiQuery &query,
                                 const std::string accessToken,
                                 const std::function<void (std::vector<kiicloud::CKiiObject> results,
                                                           NextQueryHandler *nextHandler,
                                                           CKiiError *error)> queryCallback)
{

}