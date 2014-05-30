//
//  CKiiLog.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/27.
//
//

#include "CKiiLog.h"
#include "CKiiiOSLogger.h"

using kiicloud::CKiiLog;
using kiicloud::CKiiLogger;

CKiiLog::CKiiLog()
{
    logger = new CKiiiOSLogger();
};

void CKiiLog::log(const std::string& message)
{
    logger->log(message);
}

CKiiLog::~CKiiLog()
{
    if (logger) {
        delete logger;
        logger = nullptr;
    }
};

std::shared_ptr<CKiiLog> kiicloud::CKiiLog::factoryPtr;

std::shared_ptr<CKiiLog> CKiiLog::getInstance()
{
    std::shared_ptr<CKiiLog> fPtr = CKiiLog::factoryPtr;
    if (fPtr.get() == nullptr) {
        fPtr = std::shared_ptr<CKiiLog>(new CKiiLog());
    }
    return fPtr;
};
