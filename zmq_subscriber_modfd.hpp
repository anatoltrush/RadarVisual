#ifndef ZMQ_SUBSCRIBER_MODFD_HPP
#define ZMQ_SUBSCRIBER_MODFD_HPP

#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <iostream>

#include <zmq.hpp>

#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;

namespace ZMQ{
class Subscriber_modfd{
public:
    zmq::context_t _context;
private:
    zmq::socket_t _receiver;
    std::vector<std::string> _receiver_points;
    std::string _filter;
    bool _stop = false;
    bool _stopped = true;
    bool _block = false;

    // config
    std::string _parameter_name = "receiver_points";
    unsigned _in_queue_size = 1;

public:
    Subscriber_modfd();
    ~Subscriber_modfd() = default;

    inline void set_block_mode( bool mode = false){_block = mode;}

    //
    bool configure(const pt::ptree &cfg, std::string &answer) noexcept;
    bool start() noexcept;
    bool stop() noexcept;
    bool receive(zmq::message_t *message) noexcept;

    //
    void set_parameter_name(const char *name) noexcept;
    void set_parameter_name(const std::string &name) noexcept;
    void add_receiver_point(const std::string& rp) noexcept;
    void rem_receiver_point(const std::string& rp) noexcept;

    //
    template<typename T> bool setopt(int option, T const& optval) noexcept{
        bool result = true;
        try{
            _receiver.setsockopt(option, optval);
        }
        catch(const std::exception& e){
            result = false;
        }
        return result;
    }

    inline void set_queue_size(unsigned size) noexcept{_in_queue_size = size;}
};
};

#endif // ZMQ_SUBSCRIBER_MODFD_HPP
