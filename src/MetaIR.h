//
// Created by Ronny on 30/08/2022.
//

#ifndef AC_HOMEKIT_NATIVE_METAIR_H
#define AC_HOMEKIT_NATIVE_METAIR_H


#include <Arduino.h>
#include <IRrecv.h>
#include <IRac.h>
#include <IRutils.h>

class MetaIR {
    IRac *ac;
//    IRrecv *irrecv;
//    decode_results results;

public:
    MetaIR();

    void sendIR(float degree, bool power,stdAc::swingv_t swingv, stdAc::fanspeed_t fanSpeedMode, stdAc::opmode_t acMode);

//    bool receiveIR(stdAc::state_t *result);
};


#endif //AC_HOMEKIT_NATIVE_METAIR_H
