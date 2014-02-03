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
//#define CLASS_NAME "org/cocos2dx/simplegame/CallFromCPP"
#define CLASS_NAME "com/hoge/myapp/CallFromCPP"

#else
//iPhone
#include "CallFromCPP.h"
//#include "KRanking.h"

#endif


std::vector<ScoreData> vScore;  //菫晏ｭ倥＠縺ｦ繧九□縺代�∵悴菴ｿ逕ｨ
char kii_label_buff[1024];  //縺薙�譁�ｭ怜�繧団ocos2d-x縺ｧ陦ｨ遉ｺ縺吶ｋ
char kii_display_name[256];

//Android/iPhone蜈ｱ騾壹Ν繝ｼ繝√Φ
void rankingResponseCPP(const char *json){
    CCLOG("rankingResponseCPP");
    
    CCLOG("json = %s",json);
    //json
    std::string err;
    
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::array& a1 = v.get<picojson::array>();
    
    int index = 0;
    strcpy(kii_label_buff, "");	//ﾃｨﾃ｢ﾃ､ﾋ凖｢ﾂｪ
    char buff[256];
    vScore.clear();	//ﾃ丑ﾃ嘉､ﾃ陰
    for (picojson::array::iterator it = a1.begin(); it != a1.end(); it++) {
        picojson::object& o1 = it->get<picojson::object>();
        std::string& s1 = o1["name"].get<std::string>();
        std::string& s2 = o1["score"].get<std::string>();
        //CCLOG("s1 = %s",s1.c_str() );
        //CCLOG("s2 = %s",s2.c_str() );
        
        vScore.push_back(ScoreData(s1,s2));
        sprintf(buff,"%s : %s \n", s1.c_str(), s2.c_str() );
        strcat(kii_label_buff, buff);//ﾃｭﾂｫﾃ｢ﾂ｡
        index++;
        if(index>=7){
            break;
        }
    }
    CCLOG("kii_label_buff %s",kii_label_buff);	//蜃ｺ譚･荳翫′縺｣縺溯｡ｨ遉ｺ譁�ｭ怜�
    
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
extern "C"
{
	//JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_CallCPP_nativeEnd
	JNIEXPORT void JNICALL Java_com_hoge_myapp_CallCPP_nativeEnd

	(JNIEnv* env, jobject thiz)
	{
	    CCLOG("Java_org_cocos2dx_simplegame_CallCPP_nativeEnd");
		CCDirector::sharedDirector()->end();
	}
    
	//JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_CallCPP_rankingResponse
	JNIEXPORT void JNICALL Java_com_hoge_myapp_CallCPP_rankingResponse
	(JNIEnv *env, jobject obj, jstring str1) {
	    CCLOG("Java_org_cocos2dx_simplegame_CallCPP_rankingResponse");
        
		const char *json = env->GetStringUTFChars(str1, 0);
        rankingResponseCPP(json);
	}

	//JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_CallCPP_setDisplayame
	JNIEXPORT void JNICALL Java_com_hoge_myapp_CallCPP_setDisplayame
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

/***
javap繧貞ｮ溯｡� CallFromCPP.class縺悟ｿ�ｦ�

Guests-MacBook-Air:tmp guest$ javap -s CallFromCPP
Compiled from "CallFromCPP.java"
public class org.cocos2dx.simplegame.CallFromCPP extends java.lang.Object{
static final java.lang.String TAG;
  Signature: Ljava/lang/String;
public org.cocos2dx.simplegame.CallFromCPP();
  Signature: ()V
public static void test();
  Signature: ()V
public static void ranking_query_all();
  Signature: ()V
public static void ranking_post(java.lang.String, int);
  Signature: (Ljava/lang/String;I)V
}
***/

#else
//iPhone
//call objective-c
void jni_test(){
    CCLOG("Jni_test iPhone");
}

//extern void ranking_query_all2();

void jni_ranking_query_all(){
    CCLOG("Jni_ranking_query_all iPhone");
    //ranking_query_all2();
    //KRanking::ranking_query_all();
    CallFromCpp::ranking_query_all();
    
    //KRanking::ranking_query(123456+1);  //for test
}


void jni_ranking_post(const char *name, int score){
    CCLOG("jni_ranking_post iPhone");
    //ranking_query_all2();
    //KRanking::ranking_query_all();
    CallFromCpp::ranking_post(name, score);
    
    //KRanking::ranking_query(123456+1);  //for test
}

void jni_save_display_name(const char *name){
    CallFromCpp::save_display_name(name);
}

#endif
