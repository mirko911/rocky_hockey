#pragma once

#include <string>
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>
#include <memory>
#include <string>

#include <opencv2/opencv.hpp>

#include "definitions.h"
#include "Config.h"
#include "Puck.h"
#include "Tracker.h"
#include "ImageTransformation.h"
#include "Prediction.h"

class RockyHockeyMain 
    : public std::enable_shared_from_this<RockyHockeyMain>
{
private:
    cv::VideoCapture m_captureDevice;
    cv::Mat m_imgSrc;
    cv::Mat m_imgDst;
    int m_FPS = 0;
    std::unique_ptr<std::thread> m_workerThread;

    Puck m_puck;
    Tracker m_tracker;

    int m_frameCounter = 0;
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
    ~RockyHockeyMain();
};

