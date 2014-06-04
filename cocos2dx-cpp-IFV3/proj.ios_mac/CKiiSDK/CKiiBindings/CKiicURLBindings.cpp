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

static size_t callbackWriteHeaders(char *ptr, size_t size, size_t nmemb, std::map<std::string, std::string> *stream)
{
    int dataLen = size * nmemb;
    std::string header(ptr, dataLen);

    std::string::size_type idx = header.find(std::string(":"));
    if (idx == std::string::npos)
        return dataLen;

    std::string key = header.substr(0, idx);
    std::string val = header.substr(idx +1, header.length());
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

void kiicloud::CKiicURLBindings::request(const std::string& requestUrl,
                                         const std::map<std::string, std::string>& requestHeaders,
                                         const std::string& requestBody,
                                         std::string** responseBody,
                                         std::map<std::string, std::string>** responseHeaders,
                                         kiicloud::CKiiError** error)
{
    kiicloud::CKiiLog::getInstance()->log("request url: " + requestUrl);
    kiicloud::CKiiLog::getInstance()->log("request body: " + requestBody);

    struct curl_slist *headers = NULL;
    std::map<std::string, std::string>::const_iterator it = requestHeaders.begin();
    for (; it!= requestHeaders.end(); ++it) {
        std::string header = (*it).first + " : " + (*it).second;
        kiicloud::CKiiLog::getInstance()->log("header: " + header);
        headers =curl_slist_append(headers, header.c_str());
    }

    CURL *curl = nullptr;
    CURLcode res;
    std::string respStr;
    std::map<std::string, std::string> recvHeaders;
    for (int i=0; i<1; ++i) {// dummy loop.
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackWrite);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respStr);
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, callbackWriteHeaders);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &recvHeaders);

            res = curl_easy_perform(curl);
            CKiiLog::getInstance()->log("response: " + respStr);

            if (res != CURLE_OK) {
                // Connection error.
                *responseBody = nullptr;
                *responseHeaders = nullptr;
                *error = new CKiiError(0, "CONNECTION_ERROR");
                break;
            }
            *responseBody = new std::string(respStr);
            *responseHeaders = new std::map<std::string, std::string>(recvHeaders);
            
            // Check response code;
            long respCode = 0;
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &respCode);
            
            if (res != CURLE_OK) {
                *error = new CKiiError(0, "CONNECTION_ERROR");
                break;
            }
            
            if ((200 <= respCode) && (respCode < 300)) {
                *error = nullptr;
                break;
            }
            
            picojson::value out;
            std::string parseError = "";
            picojson::parse(out, respStr.c_str(), respStr.c_str() + respStr.length(), &parseError);
            if (parseError != "")
            {
                *error = new CKiiError(0, "UNEXPECTED_ERROR");
                break;
            }
            
            if (out.contains("errorCode"))
            {
                picojson::value code = out.get("errorCode");
                if (code.is<std::string>())
                {
                    std::string sCode = code.get<std::string>();
                    *error = new CKiiError(respCode, sCode);
                    break;
                }
            }
            *error = new CKiiError(respCode, "UNEXPECTED_ERROR");
            break;
        } else { // failed to init curl.
            *responseBody = nullptr;
            *responseHeaders = nullptr;
            *error = new CKiiError(0, "CANT_INITIATE_CONNECTION");
            break;
        }
    }// dummy loop.

    if (*error)
    {
        kiicloud::CKiiLog::getInstance()->log("error: " + (*error)->toString());
    }

    curl_slist_free_all(headers);
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
    return;
}

void kiicloud::CKiicURLBindings::registerNewUser(
                     const std::string& appId,
                     const std::string& appKey,
                     const CKiiSite& appSite,
                     const std::string& username,
                     const std::string& password,
                     const picojson::object& data,
                     const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> registerCallback)
{
    std::string destUrl = getBaseUrl(appSite) + "/apps/" + appId + "/users";
    std::map<std::string, std::string> mheaders;
    mheaders["x-kii-appid"] = appId;
    mheaders["x-kii-appkey"] = appKey;
    mheaders["content-type"] = "application/json";

    picojson::object reqMap;
    reqMap["loginName"] = picojson::value(username);
    reqMap["password"] = picojson::value(password);

    picojson::value reqObj(reqMap);
    std::string reqStr = reqObj.serialize();

    std::string *respBody;
    std::map<std::string, std::string> *respHeaders;
    kiicloud::CKiiError *error;

    this->request(destUrl, mheaders, reqStr, &respBody, &respHeaders, &error);
    std::shared_ptr<std::string> bodyPtr(respBody);
    std::shared_ptr<std::map<std::string, std::string>> headerPtr(respHeaders);
    if (error) {
        registerCallback(nullptr, error);
        return;
    }
    
    picojson::value v;
    std::string err;
    const char* cRespBody = respBody->c_str();
    picojson::parse(v, cRespBody, cRespBody + strlen(cRespBody), &err);
    if (!err.empty()) {
        error = new CKiiError(0, err);
        registerCallback(nullptr, error);
        return;
    }
    registerCallback(new kiicloud::CKiiUser(v), error);
    return;
};

void kiicloud::CKiicURLBindings::login(
                                       const std::string& appId,
                                       const std::string& appKey,
                                       const CKiiSite& appSite,
                                       const std::string& username,
                                       const std::string& password,
                                       const picojson::object& data,
                                       const std::function<void (CKiiUser *auth, CKiiError *error)> loginCallback)
{
    std::string destUrl = getBaseUrl(appSite) + "/oauth2/token";

    std::map<std::string, std::string> mheaders;
    mheaders["x-kii-appid"] = appId;
    mheaders["x-kii-appkey"] = appKey;
    mheaders["content-type"] = "application/vnd.kii.OauthTokenRequest+json";

    picojson::object reqMap;
    reqMap["username"] = picojson::value(username);
    reqMap["password"] = picojson::value(password);

    picojson::value reqObj(reqMap);
    std::string reqStr = reqObj.serialize();

    std::string *respBody;
    std::map<std::string, std::string> *respHeaders;
    kiicloud::CKiiError *error;

    this->request(destUrl, mheaders, reqStr, &respBody, &respHeaders, &error);
    std::shared_ptr<std::string> bodyPtr(respBody);
    std::shared_ptr<std::map<std::string, std::string>> headerPtr(respHeaders);
    if (error) {
        loginCallback(nullptr, error);
        return;
    }
    picojson::value v;
    std::string err;
    const char* cRespBody = respBody->c_str();
    picojson::parse(v, cRespBody, cRespBody + strlen(cRespBody), &err);
    if (!err.empty()) {
        error = new CKiiError(0, err);
        loginCallback(nullptr, error);
        return;
    }
    loginCallback(new kiicloud::CKiiUser(v), error);
}


