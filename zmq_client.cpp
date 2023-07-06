#include "zmq_client.hpp"

Client::Client(): _context(1), client(_context, ZMQ_REQ){}

bool Client::start(std::string &errStr){
    bool result = true;

    client.setsockopt(ZMQ_RCVTIMEO, timRcvMsec);
    client.setsockopt(ZMQ_SNDTIMEO, timSnsMsec);

    _stop = false;
    try{
        if(_in_queue_size) client.setsockopt(ZMQ_RCVHWM, _in_queue_size);
        if(_out_queue_size) client.setsockopt(ZMQ_SNDHWM, _out_queue_size);
        for(const auto &its: senderPoints)
            client.connect(its);
    }
    catch(std::exception &e){
        errStr = std::string(e.what());
        result = false;
    }
    _stopped = false;
    return result;
}

bool Client::stop(){
    _stop = true;
    bool result = true;
    for(const auto &its: senderPoints){
        try{client.disconnect(its);}
        catch(std::exception &e){result = false;}
    }
    _stopped = true;
    return result;
}

bool Client::receive(zmq::message_t *message, std::string &errStr, uint8_t efforts){
    try{
        uint8_t count = 0;
        do{
            if(!_stopped && client.recv(message, _block ? 0 : ZMQ_DONTWAIT))
                return true;
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
            if(efforts > 0){
                count++;
                if(count >= efforts)
                    break;
            }
        }
        while(!_stop);
    }
    catch(std::exception &e){
        errStr = std::string(e.what());
    }
    return false;
}

bool Client::send(zmq::message_t &message, std::string &errStr){
    try{
        do{
            if(!_stopped && client.send(message, _block ? 0 : ZMQ_DONTWAIT)){
                return true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
        }
        while(!_stop);
    }
    catch(const std::exception &e){
        errStr = std::string(e.what());
    }
    return false;
}

void Client::add_sender_point(const std::string& sp){
    senderPoints.push_back(sp);
}
