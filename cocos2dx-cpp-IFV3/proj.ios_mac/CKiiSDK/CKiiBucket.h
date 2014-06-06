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

class NextQueryHandler {
public:
    void nextPage(const std::function<void (std::vector<kiicloud::CKiiObject> results, CKiiError *error)> queryCallback);

    bool hasNext();
};

class CKiiBucket
{
public:
    static void query(
                      const CKiiApp &app,
                      const std::string &scopeUri,
                      const std::string &bucketName,
                      const CKiiQuery &query,
                      const std::string accessToken,
                      const std::function<void (std::vector<kiicloud::CKiiObject> results,
                                                NextQueryHandler* nextHandler,
                                                CKiiError *error)> queryCallback);
};

}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiBucket__) */
