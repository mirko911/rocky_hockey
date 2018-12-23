#pragma once
#include <memory>

class Config
{
public:
	int camWidth = 320;
	int camHeight = 240;
	int camFPS = 120;
	int targetFPS = 10;
	int fieldWidth = 320;
	int fieldHeight = 240;
	int wrongDetectionThreshold = 10;
	int puckRadius = 8;

	bool wrapImage = true;
	bool undistortImage = true;
	bool pauseStream = false;

	static std::shared_ptr<Config> get() {
		static std::shared_ptr<Config> d;
		return d;
	};
	Config() = default;
};

