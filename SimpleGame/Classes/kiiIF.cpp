//
//  kiiIF.c
//  SimpleGame
//
//  Created by Guest on 2014/01/22.
//
//

#include "KillIF.h"

#include <vector>

using namespace std;
using namespace cocos2d;
#include "picojson.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//Android
#include <jni.h>

#include "platform/android/jni/JniHelper.h"

#define CLASS_NAME "com/kii/sample/cocos2dx/simplegame/CallFromCPP"

#else
//iPhone
#include "CallFromCPP.h"
//#include "KRanking.h"

#endif


std::vector<ScoreData> vScore;
char kii_label_buff[1024];
char kii_display_name[256];

//Android/iPhone共通
void rankingResponseCPP(const char *json){
    CCLOG("rankingResponseCPP");
    
    CCLOG("json = %s",json);
    //json
    std::string err;
    
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::array& a1 = v.get<picojson::array>();
    
    int index = 0;
    strcpy(kii_label_buff, "");
    char buff[256];
    vScore.clear();
    for (picojson::array::iterator it = a1.begin(); it != a1.end(); it++) {
        picojson::object& o1 = it->get<picojson::object>();
        std::string& s1 = o1["name"].get<std::string>();
        std::string& s2 = o1["score"].get<std::string>();
        
        vScore.push_back(ScoreData(s1,s2));
        sprintf(buff,"%s : %s \n", s1.c_str(), s2.c_str() );
        strcat(kii_label_buff, buff);
        index++;
        if(index>=7){
            break;
        }
    }
    CCLOG("kii_label_buff %s",kii_label_buff);
    
    int i;
    for(i=0; i<vScore.size(); i++){
        //CCLOG("vScore  %d  %s %s", i, HelloWorld::vScore[i].name.c_str(),  HelloWorld::vScore[i].score.c_str());
    }
}

void setDisplayameCPP(const char *display_name){
    CCLOG("setDisplayameCPP %s", display_name);
    strcpy( kii_display_name, display_name );
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//Android
////package com.kii.sample.cocos2dx.simplegame;
extern "C"
{
<<<<<<< HEAD
	//JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_CallCPP_nativeEnd
	JNIEXPORT void JNICALL Java_com_kii_sample_cocos2dx_simplegame_CallCPP_nativeEnd

=======
	JNIEXPORT void JNICALL Java_com_kii_sample_cocos2dx_simplegame_CallCPP_nativeEnd
>>>>>>> muku
	(JNIEnv* env, jobject thiz)
	{
	    CCLOG("Java_org_cocos2dx_simplegame_CallCPP_nativeEnd");
		CCDirector::sharedDirector()->end();
	}
    
<<<<<<< HEAD
	//JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_CallCPP_rankingResponse
=======
>>>>>>> muku
	JNIEXPORT void JNICALL Java_com_kii_sample_cocos2dx_simplegame_CallCPP_rankingResponse
	(JNIEnv *env, jobject obj, jstring str1) {
	    CCLOG("Java_org_cocos2dx_simplegame_CallCPP_rankingResponse");
        
		const char *json = env->GetStringUTFChars(str1, 0);
        rankingResponseCPP(json);
	}

<<<<<<< HEAD
	//JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_CallCPP_setDisplayame
=======
>>>>>>> muku
	JNIEXPORT void JNICALL Java_com_kii_sample_cocos2dx_simplegame_CallCPP_setDisplayame
	(JNIEnv *env, jobject obj, jstring str1) {
	    CCLOG("Java_org_cocos2dx_simplegame_CallCPP_setDisplayame");

		const char *display_name = env->GetStringUTFChars(str1, 0);
		setDisplayameCPP(display_name);
	}

}

#else

//iPhone
void iPhone_nativeEnd()
{
    CCLOG("iPhone_nativeEnd");
    CCDirector::sharedDirector()->end();
}

void iPhone_rankingResponse(const char *json){
    CCLOG("iPhone_rankingResponse");
    CCLOG("json = %s",json);
    
    rankingResponseCPP(json);
}

void iPhone_setDisplayame(const char *name){
    CCLOG("iPhone_setDisplayame");
    CCLOG("name = %s",name);
    
    setDisplayameCPP(name);
}

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//Android
//call Java

void jni_test(){
    CCLOG("Jni_test");
    JniMethodInfo methodInfo;
    
    if (JniHelper::getStaticMethodInfo(methodInfo
                                       , CLASS_NAME
                                       , "test"
                                       , "()V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

void jni_ranking_query_all(){
    CCLOG("Jni_ranking_query_all Android");
    JniMethodInfo methodInfo;
    
    if (JniHelper::getStaticMethodInfo(methodInfo
                                       , CLASS_NAME
                                       , "ranking_query_all"
                                       , "()V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

void jni_ranking_post(const char *name, int score){
    CCLOG("jni_ranking_post Android %s %d", name, score);
    JniMethodInfo methodInfo;

    if (JniHelper::getStaticMethodInfo(methodInfo
                                       , CLASS_NAME
                                       , "ranking_post"
                                       , "(Ljava/lang/String;I)V"))
    {
    	// java側は「String」にする。UTF8変換
    	jstring stringArg1 = methodInfo.env->NewStringUTF(name);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg1, score);
        methodInfo.env->DeleteLocalRef(stringArg1);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

void jni_save_display_name(const char *name){
    CCLOG("jni_save_display_name Android %s", name);
    JniMethodInfo methodInfo;

    if (JniHelper::getStaticMethodInfo(methodInfo
                                       , CLASS_NAME
                                       , "save_display_name"
                                       , "(Ljava/lang/String;)V"))
    {
    	// java側は「String」にする。UTF8変換
    	jstring stringArg1 = methodInfo.env->NewStringUTF(name);	//stringArg1
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg1);
        methodInfo.env->DeleteLocalRef(stringArg1);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

#else
//iPhone
//call objective-c
void jni_test(){
    CCLOG("Jni_test iPhone");
}

void jni_ranking_query_all(){
    CCLOG("Jni_ranking_query_all iPhone");
    CallFromCpp::ranking_query_all();
}

void jni_ranking_post(const char *name, int score){
    CCLOG("jni_ranking_post iPhone");
    CallFromCpp::ranking_post(name, score);
}

void jni_save_display_name(const char *name){
    CallFromCpp::save_display_name(name);
}

#endif
