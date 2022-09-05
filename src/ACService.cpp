//
// Created by Ronny on 29/08/2022.
//

#include "ACService.h"
#include "Constants.h"

ACService::ACService() : Service::HeaterCooler() {

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
    // send InfraRed for New values
    Serial.println("ACService::update -- got new state from the app.");
    yield();
    ACService::updateCurrentCharacteristic();
    sendTask();
    return true;
}

void ACService::loop() {

    // TODO: get sensor for current temp at room

    if(metaIr->receiveIR(&newState)){
        updateHomeKit();
    }

}

void ACService::sendTask() {
    metaIr->sendIR(getTemperatureDegree(),
                   getPowerAC(),
                   getSwingMode(),
                   getFanSpeedMode(),
                   getACMode());
}

void ACService::updateCurrentCharacteristic() const {

///  update value
    active->setVal(active->getNewVal());
    targetHeaterCoolerState->setVal(targetHeaterCoolerState->getNewVal());
    rotationSpeed->setVal(rotationSpeed->getNewVal());
    temperatureDisplayUnits->setVal(temperatureDisplayUnits->getNewVal());
    swingMode->setVal(swingMode->getNewVal());
    coolingThresholdTemperature->setVal(coolingThresholdTemperature->getNewVal());
    heatingThresholdTemperature->setVal(heatingThresholdTemperature->getNewVal());


    if(targetHeaterCoolerState->getVal() == constants::HEATING_TARGET_STATE){
        currentHeaterCoolerState->setVal(constants::HEATING_CURRENT_STATE);
    }
    if(targetHeaterCoolerState->getVal() == constants::COOLING_TARGET_STATE){
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

void ACService::updateHomeKit(){
    Serial.println("ACService::updateHomeKit -- got new state from IR controller, update homekit characteristics");
    yield();
    if(newState.power) {
        // MODE
        if (newState.mode == stdAc::opmode_t::kCool) {
            targetHeaterCoolerState->setVal(constants::COOLING_TARGET_STATE);
            currentHeaterCoolerState->setVal(constants::COOLING_CURRENT_STATE);
        }
        else if (newState.mode == stdAc::opmode_t::kHeat) {
            targetHeaterCoolerState->setVal(constants::HEATING_TARGET_STATE);
            currentHeaterCoolerState->setVal(constants::HEATING_CURRENT_STATE);
        }
        // end MODE

        // TEMP
        if (currentHeaterCoolerState->getVal() == constants::COOLING_CURRENT_STATE)
            coolingThresholdTemperature->setVal(newState.degrees);
        else
            heatingThresholdTemperature->setVal(newState.degrees);
        // end TEMP

        // SWING
        if (newState.swingv == stdAc::swingv_t::kAuto)
            swingMode->setVal(1);
        else
            swingMode->setVal(0);
        // end SWING

        // FANSPEED
        if(newState.fanspeed == stdAc::fanspeed_t::kLow)
            rotationSpeed->setVal(20);
        else if(newState.fanspeed == stdAc::fanspeed_t::kHigh)
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