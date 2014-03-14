LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/StartScene.cpp \
                   ../../Classes/RankingScene.cpp \
                   ../../Classes/GameOverScene.cpp \
                   ../../Classes/kii/CKiiClause.cpp \
                   ../../Classes/kii/KBase.cpp \
                   ../../Classes/kii/CKiiBucket.cpp \
                   ../../Classes/kii/CKiiQuery.cpp \
                   ../../Classes/kii_jni/kiiIF.cpp \
                   ../../Classes/kii_jni/kiiCPP.cpp \
                   ../../Classes/test/CKiiApiTest.cpp \
                   ../../Classes/kiiRanking/kiiRanking.cpp \
                   ../../Classes/PName.cpp \
                   ../../Classes/HelloWorldScene.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../cocos2d/

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,extensions)
$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
