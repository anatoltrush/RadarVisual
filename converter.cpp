#include "converter.h"

uint16_t Converter::getDecData(const QString &hexData, uint8_t indBeg, uint8_t len){
    QString binData = hexToBin(hexData);

    QString binFragment = binData.mid(indBeg, len);

    bool ok = false;
    uint16_t res = binFragment.toUShort(&ok, 2);

    return res;
}

QString Converter::hexToBin(const QString &hexData){
    bool ok = false;
    QString binData = QString::number(hexData.toLongLong(&ok, 16), 2);
    while (binData.length() < hexData.length() * 4)
        binData.prepend("0");
    return binData;
}
