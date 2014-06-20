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
#include "CKiiLog.h"
#include "CKiiQuery.h"
#include <regex>

using kiicloud::CKiiSite;
using kiicloud::CKiiUser;
using kiicloud::CKiiError;

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

    header.erase(std::remove(header.begin(), header.end(), '\n'));
    header.erase(std::remove(header.begin(), header.end(), '\r'));

    std::regex reg("^(.*)\\s*:\\s*(.*)");
    std::string key  =  std::regex_replace(header, reg, "$1");
    std::string val  =  std::regex_replace(header, reg, "$2");

    stream->insert(std::pair<std::string, std::string>(key, val));
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

void kiicloud::CKiicURLBindings::request(
                                         const Method& method,
                                         const std::string& requestUrl,
                                         const std::map<std::string, std::string>& requestHeaders,
                                         const std::string& requestBody,
                                         std::string** responseBody,
                                         std::map<std::string, std::string>** responseHeaders,
                                         kiicloud::CKiiError** error)
{
    CKiiLog("request url: " + requestUrl);
    CKiiLog("request body: " + requestBody);

    struct curl_slist *headers = NULL;
    std::map<std::string, std::string>::const_iterator it = requestHeaders.begin();
    for (; it!= requestHeaders.end(); ++it) {
        std::string header = (*it).first + " : " + (*it).second;
        CKiiLog("header: " + header);
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
            if (method == POST) {
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackWrite);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respStr);
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, callbackWriteHeaders);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &recvHeaders);

            res = curl_easy_perform(curl);
            CKiiLog("response: " + respStr);
            CKiiLog("recvHeaders: ");
            auto itr = recvHeaders.begin();
            while (itr != recvHeaders.end()) {
                auto key = (*itr).first;
                auto val = (*itr).second;
                CKiiLog(key + " : " + val);
                ++itr;
            }

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
        CKiiLog("error: " + (*error)->toString());
    }

    curl_slist_free_all(headers);
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
    return;
}

