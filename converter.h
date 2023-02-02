#ifndef CONVERTER_H
#define CONVERTER_H

#include "entity.h"

class Converter{
public:    
    static uint16_t getDecData(const QString& hexData, uint8_t indBeg, uint8_t len);
    static QString floatCutOff(float value, int afterDot);
#ifdef __WIN32
#else
    static CanLine getCanLineFromCanData(const std::string &device, const canfd_frame& frame);
#endif

private:
    static QString hexToBin(const QString& hexData);
    static QString hexToDec(uint data){return QString::number(data, 16);}
    static QString hexToDec2(uint8_t data);
};

#endif // CONVERTER_H
