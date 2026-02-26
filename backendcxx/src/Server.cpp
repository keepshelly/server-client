#include "GlobalData.h"
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <thread>
using json = nlohmann::json;
void server_thread_func(GlobalData* globalData) {
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5554");
    std::cout << "server listening on 5554" << std::endl;
    while (true) {
        zmq::message_t request;
        auto res = socket.recv(request, zmq::recv_flags::none);
        if (!res) continue;
        std::string msg_str(static_cast<char*>(request.data()), request.size());
        float lat = 0.0f;
        float lon = 0.0f;
        try {
            auto j = json::parse(msg_str);
            if(j.contains("lat")) lat = j["lat"];
            if(j.contains("lon")) lon = j["lon"];
            std::ofstream file("gps_log.json");
            file << j.dump(4);
            file.close();
            auto res = socket.recv(request, zmq::recv_flags::none);
            if (res) {
            std::string msg(static_cast<char*>(request.data()), request.size());
            std::cout << "DEBUG: Received raw data: [" << msg << "]" << std::endl;
}
        } catch (...) {
            std::cout << "JSON error" << std::endl;
        }
        globalData->update(lat, lon, msg_str);
        std::string reply = "OK";
        socket.send(zmq::message_t(reply), zmq::send_flags::none);
    }
}