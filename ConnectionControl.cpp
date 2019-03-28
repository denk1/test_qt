#include "ConnectionControl.h"
#include "VehicleBase.h"

namespace RAT {

ConnectionControl::ConnectionControl(ITS *ptrITS):
    mPtrITS(ptrITS),
    isOpen(false)
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
    isOpen = true;
}

void ConnectionControl::on_close(connection_hdl hdl)
{
    std::cout << "on_close connection" << std::endl;
    isOpen = false;

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
        mPtrITS->getVehicle()->setCoeffForce(getParamValue("throttle_proc", data["params"]));
        mPtrITS->getVehicle()->processControl(VehicleBase::VSBackwardPressed);
    }
    else if(data["action"]=="stopping_up")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSBackwardReleased);
    }
    else if(data["action"]=="racing_down")
    {
        mPtrITS->getVehicle()->setCoeffForce(getParamValue("throttle_proc", data["params"]));
        mPtrITS->getVehicle()->processControl(VehicleBase::VSForwardPressed);
    }
    else if(data["action"]=="racing_up")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSForwardReleased);
    }
    else if(data["action"]=="turn_left_down")
    {
        mPtrITS->getVehicle()->setAimAngleSteering(getParamValue("steering_angle", data["params"]));
        mPtrITS->getVehicle()->processControl(VehicleBase::VSLeftPressed);
    }
    else if(data["action"]=="turn_left_up")
    {
        mPtrITS->getVehicle()->processControl(VehicleBase::VSLeftReleased);
    }
    else if(data["action"]=="turn_right_down")
    {
        mPtrITS->getVehicle()->setAimAngleSteering(getParamValue("steering_angle", data["params"]));
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

void ConnectionControl::on_timer(const websocketpp::lib::error_code &ec)
{
    if (ec) {
        // there was an error, stop telemetry
        mPtrServer->get_alog().write(websocketpp::log::alevel::app,
                "Timer Error: "+ec.message());
        return;
    }
    const Ogre::Vector3& v = mPtrITS->getVehicle()->getVehicleSN()->getPosition();
    if(0 < v.length())
    {
        std::cout << "x=" << mPtrITS->getVehicle()->getVehicleSN()->getPosition().x
                  << "y=" << mPtrITS->getVehicle()->getVehicleSN()->getPosition().y
                  << "z=" << mPtrITS->getVehicle()->getVehicleSN()->getPosition().z << std::endl;
    }
    std::ostringstream strSrm;
    strSrm << mPtrITS->getVehicle()->getSpeed();

    if(isOpen)
        mPtrServer->send(mHdl, strSrm.str().c_str(), websocketpp::frame::opcode::TEXT);
    set_timer(100);
}

void ConnectionControl::set_timer(int t)
{
    mPtrTimer = mPtrServer->set_timer(
                t,
                bind(
                    &ConnectionControl::on_timer,
                    this,
                    _1
                    ));

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
        set_timer(7000);
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

Ogre::Real ConnectionControl::getParamValue(Ogre::String nameParam, json arrayParams)
{
    int paramsSize = arrayParams.size();
    int require_value = 0;
    if(paramsSize > 0)
    {
        if(arrayParams.find(nameParam) != arrayParams.end())
        {
            require_value = arrayParams[nameParam];
        }
    }
    return require_value;
}

//void ConnectionControl::setServer(server *inPtrServer)
//{
//    mPtrServer = inPtrServer;
//}


}
