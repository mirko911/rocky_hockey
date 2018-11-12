#include "Prediction.h"



Prediction::Prediction()
{
    m_walls = {
        Wall{
            Eigen::Vector2f(0, 0),
            Eigen::Vector2f(0, config.fieldWidth),
            Eigen::Vector2f(0, -1),
        },
        Wall{
            Eigen::Vector2f(0, 0),
            Eigen::Vector2f(0, config.fieldWidth),
            Eigen::Vector2f(0, -1),
        },
        Wall{
            Eigen::Vector2f(0, 0),
            Eigen::Vector2f(0, config.fieldWidth),
            Eigen::Vector2f(0, -1),
        },
        Wall{
            Eigen::Vector2f(0, 0),
            Eigen::Vector2f(0, config.fieldWidth),
            Eigen::Vector2f(0, -1),
        }
    };
}


Prediction::~Prediction()
{
}
