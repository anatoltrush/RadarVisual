#include "zmq_subscriber_modfd.hpp"

#include <iostream>

Subscriber_modfd::Subscriber_modfd() : _context(1), receiver(_context, ZMQ_SUB), _filter("") {}

bool Subscriber_modfd::start(std::string &errStr){
    receiver.setsockopt(ZMQ_RCVTIMEO, timRcvMsec);

    _stop = false;
    try{
        if(_in_queue_size) receiver.setsockopt(ZMQ_RCVHWM, _in_queue_size);

        receiver.setsockopt(ZMQ_SUBSCRIBE, _filter.c_str(), _filter.length());

        for(const auto &it: receiverPoints)
            receiver.connect(it);
    }
    catch(std::exception &e){
        errStr = std::string(e.what());
        return false;
    }

    _stopped = false;
    return true;
}

bool Subscriber_modfd::stop(){
    bool result = true;
    _stop = true;
    for(const auto &it: receiverPoints){
        try{receiver.disconnect(it);}
        catch(std::exception &e){result = false;}
    }
    _stopped = true;
    return result;
}

bool Subscriber_modfd::receive(zmq::message_t *message){
    if (_block){
        if (receiver.recv(message)) return true;
        else return false;
    }
    else {
        do{
            if (!_stopped && receiver.recv(message, ZMQ_DONTWAIT)) return true;
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
        }
        while(!_stop);
    }
    return false;
}
