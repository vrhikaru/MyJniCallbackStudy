#include <jni.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <android/log.h>
#include "ObserverChain.h"
#include "CMLog.h"

static JavaVM *jvm = nullptr;
std::vector<ObserverChain *> store_Wlistener_vector;
JNIEnv *store_env;


void txtCallback(JNIEnv *env, const _jstring *message_);

[[noreturn]] void loopingForTest();
void test_string_callback_fom_c(char *val);

extern "C"{

    JNIEXPORT jstring JNICALL
    Java_com_cmedia_nativefunction_NativeLib_stringFromJNI(
            JNIEnv* env,
            jobject /* this */) {

        std::string hello = "Hello from C++";
        return env->NewStringUTF(hello.c_str());
    }

    JNIEXPORT void JNICALL
    Java_com_cmedia_nativefunction_NativeLib_nsubscribeListener(JNIEnv *env, jobject instance,
                                                                       jobject listener) {

        env->GetJavaVM(&jvm); //store jvm reference for later call

        store_env = env;

        jweak store_Wlistener = env->NewWeakGlobalRef(listener);
        jclass clazz = env->GetObjectClass(store_Wlistener);

        jmethodID store_method = env->GetMethodID(clazz, "onAcceptMessage", "(Ljava/lang/String;)V");
        jmethodID store_methodVAL = env->GetMethodID(clazz, "onAcceptMessageVal", "(I)V");
        auto *tmpt = new ObserverChain(store_Wlistener, store_method, store_methodVAL);
        store_Wlistener_vector.push_back(tmpt);
        LOGV("Subscribe to Listener  OK");

        if (nullptr == store_method) return;

        loopingForTest();


    }
    JNIEXPORT void JNICALL
    Java_com_cmedia_nativefunction_NativeLib_ndismissListener(JNIEnv *env, jobject instance) {
        if (!store_Wlistener_vector.empty()) {
            for (auto &i: store_Wlistener_vector) {
                env->DeleteWeakGlobalRef(i->store_Wlistener);
                i->store_method = nullptr;
                i->store_methodVAL = nullptr;
            }
            store_Wlistener_vector.clear();
        }


    }

    JNIEXPORT void JNICALL
    Java_com_cmedia_nativefunction_NativeLib_nonNextListener(JNIEnv *env, jobject instance,
                                                                    jstring message_) {


        txtCallback(env, message_);

    }

}

static int i = 0;
char buff[100];

[[noreturn]] void loopingForTest(){
    std::chrono::seconds interval( 1 ) ; // 10 seconds
    while (1)
    {
        LOGV("tick %d",i++);
        snprintf(buff, sizeof(buff), "loop count = %d",i);
        test_string_callback_fom_c(buff);
        std::this_thread::sleep_for( interval ) ;
    }
}

void test_string_callback_fom_c(char *val) {

    LOGV("start Callback  to JNL [%s]",val);
    JNIEnv *g_env;
    if (nullptr == jvm) {
        __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", "  No VM  \n");
        return;
    }
    //  double check it's all ok
    JavaVMAttachArgs args;
    args.version = JNI_VERSION_1_6; // set your JNI version
    args.name = nullptr; // you might want to give the java thread a name
    args.group = nullptr; // you might want to assign the java thread to a ThreadGroup

    int getEnvStat = jvm->GetEnv((void **) &g_env, JNI_VERSION_1_6);

    if (getEnvStat == JNI_EDETACHED) {
        __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", " not attached\n");
        if (jvm->AttachCurrentThread(&g_env, &args) != 0) {
            __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", " Failed to attach\n");
        }
    } else if (getEnvStat == JNI_OK) {
        __android_log_print(ANDROID_LOG_VERBOSE, "GetEnv:", " JNI_OK\n");
    } else if (getEnvStat == JNI_EVERSION) {
        __android_log_print(ANDROID_LOG_ERROR, "GetEnv:", " version not supported\n");
    }

    jstring message = g_env->NewStringUTF(val);//

    txtCallback(g_env, message);

    if (g_env->ExceptionCheck()) {
        g_env->ExceptionDescribe();
    }

    if (getEnvStat == JNI_EDETACHED) {
        jvm->DetachCurrentThread();
    }
}

void txtCallback(JNIEnv *env, const _jstring *message_) {
    if (!store_Wlistener_vector.empty()) {
        for (auto &i: store_Wlistener_vector) {
            env->CallVoidMethod(i->store_Wlistener,
                                i->store_method, message_);
        }
    }
}
