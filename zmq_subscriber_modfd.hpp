#ifndef ZMQ_SUBSCRIBER_MODFD_HPP
#define ZMQ_SUBSCRIBER_MODFD_HPP

#include <string>
#include <thread>
#include <vector>

#include <zmq.hpp>

class Subscriber_modfd{
public:
    zmq::context_t _context;
private:
    zmq::socket_t _receiver;
    std::vector<std::string> receiverPoints;
    std::string _filter;
    bool _stop = false;
    bool _stopped = true;
    bool _block = false;

    // config
    std::string _parameter_name = "receiver_points";
    uint8_t _in_queue_size  = 1;

public:
    Subscriber_modfd();
    ~Subscriber_modfd() = default;

    inline void set_block_mode( bool mode = false){_block = mode;}

    void configure (const std::string &add){
        receiverPoints.clear();
        receiverPoints.push_back(add);
    }
    bool start(std::string &errStr) noexcept;
    bool stop() noexcept;
    bool receive(zmq::message_t *message) noexcept;

    void set_parameter_name(const char *name) noexcept{_parameter_name = name;}
    void set_parameter_name(const std::string &name) noexcept{_parameter_name = name;}
    void add_receiver_point(const std::string& rp) noexcept{receiverPoints.push_back(rp);}
    void set_queue_size(unsigned size) noexcept{_in_queue_size = size;}
};

#endif // ZMQ_SUBSCRIBER_MODFD_HPP
