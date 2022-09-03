#include "HomeSpan.h"
#include "ACService.h"
#include "MetaAccessory.h"
#include "Constants.h"

// Declare Functions
void setupHomeSpan();
void startSerial(unsigned long baud);

MetaAccessory *acAccessory;
ACService *acService;

void setup() {

    startSerial(constants::baudRate);

    setupHomeSpan();

    homeSpan.begin(Category::AirConditioners,"MetaHouse AC");

    acAccessory = new MetaAccessory(1,"MetaHouse AC","MetaHouse", "Air Conditioner", "fff15r322");

    acService = new ACService();
}

void loop() {

    homeSpan.poll();

}

void setupHomeSpan(){
    homeSpan.setApSSID("MetaHouse_AC_Setup");
    homeSpan.setApPassword("123456789");
    homeSpan.enableAutoStartAP();
    homeSpan.setQRID("HYED"); // generate QR Code
}

void startSerial(unsigned long baud){
    Serial.begin(baud);
    while (!Serial)  // Wait for the serial connection to be established.
        delay(50);
}

