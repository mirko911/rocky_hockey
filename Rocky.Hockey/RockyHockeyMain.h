#pragma once

#include <string>
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
#include <memory>

#include <opencv2/opencv.hpp>

#include "definitions.h"
#include "Puck.h"
#include "Tracker.h"

class RockyHockeyMain 
    : public std::enable_shared_from_this<RockyHockeyMain>
{
private:
    cv::VideoCapture m_caputreDevice;
    cv::Mat m_imgSrc;
    cv::Mat m_imgDst;
    int m_FPS = 0;
    std::unique_ptr<std::thread> m_workerThread;

    Puck m_puck;
    Tracker m_tracker;
public:
    bool m_exit = false;

    RockyHockeyMain(const std::string path);
    RockyHockeyMain(const int camID);
    RockyHockeyMain();
    void Init();
    void Run();
    void Fini();

    void worker_thread();
    void onKeyPress(const int key);
    ~RockyHockeyMain();
};

