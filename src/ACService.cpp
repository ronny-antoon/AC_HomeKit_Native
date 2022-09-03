//
// Created by Ronny on 29/08/2022.
//

#include "ACService.h"
#include "Constants.h"

ACService::ACService() : Service::HeaterCooler() {

    newState = nullptr;

    metaIr = new MetaIR();

    active = new Characteristic::Active(0, false);

    currentTemperature = new Characteristic::CurrentTemperature(22, false); // sensor temp
    currentTemperature->setRange(10,50,1);

    currentHeaterCoolerState = new Characteristic::CurrentHeaterCoolerState(2, false); // SpanCharacteristic *setValidValues(int n, [int v1, int v2 ...]) 1 idle, 2 heating, 3 cooling
    currentHeaterCoolerState->setValidValues(2,2,3);

    targetHeaterCoolerState = new Characteristic::TargetHeaterCoolerState(1, false); // SpanCharacteristic *setValidValues(int n, [int v1, int v2 ...]) 0 auto, 1 heat, 2 cool
    targetHeaterCoolerState->setValidValues(2,1,2);

    rotationSpeed = new Characteristic::RotationSpeed(30, false); //SpanCharacteristic *setValidValues(int n, [int v1, int v2 ...])

    temperatureDisplayUnits = new Characteristic::TemperatureDisplayUnits(0, false);

    swingMode = new Characteristic::SwingMode(1, false);

    coolingThresholdTemperature = new Characteristic::CoolingThresholdTemperature(16, false); // cooling temp
    coolingThresholdTemperature->setRange(16,30,1);
    heatingThresholdTemperature = new Characteristic::HeatingThresholdTemperature(30, false); // heating temp
    heatingThresholdTemperature->setRange(16,30,1);

}

boolean ACService::update() {
    Serial.println("Update requested");
    // send InfraRed for New values
    ACService::syncTargetModeAndCurrent();
    sendTask();
    return true;
}

void ACService::loop() {

//    if(metaIr->receiveIR(newState)){
//        updateHomeKit(newState);
//    }

}

void ACService::sendTask() {
    metaIr->sendIR(getTemperatureDegree(),
                   getPowerAC(),
                   getSwingMode(),
                   getFanSpeedMode(),
                   getACMode());
}

void ACService::syncTargetModeAndCurrent() const {
    if(targetHeaterCoolerState->getNewVal() == constants::HEATING_TARGET_STATE){
        currentHeaterCoolerState->setVal(constants::HEATING_CURRENT_STATE);
    }
    if(targetHeaterCoolerState->getNewVal() == constants::COOLING_TARGET_STATE){
        currentHeaterCoolerState->setVal(constants::COOLING_CURRENT_STATE);
    }
}

float ACService::getTemperatureDegree() {
    if(currentHeaterCoolerState->getVal() == constants::COOLING_CURRENT_STATE)
        return (float)coolingThresholdTemperature->getVal<double>();
    if(currentHeaterCoolerState->getVal() == constants::HEATING_CURRENT_STATE)
        return (float)heatingThresholdTemperature->getVal<double>();
    return 20;
}

bool ACService::getPowerAC() {
    if(active->getVal<uint8_t>() == 1)
        return true;
    return false;
}

stdAc::swingv_t ACService::getSwingMode() {
    if(swingMode->getVal<uint8_t>() == 0)
        return stdAc::swingv_t::kOff;
    return stdAc::swingv_t::kAuto;
}

stdAc::fanspeed_t ACService::getFanSpeedMode() {
    if(rotationSpeed->getVal<double>() <= 33)
        return stdAc::fanspeed_t::kLow;
    if(rotationSpeed->getVal<double>() >= 66)
        return stdAc::fanspeed_t::kHigh;
    return stdAc::fanspeed_t::kMedium;
}

stdAc::opmode_t ACService::getACMode() {
    if (currentHeaterCoolerState->getVal() == constants::COOLING_CURRENT_STATE)
        return stdAc::opmode_t::kCool;
    if (currentHeaterCoolerState->getVal() == constants::HEATING_CURRENT_STATE)
        return stdAc::opmode_t::kHeat;
    return stdAc::opmode_t::kOff;
}

void ACService::updateHomeKit(stdAc::state_t *_newState){
    // TODO: convert from newState to homekit characteristics
    if(_newState->power) {
        // MODE
        if (_newState->mode == stdAc::opmode_t::kCool)
            targetHeaterCoolerState->setVal(constants::COOLING_TARGET_STATE);
        else if (_newState->mode == stdAc::opmode_t::kHeat)
            targetHeaterCoolerState->setVal(constants::HEATING_TARGET_STATE);
        syncTargetModeAndCurrent();
        // end MODE

        // TEMP
        if (currentHeaterCoolerState->getVal() == constants::COOLING_CURRENT_STATE)
            coolingThresholdTemperature->setVal(_newState->degrees);
        else
            heatingThresholdTemperature->setVal(_newState->degrees);
        // end TEMP

        // SWING
        if (_newState->swingv == stdAc::swingv_t::kAuto)
            swingMode->setVal(1);
        else
            swingMode->setVal(0);
        // end SWING

        // FANSPEED
        if(_newState->fanspeed == stdAc::fanspeed_t::kLow)
            rotationSpeed->setVal(20);
        else if(_newState->fanspeed == stdAc::fanspeed_t::kHigh)
            rotationSpeed->setVal(80);
        else
            rotationSpeed->setVal(50);
        // end FANSPEED

        // ACTIVE
        active->setVal(1, true);
    } else{
        active->setVal(0, true);
    }
}