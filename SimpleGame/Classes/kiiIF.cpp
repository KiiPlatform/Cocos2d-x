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
#define CLASS_NAME "org/cocos2dx/simplegame/CallFromCPP"

#else
//iPhone
#include "CallFromCPP.h"
#include "KRanking.h"

#endif

//

std::vector<ScoreData> vScore;
char kii_label_buff[1024];

void rankingResponse22(const char *json){
    CCLOG("rankingResponse22");
    
    CCLOG("json = %s",json);
    //json
    std::string err;
    
    picojson::value v;
    picojson::parse(v, json, json + strlen(json), &err);
    picojson::array& a1 = v.get<picojson::array>();
    
    int index = 0;
    strcpy(kii_label_buff, "");	//èâä˙âª
    char buff[256];
    vScore.clear();	//ÉNÉäÉA
    for (picojson::array::iterator it = a1.begin(); it != a1.end(); it++) {
        picojson::object& o1 = it->get<picojson::object>();
        std::string& s1 = o1["name"].get<std::string>();
        std::string& s2 = o1["score"].get<std::string>();
        //CCLOG("s1 = %s",s1.c_str() );
        //CCLOG("s2 = %s",s2.c_str() );
        
        vScore.push_back(ScoreData(s1,s2));
        sprintf(buff,"%s : %s \n", s1.c_str(), s2.c_str() );
        strcat(kii_label_buff, buff);//í«â¡
        index++;
        if(index>=7){
            break;
        }
    }
    CCLOG("kii_label_buff %s",kii_label_buff);	//出来上がった表示文字列
    
    int i;
    for(i=0; i<vScore.size(); i++){
        //CCLOG("vScore  %d  %s %s", i, HelloWorld::vScore[i].name.c_str(),  HelloWorld::vScore[i].score.c_str());
    }
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//Android
extern "C"
{
	JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_CallCPP_nativeEnd
	(JNIEnv* env, jobject thiz)
	{
	    CCLOG("SimpleGame_nativeEnd");
		//CCDirector::sharedDirector()->end();
	}
    
	JNIEXPORT void JNICALL Java_org_cocos2dx_simplegame_CallCPP_rankingResponse
	(JNIEnv *env, jobject obj, jstring str1) {
	    CCLOG("Java_org_cocos2dx_simplegame_CallCPP_rankingResponse");
        
		const char *json = env->GetStringUTFChars(str1, 0);
        rankingResponse22(json);
	}
}

#else

//iPhone
void iPhone_rankingResponse(const char *json){
    CCLOG("iPhone_rankingResponse");
    CCLOG("json = %s",json);
    
    rankingResponse22(json);
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
    //CallFromCpp::ranking_query_all();
    
    KRanking::ranking_query(123456+1);  //for test
}




#endif
