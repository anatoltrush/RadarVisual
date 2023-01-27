#ifndef CONVERTER_H
#define CONVERTER_H

#include "entity.h"

class Converter{
public:
    static uint16_t getDecData(const QString& hexData, uint8_t indBeg, uint8_t len);
    static QString floatCutOff(float value, int afterDot);

private:
    static QString hexToBin(const QString& hexData);
};

#endif // CONVERTER_H
