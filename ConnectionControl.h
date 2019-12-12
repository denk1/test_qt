#ifndef CONNECTION_CONTROL_H
#define CONNECTION_CONTROL_H
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include "ConnectionControlInterface.h"


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

class ConnectionControl : public ConnectionControlInterface {
public:
    ConnectionControl(ITS* ptrITS);
    ~ConnectionControl();
    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void on_message(connection_hdl hdl, message_ptr msg);
    void on_timer(error_code const &ec);
    void run();

protected:
    Ogre::Real getParamValue(Ogre::String nameParam, json arrayParams);
    void set_timer(int i);
private:
    //void setServer(server* inPtrServer);
    typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;
    websocketpp::connection_hdl mHdl;
    con_list m_connections;
    std::thread mThread;
    server* mPtrServer;
    server::timer_ptr mPtrTimer;
};

}
#endif
