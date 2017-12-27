LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := $(LOCAL_PATH)/monopoly/main.cpp \
                   $(LOCAL_PATH)/../../../Classes/AppDelegate.cpp \
                   $(LOCAL_PATH)/../../../Classes/BackToTitleLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/ContinueGameScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/ContinueMainLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/ExitPopupLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameInitLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameMainLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/GameSetLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/NewGameScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/NoUsableSaveLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/SaveGameCoverLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/SaveGameLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/SelectGameRolesLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/SelectRoleDuplicateLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/TitleAniLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/TitleAniScene.cpp \
                   $(LOCAL_PATH)/../../../Classes/TitleBackgroundLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/TitleLayer.cpp \
                   $(LOCAL_PATH)/../../../Classes/TitleScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
