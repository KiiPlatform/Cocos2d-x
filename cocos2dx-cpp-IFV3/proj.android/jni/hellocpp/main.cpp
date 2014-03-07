#include "AppDelegate.h"
#include "cocos2d.h"
#include "CCEventType.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#include <android/log.h>
#include <android_native_app_glue.h>


#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

void jniTest(struct android_app* state);

JNIEnv* g_env;
JavaVM* g_vm;
jobject g_thiz;
struct android_app* g_state;


void cocos_android_app_init (struct android_app* app) {
    LOGD("cocos_android_app_init");
    AppDelegate *pAppDelegate = new AppDelegate();
    g_state = app;

    LOGD("cocos_android_app_init2");
    //jniTest(app);
    LOGD("cocos_android_app_init3");

    //jniTest(g_state);

    LOGD("cocos_android_app_init4");
}

void callJava(){
    LOGD("callJava %08x %08x",(unsigned int)g_env, (unsigned int)g_thiz );
	jclass test = (g_env)->GetObjectClass(g_thiz);
    LOGD("callJav2");
	jmethodID methodj = (g_env)->GetMethodID(test, "TestPreference", "()V");
    LOGD("callJava3");
}

void jni_kiiReq(const char *json, int serviceID){
	LOGD("jni_kiiReq Android %s %d", json, serviceID);

	//jniTest(g_state);

	LOGD("jni_kiiReq2");

	android_app* state = g_state;
	JNIEnv* env;
	JavaVM* vm = state->activity->vm;
    //JniMethodInfo methodInfo;

	(vm)->AttachCurrentThread(&env, NULL);
	jobject thiz = state->activity->clazz;
	LOGD("jni_kiiReq3");
	jclass test = (env)->GetObjectClass(thiz);
	jmethodID methodj = (env)->GetMethodID(test, "kiiReq", "(Ljava/lang/String;I)V");
	LOGD("jni_kiiReq4");
	jstring stringArg1 = env->NewStringUTF(json);
	LOGD("jni_kiiReq5");
	(env)->CallVoidMethod(thiz, methodj, stringArg1, serviceID);
	LOGD("jni_kiiReq6");
/***
    JniMethodInfo methodInfo;

    if (JniHelper::getStaticMethodInfo(methodInfo
                                       , CLASS_NAME
                                       , "kiiReq"
                                       , "(Ljava/lang/String;I)V"))
    {
    	// java側は「String」にする。UTF8変換
    	jstring stringArg1 = methodInfo.env->NewStringUTF(json);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg1, serviceID);
        methodInfo.env->DeleteLocalRef(stringArg1);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
***/
}

void jniTest(struct android_app* state)
{
    LOGD("jniTest");
	JNIEnv* env;
	JavaVM* vm = state->activity->vm;

	g_env = env;
	g_vm = vm;
	//g_thiz = thiz;

	(vm)->AttachCurrentThread(&env, NULL);
	jobject thiz = state->activity->clazz;
	g_thiz = thiz;
    //LOGD("jniTes2 %08x %08x",(unsigned int)g_env, (unsigned int)g_thiz );

    LOGD("jniTes2");
	jstring stringArg1 = env->NewStringUTF("hoge");

	jclass test = (env)->GetObjectClass(thiz);
	jmethodID methodj = (env)->GetMethodID(test, "TestPreference", "()V");

    LOGD("jniTes3");

	(env)->CallVoidMethod(thiz, methodj);
    LOGD("jniTest4");
	//(env)->CallVoidMethod(thiz, methodj);

	//(vm)->DetachCurrentThread();

    LOGD("jniTest5");

    //
    //callJava();
    LOGD("jniTest6");

	/***
	 * //以下追記
    PluginJniHelper::setJavaVM(app->activity->vm);
    PluginJniHelper::setClassLoaderFrom(app->activity->clazz);
    cocos2d::plugin::PluginUtils::initPluginWrapper(app);

	 */
}
