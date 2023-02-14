#ifndef ZMQ_CLIENT_HPP
#define ZMQ_CLIENT_HPP

#include <string>
#include <vector>
#include <thread>
#include <iostream>

#include <zmq.hpp>

class Client{
public:
    Client();

    inline void set_block_mode(bool mode = false){_block = mode;}

    void configure(const std::string &addr){
        senderPoints.clear();
        senderPoints.push_back(addr);
    }
    bool start(std::string &errStr);
    bool stop();
    bool receive(zmq::message_t *message, std::string &errStr);
    bool send(zmq::message_t &message, std::string &errStr);

    void set_parameter_name(const char *name);
    void set_parameter_name(const std::string &name);
    void add_sender_point(const std::string& sp);

private:
    zmq::context_t _context;
    zmq::socket_t client;
    std::vector<std::string> senderPoints;
    bool _stop = false;
    bool _stopped = true;
    bool _block = false;
    unsigned _in_queue_size = 1;
    unsigned _out_queue_size = 1;
};

#endif // ZMQ_CLIENT_HPP
