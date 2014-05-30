//
//  CKiiiOSLogger.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/27.
//
//

#include "CKiiiOSLogger.h"
#include <cstdarg>

void kiicloud::CKiiiOSLogger::log(const std::string& message)
{
    NSString *str = [NSString stringWithCString:message.c_str() encoding:NSUTF8StringEncoding];
    NSLog(@"CKiiLogger: %@", str);
}

kiicloud::CKiiiOSLogger::CKiiiOSLogger()
{
    
}

kiicloud::CKiiiOSLogger::~CKiiiOSLogger()
{
    
}