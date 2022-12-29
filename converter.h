#ifndef CONVERTER_H
#define CONVERTER_H

#include <QMap>

class Converter
{
public:
    Converter();

    uint8_t currRadarNum = 255;
    uint8_t currProperty = 255;
    QMap<QString, bool> types;
};

#endif // CONVERTER_H
