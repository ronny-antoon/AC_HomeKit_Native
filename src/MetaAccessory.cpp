//
// Created by Ronny on 30/08/2022.
//

#include "MetaAccessory.h"

MetaAccessory::MetaAccessory(uint32_t mAid, const char *name, const char *manufacturer, const char *model, const char *serialNumber) : SpanAccessory(mAid){

    new Service::AccessoryInformation();
    new Characteristic::Name(name);
    new Characteristic::Manufacturer(manufacturer);
    new Characteristic::Model(model);
    new Characteristic::SerialNumber(serialNumber);
    new Characteristic::Identify();

}
