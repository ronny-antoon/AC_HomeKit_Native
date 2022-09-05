//
// Created by Ronny on 29/08/2022.
//

#ifndef AC_HOMEKIT_NATIVE_ACSERVICE_H
#define AC_HOMEKIT_NATIVE_ACSERVICE_H


#include "HomeSpan.h"
#include "IRsend.h"
#include "MetaIR.h"

class ACService : public  Service::HeaterCooler{
private:
    stdAc::state_t newState{};
    void sendTask();
    void updateCurrentCharacteristic() const;
    MetaIR *metaIr;
    void updateHomeKit();

public:

    // Create properties
    SpanCharacteristic *active;
    SpanCharacteristic *currentTemperature;
    SpanCharacteristic *currentHeaterCoolerState;
    SpanCharacteristic *targetHeaterCoolerState;
    SpanCharacteristic *rotationSpeed;
    SpanCharacteristic *temperatureDisplayUnits;
    SpanCharacteristic *swingMode;
    SpanCharacteristic *coolingThresholdTemperature;
    SpanCharacteristic *heatingThresholdTemperature;

    // start constructor
    ACService();

    //when homekit (control) change run this func
    boolean update() override;

    // when Remote (control) change run this func
    void loop() override;


    float getTemperatureDegree();
    bool getPowerAC();
    stdAc::swingv_t getSwingMode();
    stdAc::fanspeed_t getFanSpeedMode();
    stdAc::opmode_t getACMode();

};


#endif //AC_HOMEKIT_NATIVE_ACSERVICE_H