void kiicloud::CKiicURLBindings::registerNewUser(
                                                 const CKiiApp& app,
                     const std::string& username,
                     const std::string& password,
                     const picojson::object& data,
                     const std::function<void (CKiiUser *authenticatedUser, CKiiError *error)> registerCallback)
{
    std::string destUrl = app.appUrl() + "/users";
    std::map<std::string, std::string> mheaders;
    mheaders["x-kii-appid"] = app.appId;
    mheaders["x-kii-appkey"] = app.appKey;
    mheaders["content-type"] = "application/json";

    picojson::object reqMap;
    reqMap["loginName"] = picojson::value(username);
    reqMap["password"] = picojson::value(password);

    picojson::value reqObj(reqMap);
    std::string reqStr = reqObj.serialize();

    std::string *respBody;
    std::map<std::string, std::string> *respHeaders;
    kiicloud::CKiiError *error;

    this->request(POST, destUrl, mheaders, reqStr, &respBody, &respHeaders, &error);
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
                                       const CKiiApp& app,
                                       const std::string& username,
                                       const std::string& password,
                                       const picojson::object& data,
                                       std::function<void (CKiiUser *auth, CKiiError *error)> loginCallback)
{
    std::string destUrl = getBaseUrl(app.appSite) + "/oauth2/token";

    std::map<std::string, std::string> mheaders;
    mheaders["x-kii-appid"] = app.appId;
    mheaders["x-kii-appkey"] = app.appKey;
    mheaders["content-type"] = "application/vnd.kii.OauthTokenRequest+json";

    picojson::object reqMap;
    reqMap["username"] = picojson::value(username);
    reqMap["password"] = picojson::value(password);

    picojson::value reqObj(reqMap);
    std::string reqStr = reqObj.serialize();

    std::string *respBody;
    std::map<std::string, std::string> *respHeaders;
    kiicloud::CKiiError *error;

    this->request(POST, destUrl, mheaders, reqStr, &respBody, &respHeaders, &error);
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

void kiicloud::CKiicURLBindings::refreshUser(const kiicloud::CKiiApp &app,
                                             kiicloud::CKiiUser &user,
                                             std::function<void (picojson::value keyValues, CKiiError *error)> refreshCallback)
{
    picojson::value kvs;
    if (user.getId().empty()) {
        CKiiError * error = new CKiiError(0, "given user does not have ID");
        refreshCallback(kvs, error);
        return;
    }
    std::string destUrl = getBaseUrl(app.appSite) + "/apps/" + app.appId + "/users/" + user.getId();
    std::map<std::string, std::string> mheaders;
    mheaders["x-kii-appid"] = app.appId;
    mheaders["x-kii-appkey"] = app.appKey;
    mheaders["authorization"] = "Bearer " + user.getAccessToken();
    
    std::string *respBody;
    std::map<std::string, std::string> *respHeaders;
    kiicloud::CKiiError *error;
    
    this->request(GET, destUrl, mheaders, "", &respBody, &respHeaders, &error);
    if (error) {
        refreshCallback(kvs, error);
        return;
    }

    std::string err;
    const char* cRespBody = respBody->c_str();
    picojson::parse(kvs, cRespBody, cRespBody + strlen(cRespBody), &err);
    if (!err.empty()) {
        error = new CKiiError(0, err);
    }
    refreshCallback(kvs, error);
}

void kiicloud::CKiicURLBindings::queryBucket(const CKiiApp& app,
                                             const std::string& scopeUri,
                                             const std::string& bucketName,
                                             const CKiiQuery& query,
                                             const std::string& accessToken,
                                             const std::function<void (picojson::value result,
                                                                 CKiiError* error)> queryCallback)
{
    std::string destUrl = scopeUri + "/buckets/" + bucketName + "/query";
    std::map<std::string, std::string> mheaders;
    mheaders["x-kii-appid"] = app.appId;
    mheaders["x-kii-appkey"] = app.appKey;
    mheaders["content-type"] = "application/vnd.kii.QueryRequest+json";
    if (!accessToken.empty())
        mheaders["authorization"] = "Bearer " + accessToken;

    std::string *respBody;
    std::map<std::string, std::string> *respHeaders;
    kiicloud::CKiiError *error;

    picojson::value jresult;
    this->request(POST, destUrl, mheaders, query.toString(), &respBody, &respHeaders, &error);
    if (error) {
        queryCallback(jresult, error);
        return;
    }

    std::string err;
    const char* cRespBody = respBody->c_str();
    picojson::parse(jresult, cRespBody, cRespBody + strlen(cRespBody), &err);
    if (!err.empty()) {
        error = new CKiiError(0, err);
    }
    queryCallback(jresult, error);
}

void kiicloud::CKiicURLBindings::saveNewObject(const CKiiApp& app,
                                               const std::string &scopeUri,
                                               const std::string &bucketName,
                                               const picojson::object values,
                                               const std::string &accessToken,
                                               const std::function<void (picojson::value, std:: string& etag, CKiiError *)> saveCallback)
{
    std::string destUrl = scopeUri + "/buckets/" + bucketName + "/objects";
    std::map<std::string, std::string> mheaders;
    mheaders["x-kii-appid"] = app.appId;
    mheaders["x-kii-appkey"] = app.appKey;
    mheaders["content-type"] = "application/json";
    if (!accessToken.empty())
        mheaders["authorization"] = "Bearer " + accessToken;

    picojson::value reqBodyJson = picojson::value(values);
    std::string *respBody;
    std::map<std::string, std::string> *respHeaders;
    kiicloud::CKiiError *error;
    
    picojson::value jresult;
    std::string etag;
    this->request(POST, destUrl, mheaders, reqBodyJson.serialize(), &respBody, &respHeaders, &error);
    if (error) {
        saveCallback(jresult, etag, error);
        return;
    }
    etag = (*respHeaders)["ETag"];
    
    std::string err;
    const char* cRespBody = respBody->c_str();
    picojson::parse(jresult, cRespBody, cRespBody + strlen(cRespBody), &err);
    if (!err.empty()) {
        error = new CKiiError(0, err);
    }
    saveCallback(jresult, etag, error);
}

void kiicloud::CKiicURLBindings::patchObject(const kiicloud::CKiiApp &app,
                                             const std::string &objUri,
                                             const std::string &objVersion,
                                             const picojson::object values,
                                             const std::string& accessToken,
                                             bool forceUpdate,
                                             const std::function<void (picojson::value value,
                                                                       CKiiError* error)> patchCallback)
{
    std::map<std::string, std::string> mheaders;
    mheaders["x-kii-appid"] = app.appId;
    mheaders["x-kii-appkey"] = app.appKey;
    mheaders["content-type"] = "application/json";
    mheaders["x-http-method-override"] = "PATCH";
    if (!forceUpdate)
        mheaders["if-match"] = objVersion;
    if (!accessToken.empty())
        mheaders["authorization"] = "Bearer " + accessToken;

    picojson::value reqBodyJson = picojson::value(values);
    std::string *respBody;
    std::map<std::string, std::string> *respHeaders;
    kiicloud::CKiiError *error;

    picojson::value jresult;
    this->request(POST, objUri, mheaders, reqBodyJson.serialize(), &respBody, &respHeaders, &error);
    if (error) {
        patchCallback(jresult, error);
        return;
    }

    std::string err;
    const char* cRespBody = respBody->c_str();
    picojson::parse(jresult, cRespBody, cRespBody + strlen(cRespBody), &err);
    if (!err.empty()) {
        patchCallback(jresult, error);
        return;
    }
    patchCallback(jresult, error);
}
