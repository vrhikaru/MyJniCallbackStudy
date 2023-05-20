//
// Created by vrhik on 2023/5/17.
//

#ifndef MY_JNI_CALLBACK_OBSERVERCHAIN_H
#define MY_JNI_CALLBACK_OBSERVERCHAIN_H
#include <jni.h>

class ObserverChain {

public:
    ObserverChain(jweak pJobject, jmethodID pID, jmethodID pJmethodID);

    jweak store_Wlistener=NULL;
    jmethodID store_method = NULL;
    jmethodID store_methodVAL = NULL;
};


#endif //MY_JNI_CALLBACK_OBSERVERCHAIN_H
