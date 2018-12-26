#pragma once
#include <memory>
#include <cpp-json/json.h>

class Config
{
public:
	int camWidth = 320;
	int camHeight = 240;
	int camFPS = 60;
	int targetFPS = 60;
	int fieldWidth = 320;
	int fieldHeight = 240;
	int wrongDetectionThreshold = 10;
	float puckRadius = 7.81f;

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

	json::object asJson();

	Config() = default;
};

