#include "RockyHockeyMain.h"

RockyHockeyMain::RockyHockeyMain(const std::string path) : m_caputreDevice(path)
{
    std::cout << "[RockyHockeyMain] using a file as capture device" << std::endl;
}

RockyHockeyMain::RockyHockeyMain(const int camID) : m_caputreDevice(camID)
{
    std::cout << "[RockyHockeyMain] using webcam as capture device" << std::endl;
}

RockyHockeyMain::RockyHockeyMain()
{
    std::cerr << "[RockyHockeyMain] using [nothing] as capture device" << std::endl;
    m_exit = true;
}

void RockyHockeyMain::Init()
{
    if (!m_caputreDevice.isOpened())
    {
        std::cerr << "[RockyHockeyMain] Can't open the video device" << std::endl;
        m_exit = true;
    }

    //Make sure that we have an image in imgSrc and imgDst,
    //because we need the size to initialize the displayMat
    m_caputreDevice >> m_imgSrc;
    if (m_imgSrc.empty()) {
        std::cerr << "[RockyHockeyMain] First frame is empty" << std::endl;
        m_exit = true;
    }
    m_imgSrc.copyTo(m_imgDst);

    m_workerThread = std::make_unique<std::thread>(&RockyHockeyMain::worker_thread, this);
}

void RockyHockeyMain::Run()
{
    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now;

    cv::namedWindow("display", cv::WINDOW_FREERATIO | cv::WINDOW_OPENGL);
    cv::Mat displayMat(m_imgSrc.size(), m_imgSrc.type());

    int key = -1;
    while (!m_exit)
    {
        key = cv::waitKey(1);
        if (key > -1)
        {
            onKeyPress(key);
        }

        {
            std::mutex m;
            std::lock_guard<std::mutex> lockGuard(m);
            m_imgDst.copyTo(displayMat);
        }

        cv::imshow("display", displayMat);

        /*
       ==================================================
         FPS Output starts here
       =================================================
       */
        now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() >= 1000000L)
        {
            std::cout << "FPS: " << m_FPS <<  std::endl;
            m_FPS = 0;
            lastTime = std::chrono::steady_clock::now();
        }
    }
}

void RockyHockeyMain::Fini()
{
    std::cout << std::endl << "[RockyHockeyMain] received exit signal" << std::endl;

    m_workerThread->join();
    std::cout << "[RockyHockeyWorker] stopping worker thread" << std::endl;
}

void RockyHockeyMain::worker_thread()
{
    std::cout << "[RockyHockeyWorker] started worker thread" << std::endl;
    std::chrono::steady_clock::time_point before;
    std::chrono::steady_clock::time_point after;
    std::chrono::duration<float> target_delta = std::chrono::seconds(1);
    std::chrono::duration<float> delta;

    // don't divide by zero
    float targetFPS = 30;
    if (targetFPS > 0)
        target_delta /= targetFPS;

    cv::Mat workingImage;

    while (!m_exit)
    {
        before = std::chrono::steady_clock::now();

        m_caputreDevice >> m_imgSrc;

        if (m_imgSrc.empty()) {
            std::cerr << "[RockyHockeyWorker] image is empty" << std::endl;
            continue;
        }

        cv::cvtColor(m_imgSrc, workingImage, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(workingImage, workingImage, cv::Size(3,3), 3, 3, 4);
        workingImage = workingImage > 127;
        m_tracker.Tick(workingImage, workingImage, m_puck);

       // cv::adaptiveThreshold(workingImage, workingImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 71, 18);

        {
            std::mutex m;
            std::lock_guard<std::mutex> lockGuard(m);
            workingImage.copyTo(m_imgDst);
        }

        /*
        ==================================================
          FPS Measuring and thread sleeping starts here
        =================================================
        */
        m_FPS++;

        after = std::chrono::steady_clock::now();
        // measure time it took to process the image
        delta = std::chrono::duration_cast<std::chrono::microseconds>(after - before);

        // if the frame was processed faster than necessary, wait the some time to reach the target frame rate
        // if targetFPS is 0, don't wait
        if (delta < target_delta && targetFPS > 0)
        {
            std::this_thread::sleep_for(target_delta - delta);
        }
    }
}

void RockyHockeyMain::onKeyPress(const int key)
{
    std::cout << "[RockyHockeyMain] onKeyPress. Detected key \"" << key << "\"" << std::endl;
    
    //@todo: implement
    switch (key)
    {
    case '27':
        m_exit = true;
        break;
    default:
        break;
    }
}

RockyHockeyMain::~RockyHockeyMain()
{
    Fini();
}
