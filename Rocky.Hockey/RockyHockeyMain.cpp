#include "RockyHockeyMain.h"

void RockyHockeyMain::startWebsocketServer()
{
	m_server.set_open_handler(bind(&RockyHockeyMain::onWSOpen, this, ::_1));
	m_server.set_close_handler(bind(&RockyHockeyMain::onWSClose, this, ::_1));

	std::cout << "[RockyHockeyWebsocket] Init websocket" << std::endl;

	m_server.init_asio();
	std::cout << "[RockyHockeyWebsocket] start listening on Port 9003" << std::endl;
	m_server.listen(9003);
	m_server.start_accept();

#ifndef DEBUG
	m_server.set_access_channels(websocketpp::log::alevel::none);
#endif // !DEBUG

	//m_server.run();

	std::cout << "[RockyHockeyWebsocket] started websocket thread" << std::endl;
	m_workerWebsocket = std::make_unique<std::thread>((std::bind(&server::run, &m_server)));
}

void RockyHockeyMain::sendWSHeartBeat()
{
	auto heartbeat = json::object{};
	heartbeat.insert("puck", json::object{
						{"position", toJson(m_puck.getPosition())},
						{"direction", toJson(m_puck.getDirection())}
	});

	json::object prediction;
	//Prediction
	json::array reflections;
	for (const auto& reflection : m_prediction.getReflections()) {
		reflections.append(json::object{
						{"position", toJson(reflection.position)},
						{"direction", toJson(reflection.direction)}
			});
	}
	prediction.insert("reflections", reflections);

	json::array walls;
	for (const auto& wall : m_prediction.getWalls()) {
		walls.append(json::object{
						{"start", toJson(wall.start)},
						{"end", toJson(wall.end)}
			});
	}
	prediction.insert("walls", walls);

	json::object defendLine = {
		{"start", toJson(m_prediction.getDefendLine().start)},
		{"end", toJson(m_prediction.getDefendLine().end)}
	};
	prediction.insert("defendLine", defendLine);
	prediction.insert("predictedPosition", toJson(m_prediction.getPredictedPosition()));

	heartbeat.insert("prediction", prediction);
	heartbeat.insert("settings", Config::get()->asJson());

	//std::cout << "[RockyHockeyWorker] Send websocket status" << std::endl;
	for (auto it : m_websocketConnections) {
		m_server.send(it, stringify(heartbeat), websocketpp::frame::opcode::text);
	}
}

RockyHockeyMain::RockyHockeyMain(const std::string path) : m_captureDevice(path)
{
    std::cout << "[RockyHockeyMain] using a file as capture device" << std::endl;
}

RockyHockeyMain::RockyHockeyMain(const int camID) : m_captureDevice(camID)
{
    std::cout << "[RockyHockeyMain] using webcam as capture device" << std::endl;
    m_captureDevice.set(cv::CAP_PROP_FPS, Config::get()->camFPS);
    m_captureDevice.set(cv::CAP_PROP_FRAME_WIDTH, Config::get()->camWidth);
    m_captureDevice.set(cv::CAP_PROP_FRAME_HEIGHT, Config::get()->camHeight);
}

RockyHockeyMain::RockyHockeyMain()
{
    std::cerr << "[RockyHockeyMain] using [nothing] as capture device" << std::endl;
    m_exit = true;
}

void RockyHockeyMain::Init()
{

    MotionController moCo("TEST", 9000);
    Vector position(50, 120);
    moCo.calculateRatio(cv::Size(320, 240));
    moCo.moveToPosition(position);

    if (!m_captureDevice.isOpened())
    {
        std::cerr << "[RockyHockeyMain] Can't open the video device" << std::endl;
        m_exit = true;
    }

    //Make sure that we have an image in imgSrc and imgDst,
    //because we need the size to initialize the displayMat
    m_captureDevice >> m_imgSrc;
    if (m_imgSrc.empty()) {
        std::cerr << "[RockyHockeyMain] First frame is empty" << std::endl;
        m_exit = true;
    }
    m_imgSrc.copyTo(m_imgDst);

    m_workerThread = std::make_unique<std::thread>(&RockyHockeyMain::worker_thread, this);

	startWebsocketServer();
}

