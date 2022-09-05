//
// Created by Ronny on 30/08/2022.
//

#ifndef AC_HOMEKIT_NATIVE_CONSTANTS_H
#define AC_HOMEKIT_NATIVE_CONSTANTS_H


#include "IRremoteESP8266.h"

namespace constants
{
    const unsigned long baudRate = 115200;
    const uint8_t COOLING_CURRENT_STATE = 3;
    const uint8_t HEATING_CURRENT_STATE = 2;
    const uint8_t COOLING_TARGET_STATE = 2;
    const uint8_t HEATING_TARGET_STATE = 1;
    const uint16_t IR_LED_PIN = 13;
    const uint16_t IR_RECIVER_PIN = 12;
    const decode_type_t AC_PROTOCOL = decode_type_t::KELVINATOR;
}

namespace ir_decoder_constants
{
    // As this program is a special purpose capture/decoder, let us use a larger
    // than normal buffer, so we can handle Air Conditioner remote codes.
    const uint16_t kCaptureBufferSize = 1024;
    const uint8_t kTimeout = 50;
    const uint16_t kMinUnknownSize = 12;
}
#endif //AC_HOMEKIT_NATIVE_CONSTANTS_H
