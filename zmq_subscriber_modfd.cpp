#include "zmq_subscriber_modfd.hpp"

#include <boost/foreach.hpp>

namespace ZMQ{
Subscriber_modfd::Subscriber_modfd() : _context(1), _receiver(_context, ZMQ_SUB), _filter("") { }
bool Subscriber_modfd::configure(const pt::ptree &cfg, std::string &answer) noexcept{
    try
    {
        boost::optional<const pt::ptree&> points = cfg.get_child_optional(_parameter_name);
        if(points)
        {
            _receiver_points.clear();
            BOOST_FOREACH(const pt::ptree::value_type &child, cfg.get_child(_parameter_name))
            {
                assert(child.first.empty());
                _receiver_points.push_back(child.second.get_value<std::string>());
            }
        }

        boost::optional<std::string> pstr = cfg.get_optional<std::string>("filter");
        if(pstr) _filter = *pstr;

        boost::optional<unsigned> puint = cfg.get_optional<unsigned>("in_queue_size");
        if(puint) _in_queue_size = *puint;

        return true;
    }
    catch(const pt::ptree_error &e){
        answer += std::string("ZMQ::Subscriber. Configuratiom error: ") + e.what() + "\n";
    }
    return false;
}

bool Subscriber_modfd::start() noexcept{
    _stop = false;
    try{
        if(_in_queue_size)
            _receiver.setsockopt(ZMQ_RCVHWM, _in_queue_size);

        _receiver.setsockopt(ZMQ_SUBSCRIBE, _filter.c_str(), _filter.length());

        for(auto it: _receiver_points)
            _receiver.connect(it);
    }
    catch(std::exception &e){
        return false;
    }

    _stopped = false;
    return true;
}

bool Subscriber_modfd::stop() noexcept{
    bool result = true;
    _stop = true;
    for(auto it: _receiver_points){
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

void Subscriber_modfd::set_parameter_name(const char *name) noexcept{
    _parameter_name = name;
}

void Subscriber_modfd::set_parameter_name(const std::string &name) noexcept{
    _parameter_name = name;
}

void Subscriber_modfd::add_receiver_point(const std::string& rp) noexcept{
    _receiver_points.push_back(rp);
}

void Subscriber_modfd::rem_receiver_point(const std::string& rp) noexcept{
    auto it = _receiver_points.begin();
    while(it != _receiver_points.end()){
        if(!it->compare(rp)) it = _receiver_points.erase(it);
        else it++;
    }
}
};
