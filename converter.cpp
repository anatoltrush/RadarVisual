#include "converter.h"

uint16_t Converter::getDecData(const QString &hexData, uint8_t indBeg, uint8_t len){
    QString binData = hexToBin(hexData);

    QString binFragment = binData.mid(indBeg, len);

    bool ok = false;
    uint16_t res = binFragment.toUShort(&ok, 2);

    return res;
}

QString Converter::floatCutOff(float value, int afterDot){
    std::stringstream stream;
    stream << std::fixed << std::setprecision(afterDot) << value;
    return QString::fromStdString(stream.str());
}

CanLine Converter::getCanLineFromCanData(const std::string &device, const canfd_frame &frame){
    CanLine canLine;
    canLine.timeStamp = GET_CUR_TIME_MICRO;
    canLine.canNum = QString::fromStdString(device);
    canLine.messId = hexToDec(frame.can_id);
    QString hexData;
    for(uint8_t i = 0; i < frame.len; i++)
        hexData.append(hexToDec2(frame.data[i]));
    canLine.messData = hexData;
    return canLine;
}

QString Converter::hexToBin(const QString &hexData){
    bool ok = false;
    QString binData = QString::number(hexData.toULongLong(&ok, 16), 2);
    while (binData.length() < hexData.length() * 4)
        binData.prepend("0");
    return binData;
}

QString Converter::hexToDec2(uint8_t data){
    QString hexStr = QString::number(data, 16);
    while (hexStr.length() < 2)
        hexStr.prepend("0");
    return hexStr;
}
