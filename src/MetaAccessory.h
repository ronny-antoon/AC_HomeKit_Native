//
// Created by Ronny on 30/08/2022.
//

#include "HomeSpan.h"

#ifndef AC_HOMEKIT_NATIVE_METAACCESSORY_H
#define AC_HOMEKIT_NATIVE_METAACCESSORY_H


class MetaAccessory : public SpanAccessory{
public:

    MetaAccessory(uint32_t mAid, const char *name, const char *manufacturer, const char *model, const char *serialNumber);

};


#endif //AC_HOMEKIT_NATIVE_METAACCESSORY_H
