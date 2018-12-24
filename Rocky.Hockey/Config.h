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
	float puckRadius = 8.0f;

	bool wrapImage = true;
	bool undistortImage = true;
	bool pauseStream = false;

	static std::shared_ptr<Config> get() {
		static std::shared_ptr<Config> d;
		if (d == 0) {
			d = std::make_shared<Config>();
		}
		return d;
	};
	Config() = default;
};

