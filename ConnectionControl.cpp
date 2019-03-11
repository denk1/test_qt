#include "ConnectionControl.h"
#include "VehicleBase.h"

namespace RAT {

ConnectionControl::ConnectionControl(ITS *ptrITS):
    mPtrITS(ptrITS)
{

}

ConnectionControl::~ConnectionControl()
{
    if(mThread.joinable()) mThread.join();
}

void ConnectionControl::on_open(connection_hdl hdl)
{
    std::cout << "init connection" << std::endl;
    mHdl = hdl;
    server::connection_ptr con = mPtrServer->get_con_from_hdl(hdl);
    con->set_close_handshake_timeout(40000);
    con->set_open_handshake_timeout(40000);
    con->set_pong_timeout(40000);


}

void ConnectionControl::on_close(connection_hdl hdl)
{
    std::cout << "on_close connection" << std::endl;

}

void ConnectionControl::on_message(connection_hdl hdl, message_ptr msg)
{
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
    // recieve json-message from
    nlohmann::json data = nlohmann::json::parse(msg->get_payload());

    if(data["action"]=="stopping_down")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSBackwardPressed);
    }
    else if(data["action"]=="stopping_up")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSBackwardReleased);
    }
    else if(data["action"]=="racing_down")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSForwardPressed);
    }
    else if(data["action"]=="racing_up")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSForwardReleased);
    }
    else if(data["action"]=="turn_left_down")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSLeftPressed);
    }
    else if(data["action"]=="turn_left_up")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSLeftReleased);
    }
    else if(data["action"]=="turn_right_down")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSRightPressed);
    }
    else if(data["action"]=="turn_right_up")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSRightReleased);
    }
    else if(data["action"]=="hand_break_down")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSHandBreakPressed);
    }
    else if(data["action"]=="hand_break_up")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSHandBreakReleased);
    }
    // check for a special command to instruct the server to stop listening so
    // it can be cleanly exited.
    //    if (msg->get_payload() == "stop-listening") {
    //        mServer.stop_listening();
    //        return;
    //    }
}

void ConnectionControl::run()
{
    server mServer;
    try
    {
        // Set logging settings
        mServer.set_access_channels(websocketpp::log::alevel::all);
        mServer.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Set pointer of server
        mPtrServer = &mServer;

        // Initialize the Asio transport policy
        mServer.init_asio();

        // bind the handlers
        mServer.set_open_handler(bind(&ConnectionControl::on_open, this, _1));
        mServer.set_close_handler(bind(&ConnectionControl::on_close, this, _1));
        mServer.set_message_handler(bind(&ConnectionControl::on_message, this, _1, _2));

        mServer.listen(6789);
        mServer.start_accept();
        mServer.run();
    }
    catch(websocketpp::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch(...)
    {
        std::cout << "other exception" << std::endl;
    }
}

void ConnectionControl::start()
{
    mThread = std::thread(&ConnectionControl::run, this );
}

//void ConnectionControl::setServer(server *inPtrServer)
//{
//    mPtrServer = inPtrServer;
//}


}
