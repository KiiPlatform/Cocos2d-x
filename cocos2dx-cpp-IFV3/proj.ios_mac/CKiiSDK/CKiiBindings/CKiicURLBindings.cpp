//
//  CKiicURLBindings.cpp
//  cocos2dx-cpp-IFV3
//
//  Created by 熊野 聡 on 2014/05/27.
//
//

#include "CKiicURLBindings.h"
#include "curl.h"
#include "picojson.h"
#include "CKiiLogger.h"
#include "CKiiLog.h"

using kiicloud::CKiiSite;
using kiicloud::CKiiUser;
using kiicloud::CKiiError;

static std::string getBaseUrl(const CKiiSite& appSite)
{
    switch(appSite)
    {
        case kiicloud::cKiiSiteCN:
            return "https://api-cn2.kii.com/api";
        case kiicloud::cKiiSiteJP:
            return "https://api-jp.kii.com/api";
        case kiicloud::cKiiSiteUS:
            return "https://api.kii.com/api";
        case kiicloud::cKiiSiteSG:
            return "https://api-sg.kii.com/api";
    }
}

static size_t callbackWrite(char *ptr, size_t size, size_t nmemb, std::string *stream)
{
    int dataLen = size * nmemb;
    stream->append(ptr, dataLen);
    return dataLen;
}

static size_t callbackWriteHeaders(char *ptr, size_t size, size_t nmemb, std::string *stream)
{
    int dataLen = size * nmemb;
    stream->append(ptr, dataLen);
    return dataLen;
}

kiicloud::CKiicURLBindings::CKiicURLBindings()
{
}

kiicloud::CKiicURLBindings::CKiicURLBindings(const CKiicURLBindings& lv)
{
}

kiicloud::CKiicURLBindings::CKiicURLBindings(CKiicURLBindings&& lv)
{
}

kiicloud::CKiicURLBindings::~CKiicURLBindings()
{
    
};

void kiicloud::CKiicURLBindings::registerNewUser(
                     const std::string& appId,
                     const std::string& appKey,
                     const CKiiSite& appSite,
                     const std::string& username,
                     const std::string& password,
                     const picojson::object& data,
                     const std::function<void (std::shared_ptr<CKiiUser> auth, std::shared_ptr<CKiiError> error)> registerCallback)
{
    // TODO: implement it.
};

void kiicloud::CKiicURLBindings::login(
                                       const std::string& appId,
                                       const std::string& appKey,
                                       const CKiiSite& appSite,
                                       const std::string& username,
                                       const std::string& password,
                                       const picojson::object& data,
                                       const std::function<void (std::shared_ptr<CKiiUser> auth, std::shared_ptr<CKiiError> error)> loginCallback)
{
    std::string destUrl = getBaseUrl(appSite) + "/oauth2/token";
    
    CURL *curl;
    CURLcode res;
    std::string resp;
    std::string recvHeaders;
    
    struct curl_slist *headers = NULL;
    
    headers = curl_slist_append(headers, ("x-kii-appid: " + appId).c_str());
    headers = curl_slist_append(headers, ("x-kii-appkey: " + appKey).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/vnd.kii.OauthTokenRequest+json");
    
    picojson::object reqMap;
    reqMap["username"] = picojson::value(username);
    reqMap["password"] = picojson::value(password);

    picojson::value reqObj(reqMap);
    std::string reqStr = reqObj.serialize();
    std::shared_ptr<CKiiLog> ptr = CKiiLog::getInstance();
    CKiiLog::getInstance().get()->log("reqStr: " + reqStr);
    
    for (int i=0; i < 1; ++i) { // dummy loop
        std::shared_ptr<CKiiUser> uPtr;
        std::shared_ptr<CKiiError> ePtr;
        curl = curl_easy_init();
        if (curl)
        {
            
            curl_easy_setopt(curl, CURLOPT_URL, destUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, reqStr.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackWrite);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resp);
            
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, callbackWriteHeaders);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &recvHeaders);
            
            res = curl_easy_perform(curl);
            CKiiLog::getInstance()->log("response: " + resp);
            
            if (res != CURLE_OK) {
                // Connection error.
                ePtr = std::shared_ptr<CKiiError>(new CKiiError(0, "CONNECTION_ERROR"));
                CKiiLog::getInstance()->log("error: " + ePtr.get()->toString());
                loginCallback(uPtr, ePtr);
                break;
            }
            
            // Check response code;
            long respCode = 0;
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &respCode);
            
            if (res != CURLE_OK) {
                ePtr = std::shared_ptr<CKiiError>(new CKiiError(0, "CONNECTION_ERROR"));
                CKiiLog::getInstance()->log("error: " + ePtr.get()->toString());
                loginCallback(uPtr, ePtr);
                break;
            }
            
            if ((200 <= respCode) && (respCode < 300)) {
                uPtr = std::shared_ptr<CKiiUser>(new CKiiUser());
                loginCallback(uPtr, ePtr);
                break;
            }
            
            picojson::value out;
            std::string parseError = "";
            picojson::parse(out, resp.c_str(), resp.c_str() + resp.length(), &parseError);
            if (parseError != "")
            {
                ePtr = std::shared_ptr<CKiiError>(new CKiiError(respCode, "UNEXPECTED_ERROR"));
                CKiiLog::getInstance()->log("error: " + ePtr.get()->toString());
                loginCallback(uPtr, ePtr);
                break;
            }
            
            if (out.contains("errorCode"))
            {
                picojson::value code = out.get("errorCode");
                if (code.is<std::string>())
                {
                    std::string sCode = code.get<std::string>();
                    CKiiLog::getInstance()->log("sCode: " + sCode);
                    ePtr = std::shared_ptr<CKiiError>(new CKiiError(respCode, sCode));
                    CKiiLog::getInstance()->log("error: " + ePtr.get()->toString());
                    loginCallback(uPtr, ePtr);
                    break;
                }
            }
            ePtr = std::shared_ptr<CKiiError>(new CKiiError(respCode, "UNEXPECTED_ERROR"));
            CKiiLog::getInstance()->log("error: " + ePtr.get()->toString());
            loginCallback(uPtr, ePtr);
            break;
        } else {
            ePtr = std::shared_ptr<CKiiError>(new CKiiError(0, "CANT_INITIATE_CONNECTION"));
            CKiiLog::getInstance()->log("error: " + ePtr.get()->toString());
            loginCallback(uPtr, ePtr);
            break;
        }
    } // dummy loop.

    curl_slist_free_all(headers);
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
}