void RockyHockeyMain::Run()
{
    std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point now;

    cv::namedWindow("display", cv::WINDOW_FREERATIO | cv::WINDOW_OPENGL);
    cv::Mat displayMat(m_imgSrc.size(), m_imgSrc.type());

    cv::createTrackbar("Canny Low", "display", &cannyLow, cannyMax);
    cv::createTrackbar("Canny High", "display", &cannyHigh, cannyMax);
    cv::createTrackbar("DP", "display", &m_tracker.dp, m_tracker.dpLimit);
    cv::createTrackbar("thres1", "display", &m_tracker.threshold1, m_tracker.thresholdLimit);
    cv::createTrackbar("thres2", "display", &m_tracker.threshold2, m_tracker.thresholdLimit);
    cv::createTrackbar("minArea", "display", &m_tracker.minArea, m_tracker.areaLimit);
    cv::createTrackbar("maxArea", "display", &m_tracker.maxArea, m_tracker.areaLimit);

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
	m_workerWebsocket->join();

    std::cout << "[RockyHockeyWorker] stopping worker threads" << std::endl;
}

void RockyHockeyMain::worker_thread()
{
    std::cout << "[RockyHockeyWorker] started worker thread" << std::endl;
    std::chrono::steady_clock::time_point before;
    std::chrono::steady_clock::time_point after;
    std::chrono::duration<float> target_delta = std::chrono::seconds(1);
    std::chrono::duration<float> delta;

    // don't divide by zero
    if (Config::get()->targetFPS > 0)
        target_delta /= static_cast<float>(Config::get()->targetFPS);

    std::string infoText = "";

    cv::Mat workingImage = cv::Mat::zeros(m_imgSrc.size(), CV_8UC1);
    cv::Mat undistImage = cv::Mat::zeros(m_imgSrc.size(), CV_8UC1);
    cv::Mat grayImage = cv::Mat::zeros(m_imgSrc.size(), CV_8UC1);
    cv::Mat wrapImage = cv::Mat::zeros(m_imgSrc.size(), CV_8UC1);
    cv::Mat debugImage = cv::Mat::zeros(m_imgSrc.size(), CV_8UC3);
    m_imgDst = cv::Mat::zeros(m_imgSrc.size(), CV_8UC3);

    m_prediction.setFieldSize(m_imageTransform.getFieldSize());

    while (!m_exit)
    {
        before = std::chrono::steady_clock::now();

        if (!Config::get()->pauseStream) {
            m_captureDevice >> m_imgSrc;
            m_frameCounter++;
        }

        if (m_imgSrc.empty()) {
            std::cerr << "[RockyHockeyWorker] image is empty" << std::endl;
            continue;
        }

        cv::cvtColor(m_imgSrc, grayImage, cv::COLOR_BGR2GRAY);

        infoText = "";
        if (Config::get()->undistortImage) {
            m_imageTransform.undistort(grayImage, undistImage);
            infoText = "Undistored ";
        }
        else {
            undistImage = grayImage.clone();
            infoText = "Distored ";
        }

        if (Config::get()->wrapImage) {
            m_imageTransform.warpPerspective(undistImage, wrapImage);
            infoText += " Wrapped";
        }
        else {
            wrapImage = undistImage.clone();
            infoText += " Unwrapped";
        }

        wrapImage.copyTo(debugImage);
        cvtColor(debugImage, debugImage, cv::COLOR_GRAY2BGR);


        cv::putText(debugImage, infoText, cv::Point2f(10, 10), cv::FONT_HERSHEY_PLAIN, 0.4, cv::Scalar(255, 255, 255, 255));


        workingImage = wrapImage;
        cv::GaussianBlur(workingImage, workingImage, cv::Size(5,5), 0, 0, 4);
        cv::Canny(workingImage, workingImage, cannyLow, cannyHigh);

		sendWSHeartBeat();

	
       // if(m_frameCounter % 4 == 0) {
            m_tracker.Tick(workingImage, debugImage, m_puck);
       // }
        m_prediction.tick(debugImage, m_puck);

        {
            std::mutex m;
            std::lock_guard<std::mutex> lockGuard(m);
            m_imgDst = debugImage.clone(); //cv::Mat::zeros(wrapImage.size(), CV_8UC3);
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
        if (delta < target_delta && Config::get()->targetFPS > 0)
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
    case 's':
        cv::imwrite("test_image.png", m_imgSrc);
        break;
    case 'u':
		Config::get()->undistortImage = !Config::get()->undistortImage;
        break;
    case 'w':
		Config::get()->wrapImage = !Config::get()->wrapImage;
        break;
    case 'p':
		Config::get()->pauseStream = !Config::get()->pauseStream;
    default:
        break;
    }
}

void RockyHockeyMain::onWSOpen(connection_hdl hdl)
{
    std::cout << "[Websocket] New Connection" << std::endl;
    m_websocketConnections.insert(hdl);
}

void RockyHockeyMain::onWSClose(connection_hdl hdl)
{
    m_websocketConnections.erase(hdl);
}

RockyHockeyMain::~RockyHockeyMain()
{
    Fini();
}
