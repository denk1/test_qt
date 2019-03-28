#ifndef CONNECTION_CONTROL_H
#define CONNECTION_CONTROL_H
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <nlohmann/json.hpp>
#include "ITS.h"

typedef websocketpp::server<websocketpp::config::asio> server;
typedef websocketpp::connection_hdl connection_hdl;
typedef server::message_ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::lib::error_code;
using json = nlohmann::json;

using namespace std::chrono_literals;

namespace RAT {

class ConnectionControl
{
public:
    ConnectionControl(ITS* ptrITS);
    ~ConnectionControl();
    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void on_message(connection_hdl hdl, message_ptr msg);
    void on_timer(error_code const &ec);
    void set_timer(int t);
    void run();
    void start();
protected:
    Ogre::Real getParamValue(Ogre::String nameParam, json arrayParams);
private:
    //void setServer(server* inPtrServer);
    websocketpp::connection_hdl mHdl;
    std::thread mThread;
    server* mPtrServer;
    server::timer_ptr mPtrTimer;
    ITS* mPtrITS;
    bool isOpen;
};

}
#endif
