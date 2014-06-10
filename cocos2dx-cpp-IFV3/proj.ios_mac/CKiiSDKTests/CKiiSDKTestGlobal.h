//
//  CKiiSDKTestGlobal.h
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/06/10.
//
//

#ifndef cocos2dx_cpp_IFV3_CKiiSDKTestGlobal_h
#define cocos2dx_cpp_IFV3_CKiiSDKTestGlobal_h

#include "CKiiApp.h"

using kiicloud::CKiiSite;
using kiicloud::CKiiApp;
using kiicloud::cKiiSiteJP;

const static std::string appId = std::string("551d82a9");
const static std::string appKey = std::string("675bb7fbe71d562c5278ed999e61a800");
const static CKiiSite appSite = cKiiSiteJP;
static CKiiApp app = CKiiApp(appId, appKey, appSite);

#endif
