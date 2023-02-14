#ifndef ZMQ_SUBSCRIBER_MODFD_HPP
#define ZMQ_SUBSCRIBER_MODFD_HPP

#include <string>
#include <thread>
#include <vector>

#include <zmq.hpp>

class Subscriber_modfd{
public:
    Subscriber_modfd();

    zmq::context_t _context;

    inline void set_block_mode(bool mode = false){_block = mode;}

    void configure (const std::string &addr){
        receiverPoints.clear();
        receiverPoints.push_back(addr);
    }
    bool start(std::string &errStr);
    bool stop();
    bool receive(zmq::message_t *message);

    void add_receiver_point(const std::string& rp) {receiverPoints.push_back(rp);}
    void set_queue_size(unsigned size){_in_queue_size = size;}

private:
    zmq::socket_t receiver;
    std::vector<std::string> receiverPoints;
    std::string _filter;
    bool _stop = false;
    bool _stopped = true;
    bool _block = false;
    uint8_t _in_queue_size  = 1;
    int timRcvMsec = 3000;
};

#endif // ZMQ_SUBSCRIBER_MODFD_HPP
