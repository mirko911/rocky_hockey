#include "Config.h"

json::object Config::asJson()
{
	return {
				{"cam", json::object{
						{"width", camWidth},
						{"height", camHeight},
						{"fps", camFPS}
					}
				},
				{"targetFPS", targetFPS},
				{"field", json::object{
						{"width", fieldWidth},
						{"height", fieldHeight}
					}
				},
				{"puckRadius", puckRadius},
				{
					"image", json::object{
						{"wrapImage", wrapImage},
						{"undistortImage", undistortImage},
						{"pauseStream", pauseStream}
					}
				}
	};
}
