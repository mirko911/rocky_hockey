#include "Prediction.h"



Prediction::Prediction()
{
	auto config = Config::get();
    //We can skip the right wall, because it's the user side wall.
    //We don't need to predict the movement in this direction
    m_walls = {
        Wall{
            Line::Through(Vector(0,0), Vector(Config::get()->fieldWidth,0)), //TOP Wall
            Vector(0,0),
            Vector(Config::get()->fieldWidth,0),
            Vector(0, -1),
        },
        Wall{
            Line::Through(Vector(0, Config::get()->fieldHeight), Vector(Config::get()->fieldWidth, Config::get()->fieldHeight)), //BOTTOM Wall
            Vector(0, Config::get()->fieldHeight),
            Vector(Config::get()->fieldWidth, Config::get()->fieldHeight),
            Vector(0, 1),
        },
       // Wall{ //not sure if we need the third wall, because we should get an intersection with the defend
       // Line before we hit the wall
      //      Line::Through(Vector(0,0), Vector(0, Config::get()->fieldHeight)), //LEFT Wall
      //      Vector(1, 0),
       // },
    };

    //Defind line with random magic numbers 
    //@todo find a better way
    m_defendLine = {
        Line::Through(Vector(20, 0), Vector(20, Config::get()->fieldHeight)),
        Vector(20, 0),
        Vector(20, Config::get()->fieldHeight),
        Vector(1,0)
    };
}

void Prediction::tick(cv::Mat &dst, Puck & puck)
{
    //Check backward movement
    Vector directionMean = puck.getDirection();
    //std::cout << "DirMean [" << directionMean.x() << "," << directionMean.y() << "]" << std::endl;
	m_reflections.clear();
	m_reflections.reserve(3);

    if (directionMean.x() > 0 || directionMean.hasNaN()) {
        //std::cout << "[Prediction] Puck is moving backwards. Clear the history" << std::endl;
        //puck.resetPosition();
        puck.resetDirection();
        puck.resetVelocity();
        m_predictedPosition = Vector(NAN, NAN);
        m_predictionQueue.clear();
        return;
    }

    Vector intersection;

    std::cout << std::endl;

    //Let's do 3 bounces.
    //Stop when we hit the defendLine or after 3 bounces
    int i = 0;
    int j = 0;
    bool foundIntersection = false;
    while(i < 3 && !foundIntersection){
        //Redefine Vars
        Vector direction = puck.getDirection();
        Vector position = puck.getPosition();
        Vector trajectory = position + (direction * 1000);

       // std::cout << "Loop" << std::endl;
        j = 0;
        bool directIntersection = true; //used for debug output. I just want to check if we can correctly detect a direct hit
        //Check Wall Intersections
        for (const Wall &wall : m_walls) {
            j++;
			m_reflections.push_back({ position, direction });
            Line trajectoryLine = Line::Through(position, trajectory);
            cv::line(dst, Vector2Point(position), Vector2Point(trajectory), cv::Scalar(0, 0, 255), 1);

            //Check if we would hit the line without a wall intersection
            //We've to check this here, because the 'direct' intersection
            //could also happen after a wall reflection
            auto defendIntersection = m_defendLine.line.intersection(trajectoryLine);
            intersection = Vector(defendIntersection.x(), defendIntersection.y());
            if (!defendIntersection.hasNaN() && intersection.x() < position.x() && isInsideWall(m_defendLine, intersection)) {
                if (directIntersection) {
                    std::cout << "Found direct direction intersection" << printVector(intersection) << " " << printVector(position) << " " << printVector(direction) << " WallID" << j << std::endl;
                }
                else {
                    std::cout << "Found intersection" << printVector(intersection) << " " << printVector(position) << " " << printVector(direction) << " WallID" << j << std::endl;
                }
                m_predictionQueue.push_back(intersection);
                foundIntersection = true;
                break;
            }

            cv::line(dst, Vector2Point(wall.start), Vector2Point(wall.end), cv::Scalar(0, 255, 0), 1, 8);
            //Draw the intersection point
            cv::circle(dst, Vector2Point(intersection), 3, cv::Scalar(255, 255, 255), 3, 8, 0);
            directIntersection = false;

            //Check intersection beween puck and wall.
            //Also check if the intersection has the correct direction
            auto wallIntersection = wall.line.intersection(trajectoryLine);
            intersection = Vector(wallIntersection.x(), wallIntersection.y());
            if (intersection.hasNaN() || intersection.x() > position.x() || !isInsideWall(wall, intersection)) {
                std::cout << "Wrong intersection" << std::endl;
                continue;
            }

            std::cout << "Intersect on Wall " << j << std::endl;

            //r=d-2(d dot n)n
            //where d dot n is the dot product, and n must be normalized
            Vector reflection = direction - 2 * (direction.dot(wall.normal))*wall.normal;

            //Redefine Vars
            direction = reflection;
            position = intersection;
            trajectory = position + (direction * 100);
        }
        i++;
    }
    m_predictedPosition = getMean(m_predictionQueue);
    cv::circle(dst, Vector2Point(m_predictedPosition), 3, cv::Scalar(255, 255, 255), 3, 8, 0);

    for (const Wall &wall : m_walls) {
    }
    cv::line(dst, Vector2Point(m_defendLine.start), Vector2Point(m_defendLine.end), cv::Scalar(0, 255, 255), 1, 8);

}

void Prediction::setFieldSize(const cv::Size & size)
{
    //Adjust the wall positions, because we use the center of the circle for the
    //calculation. Therefore we need to subtract 0.5Radius from the wall.
    //Subtract -1, because we start with zero and not with 1
    int width = size.width - Config::get()->puckRadius - 1;
    int height = size.height - Config::get()->puckRadius -1;
    int zero = 0 + static_cast<int>(Config::get()->puckRadius * 0.5f);

    m_walls = {
           Wall{
               Line::Through(Vector(zero,zero), Vector(width,0)), //TOP Wall
               Vector(zero,zero),
               Vector(width,zero),
               Vector(0, -1),
           },
           Wall{
               Line::Through(Vector(zero, height), Vector(width, height)), //BOTTOM Wall
               Vector(zero, height),
               Vector(width, height),
               Vector(0, 1),
           },
           //not sure if we need the third wall, because we should get an intersection with the defend
           // Wall{ 
           //      Line::Through(Vector(0,0), Vector(0, Config::get()->fieldHeight)), //LEFT Wall
           //      Vector(1, 0),
           // },
    };

    //Defind line with random magic numbers 
    m_defendLine = {
        Line::Through(Vector(20, 0), Vector(20, size.height)),
        Vector(20, 0),
        Vector(20, size.height),
        Vector(1,0)
    };
}

std::vector<Reflection> Prediction::getReflections() const
{
	return m_reflections;
}

Vector Prediction::getPredictedPosition() const
{
	return m_predictedPosition;
}

std::array<Wall, 4> Prediction::getWalls() const
{
	return m_walls;
}

Wall Prediction::getDefendLine() const
{
	return m_defendLine;
}


Prediction::~Prediction()
{
}
