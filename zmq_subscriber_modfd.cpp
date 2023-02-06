#include "zmq_subscriber_modfd.hpp"

#include <iostream>

Subscriber_modfd::Subscriber_modfd() : _context(1), _receiver(_context, ZMQ_SUB), _filter("") {}

bool Subscriber_modfd::start(std::string &errStr) noexcept{
    _stop = false;
    try{
        if(_in_queue_size)
            _receiver.setsockopt(ZMQ_RCVHWM, _in_queue_size);

        _receiver.setsockopt(ZMQ_SUBSCRIBE, _filter.c_str(), _filter.length());

        for(const auto &it: receiverPoints)
            _receiver.connect(it);
    }
    catch(std::exception &e){
        errStr = std::string(e.what());
        return false;
    }

    _stopped = false;
    return true;
}

bool Subscriber_modfd::stop() noexcept{
    bool result = true;
    _stop = true;
    for(const auto &it: receiverPoints){
        try{
            _receiver.disconnect(it);
        }
        catch(std::exception &e){
            result = false;
        }
    }
    _stopped = true;
    return result;
}

bool Subscriber_modfd::receive(zmq::message_t *message) noexcept{
    if (_block){
        if (_receiver.recv(message)) return true;
        else return false;
    }
    else {
        do{
            if (!_stopped && _receiver.recv(message, ZMQ_DONTWAIT)) return true;
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
        }
        while(!_stop);
    }
    return false;
}
