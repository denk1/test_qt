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
    void run();
    void start();
private:
    //void setServer(server* inPtrServer);
    websocketpp::connection_hdl mHdl;
    std::thread mThread;
    server* mPtrServer;
    ITS* mPtrITS;
};

}
#endif
