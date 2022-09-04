#include "HomeSpan.h"
#include "ACService.h"
#include "MetaAccessory.h"
#include "Constants.h"

// Declare Functions
void setupHomeSpan();
void startSerial(unsigned long baud);

MetaAccessory *acAccessory;
ACService *acService;

/** First function called by the bootloader
 *
 * Setup program, this function called once the device start
 *
 */
void setup() {

    startSerial(constants::baudRate); // Init Serial and begin, for communication between pc and esp32

    Serial.println("Serial Setup-------------------------------------------");

    setupHomeSpan(); // setup properties for HomeSpan

    homeSpan.begin(Category::AirConditioners, "MetaHouse AC"); // begin with Accessory parent AC

    /** Init Accessory
     *
     *  new Span Accessory
     *  new Information Service
     *
     */
    acAccessory = new MetaAccessory(1,
                                    "MetaHouse AC",
                                    "MetaHouse",
                                    "Air Conditioner",
                                    "fff15r322");

    /** Init Span Service and setup all characteristics
    *
    *  new HeaterCooler Service
    *
    */
    acService = new ACService();
    Serial.println("Serial Start-------------------------------------------");
}

/** loop function
 *
 * this function called every click (Hz)
 * dont let this loop to block program from smoothly running
 *
 */
void loop() {

    /** HomeSpan loop function
     *
     * this function make some background tasks, then calls every Service loop in the HomeSpan accessories
     *
     */
    homeSpan.poll();

}

/**
 *  setup HomeSpan properties
 */
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

