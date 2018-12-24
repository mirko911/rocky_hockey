#include "Tracker.h"

Tracker::Tracker()
{
    std::cout << "[Tracker] created tracker" << std::endl;

}

bool Tracker::Tick(const cv::Mat & src, cv::Mat &dst, Puck & puck)
{
    if (m_wrongDetections >= config.wrongDetectionThreshold) {
        //std::cerr << "[Tracker] Puck wasn't found for " + std::to_string(m_wrongDetections) + "frames. Reset puck status" << std::endl;
        puck.resetDirection();
        puck.resetPosition();
        puck.resetVelocity();
        m_wrongDetections = 0;
    }

	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::RNG rng;

	int i = 0;
	findContours(src, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE, cv::Point(0, 0));
	for (const auto &contour : contours) {

		cv::Moments moms = cv::moments(cv::Mat(contour));

		double area = moms.m00; //238, 254

		if (area < 154.0 || area > 283.0) {
			i++;
			continue;
		}

		double perimeter = cv::arcLength(contour, true);
		double roundness = (perimeter*perimeter) / (6.28*area);

		if (roundness > 5) {
			continue;
		}

		
		// Calculate object center
		// We are using 320x240 pix but we are going to output the 640x480 equivalent (*2)
		double posX = floor(moms.m10 / (double)area + 0.5); // round
		double posY = floor(moms.m01 / (double)area + 0.5);
		Vector position(static_cast<float>(posX), static_cast<float>(posY));
		cv::circle(dst, Vector2Point(position), 8, cv::Scalar(255, 0, 0), 1);


		//compute blob radius
		
			std::vector<double> dists;
			for (size_t pointIdx = 0; pointIdx < contour.size(); pointIdx++)
			{
				cv::Point2d pt = contour[pointIdx];
				dists.push_back(cv::norm(static_cast<cv::Point2d>(Vector2Point(position)) - pt));
			}
			std::sort(dists.begin(), dists.end());
			double radius = (dists[(dists.size() - 1) / 2] + dists[dists.size() / 2]) / 2.;
			puck.setRadius(static_cast<float>(radius));
			Config::get()->puckRadius = static_cast<float>(radius);

		std::cout << printVector(position) << std::endl;

		i++;


		Vector oldPosition = puck.getPosition();
		if (oldPosition.hasNaN()) {
			oldPosition = position;
			puck.setPosition(position);
		}

		//Calculate mean of all old directions and blend them with a 2:1 ratio into a new direction
		Vector direction = Vector((position - oldPosition));
		Vector oldDirection = puck.getDirection();
		if (oldDirection.hasNaN()) {
			oldDirection = direction;
		}
		Vector direction_blend = (direction + oldDirection) * 0.5f;
		float distance = direction.norm();

		//Pass new vectors to puck class
		if (distance >= 5.0f) {
			puck.setPosition(position);
			puck.setDirection(direction);
		}
		puck.setVelocity(distance);
	}


    
    return true;
}


Tracker::~Tracker()
{
}
