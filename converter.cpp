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

#ifdef __WIN32
#else
void Converter::getCanFdFromZmq(const zmq::message_t &message, canfd_frame &frame, MessageId &id){
    if(message.data<ZmqCanMessage>()->_msg_type == MsgType::CANMsg){
        id = message.data<ZmqCanMessage>()->_id;

        frame.can_id = message.data<ZmqCanMessage>()->_frame.can_id;
        frame.len = message.data<ZmqCanMessage>()->_frame.len;
        frame.flags = message.data<ZmqCanMessage>()->_frame.flags;
        frame.__res0 = message.data<ZmqCanMessage>()->_frame.__res0;
        frame.__res1 = message.data<ZmqCanMessage>()->_frame.__res1;

        for( __u8 idx = 0; idx < frame.len; ++idx)
            frame.data[idx] = message.data<ZmqCanMessage>()->_frame.data[idx];
    }
}

void Converter::getZmqFromCanFd(zmq::message_t &message, const canfd_frame &frame, const MessageId &id){
    message.rebuild(sizeof(ZmqCanMessage));

    message.data<ZmqCanMessage>()->_msg_type = MsgType::CANMsg;
    message.data<ZmqCanMessage>()->_id = id;

    message.data<ZmqCanMessage>()->_frame.can_id = frame.can_id;
    message.data<ZmqCanMessage>()->_frame.len = frame.len;
    message.data<ZmqCanMessage>()->_frame.flags = frame.flags;
    message.data<ZmqCanMessage>()->_frame.__res0 = frame.__res0;
    message.data<ZmqCanMessage>()->_frame.__res1 = frame.__res1;

    for( __u8 idx = 0; idx < frame.len; ++idx)
        message.data<ZmqCanMessage>()->_frame.data[idx] = frame.data[idx];
}

void Converter::getCanFdFromCanLine(canfd_frame &frame, const CanLine &canLine){
    const uint8_t fragmSz = 2;

    frame.can_id = canLine.messId.toULong();
    frame.len = canLine.messData.length() / fragmSz;
    frame.flags = 0;
    frame.__res0 = 0;
    frame.__res1 = 0;

    /*if(frame.len == 8){
        uint8_t dataStr[8] = {255, 255, 255, 255, 255, 255, 255, 255};
        for( uint8_t idx = 0; idx < 8; ++idx){
            QString hexFragment = canLine.messData.mid(idx * fragmSz, fragmSz);
            bool ok = false;
            uint vvv = hexFragment.toUInt(&ok, 16);
            dataStr[idx] = vvv;
            int c = 5;
        }
        int b = 5;
    }*/

    for( __u8 idx = 0; idx < frame.len; ++idx){
        QString hexFragment = canLine.messData.mid(idx * fragmSz, fragmSz);
        bool ok = false;
        frame.data[idx] = hexFragment.toUInt(&ok, 16);
    }
}

CanLine Converter::getCanLineFromCanFd(const std::string &device, const canfd_frame &frame, bool isZmq){
    CanLine canLine;
    canLine.timeStamp = GET_CUR_TIME_MICRO;
    isZmq ? canLine.canNum = "can" + QString::fromStdString(device) : canLine.canNum = QString::fromStdString(device);
    canLine.messId = hexToDec(frame.can_id);
    QString hexData;
    for(uint8_t i = 0; i < frame.len; i++)
        hexData.append(hexToDec2(frame.data[i]));
    canLine.messData = hexData;
    return canLine;
}
#endif

QString Converter::binToHex(const QString &binStr){
    QString hexStr;
    bool ok = false;
    hexStr = QString::number(binStr.toULongLong(&ok, 2), 16);
    while (hexStr.length() < binStr.length() / 4)
        hexStr.prepend("0");
    hexStr = hexStr.toUpper();
    for (size_t i = 0; i < 7; i++){
        uint8_t ind = i * 3 + 2;
        hexStr.insert(ind, '.');
    }
    return hexStr;
}

QString Converter::decToBin(const QString &decStr, uint8_t binLen){
    bool ok = false;
    QString binData = QString::number(decStr.toULongLong(&ok, 10), 2);
    while (binData.length() < binLen)
        binData.prepend("0");
    return binData;
}

QString Converter::hexToBin(const QString &hexStr){
    bool ok = false;
    QString binData = QString::number(hexStr.toULongLong(&ok, 16), 2);
    while (binData.length() < hexStr.length() * 4)
        binData.prepend("0");
    return binData;
}

QString Converter::hexToDec2(uint8_t data){
    QString hexStr = QString::number(data, 16);
    while (hexStr.length() < 2)
        hexStr.prepend("0");
    return hexStr;
}
