//
//  CKiiBucket2.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/22.
//
//

#ifndef __cocos2dx_cpp_IFV3__CKiiBucket__
#define __cocos2dx_cpp_IFV3__CKiiBucket__

#include <iostream>
#include "KBase.h"
#include "CKiiObject.h"
#include "CKiiError.h"
#include "picojson.h"
#include "CKiiQuery.h"

namespace kiicloud
{
class CKiiBucket
{
public:
    void saveObject(
                    const picojson::object& values,
                    const std::function<void (CKiiObject *savedObject, CKiiError *error)> saveCallback);
    void query(
               const CKiiQuery& query,
               const::std::function<void (std::vector<CKiiObject>* results, CKiiQuery* nextQuery, CKiiError *error)> queryCallback);

};
}
#endif /* defined(__cocos2dx_cpp_IFV3__CKiiBucket__) */
