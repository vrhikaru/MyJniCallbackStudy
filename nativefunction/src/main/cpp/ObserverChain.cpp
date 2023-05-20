//
// Created by vrhik on 2023/5/17.
//

#include "ObserverChain.h"

ObserverChain::ObserverChain(jweak pJobject, jmethodID pID, jmethodID pJmethodID) {
    store_Wlistener= pJobject;
    store_method = pID;
    store_methodVAL = pJmethodID;
}
