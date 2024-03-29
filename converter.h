#ifndef CONVERTER_H
#define CONVERTER_H

#include <zmq.hpp>

#include "entity.h"

class Converter{
public:    
    static uint16_t getDecData(const QString& hexData, uint8_t indBeg, uint8_t len);
    static QString floatCutOff(const float &value, int afterDot);

    static void getZmqFromCanFd(zmq::message_t& message, const canfd_frame& frame, const MessageId& id);
    static void getCanFdFromZmq(const zmq::message_t& message, canfd_frame& frame, MessageId& id);
    static void getCanFdFromCanLine(canfd_frame& frame, const CanLine &canLine);
    static CanLine getCanLineFromCanFd(const std::string &device, const canfd_frame& frame, bool isZmq);

    static QString binToHex(const QString &binStr);
    static QString decToBin(const QString &decStr, uint8_t binLen);

private:
    static QString hexToBin(const QString& hexStr);
    static QString hexToDec(uint data){return QString::number(data, 16);}
    static QString hexToDec2(uint8_t data);
};

#endif // CONVERTER_H
