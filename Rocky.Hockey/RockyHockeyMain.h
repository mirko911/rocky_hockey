#pragma once

#include <string>
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
#include <memory>
#include <string>

#include <opencv2/opencv.hpp>

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/common/thread.hpp>

#include <cpp-json/json.h>

#include "definitions.h"
#include "Config.h"
#include "Puck.h"
#include "Tracker.h"
#include "ImageTransformation.h"
#include "Prediction.h"

typedef websocketpp::server<websocketpp::config::asio> server;
using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

class RockyHockeyMain 
    : public std::enable_shared_from_this<RockyHockeyMain>
{
private:
    typedef std::set<connection_hdl, std::owner_less<connection_hdl>> con_list;
    con_list m_websocketConnections;
    cv::VideoCapture m_captureDevice;
    cv::Mat m_imgSrc;
    cv::Mat m_imgDst;
    int m_FPS = 0;
    std::unique_ptr<std::thread> m_workerThread;
    std::unique_ptr<std::thread> m_workerWebsocket;

    Puck m_puck;
    Tracker m_tracker;
    server m_server;

    int m_frameCounter = 0;

    void startWebsocketServer();
	void sendWSHeartBeat();
public:
    bool m_exit = false;
    int cannyLow = 176;
    int cannyHigh = 216;
    int cannyMax = 1000;

    RockyHockeyMain(const std::string path);
    RockyHockeyMain(const int camID);
    RockyHockeyMain();
    void Init();
    void Run();
    void Fini();

    void worker_thread();
    void onKeyPress(const int key);
    void onWSOpen(connection_hdl hdl);
    void onWSClose(connection_hdl hdl);
    ~RockyHockeyMain();
};

