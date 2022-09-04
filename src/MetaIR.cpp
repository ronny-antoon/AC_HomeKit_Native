//
// Created by Ronny on 30/08/2022.
//

#include "MetaIR.h"
#include "Constants.h"

MetaIR::MetaIR() {
    Serial.println("MetaIR Setup----------------------------------------");


    ac = new IRac(constants::IR_LED_PIN);
//    irrecv = new IRrecv(constants::IR_RECIVER_PIN);

    ac->next.protocol = constants::AC_PROTOCOL;
    ac->next.model = -1;  // Some A/Cs have different models. Try just the first.
    ac->next.mode = stdAc::opmode_t::kCool;  // Run in cool mode initially.
    ac->next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
    ac->next.degrees = 25;  // 25 degrees.
    ac->next.fanspeed = stdAc::fanspeed_t::kMedium;  // Start the fan at medium.
    ac->next.swingv = stdAc::swingv_t::kOff;  // Don't swing the fan up or down.
    ac->next.swingh = stdAc::swingh_t::kOff;  // Don't swing the fan left or right.
    ac->next.light = true;  // Turn off any LED/Lights/Display that we can.
    ac->next.beep = false;  // Turn off any beep from the A/C if we can.
    ac->next.econo = false;  // Turn off any economy modes if we can.
    ac->next.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
    ac->next.turbo = false;  // Don't use any turbo/powerful/etc modes.
    ac->next.quiet = false;  // Don't use any quiet/silent/etc modes.
    ac->next.sleep = -1;  // Don't set any sleep time or modes.
    ac->next.clean = false;  // Turn off any Cleaning options if we can.
    ac->next.clock = -1;  // Don't set any current time if we can avoid it.
    ac->next.power = false;  // Initially start with the unit off.

    ac->sendAc();  // Have the IRac class create and send a message.

    Serial.println("MetaIR Setup finish----------------------------------------");
    //irrecv->enableIRIn();
}

void MetaIR::sendIR(float degree, bool power,stdAc::swingv_t swingv, stdAc::fanspeed_t fanSpeedMode, stdAc::opmode_t acMode) {
    Serial.println("MetaIR Send IR----------------------------------------");


    ac->next.protocol = constants::AC_PROTOCOL;
    ac->next.degrees = degree;
    ac->next.power = power;
    ac->next.swingv = swingv;
    ac->next.fanspeed = fanSpeedMode;
    ac->next.mode = acMode;

    if (ac->hasStateChanged()) {
        Serial.println("Send IR");
        ac->sendAc();
    }

    Serial.println("MetaIR Send IR Finish----------------------------------------");
}

//bool MetaIR::receiveIR(stdAc::state_t *newState) {
//
//    newState = nullptr;
//    if (irrecv->decode(&results)) {
//        if(IRAcUtils::decodeToState(&results, newState)){
//            ac->next.protocol = newState->protocol;
//            ac->next.degrees = newState->degrees;
//            ac->next.power = newState->power;
//            ac->next.swingv = newState->swingv;
//            ac->next.fanspeed = newState->fanspeed;
//            ac->next.mode = newState->mode;
//
//            ac->sendAc();
//
//            irrecv->resume();
//            return true;
//        } else{
//            irrecv->resume();
//            return false;
//        }
//    }
//    return false;
//
//}
