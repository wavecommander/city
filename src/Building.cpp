#include "Building.h"
#include "glm/fwd.hpp"

float Building::minHeight = 5.0f;
float Building::maxHeight = 1000.0f;

glm::vec3 Building::downtown = glm::vec3(0.0f,0.0f,0.0f);

Building::Building(const glm::vec3 &position, int length, int width, float height)
{
    // wolf::BufferManager::CreateVertexBuffer();
}

Building::~Building()
{
}

void Building::setMinMaxHeight(float minHeight, float maxHeight)
{
    Building::minHeight = minHeight;
    Building::maxHeight = maxHeight;
}

void Building::setDowntown(const glm::vec3 &downtown)
{
    Building::downtown = downtown;
}

void Building::_calculateHeight()
{
    float dist = (downtown - m_position).length();
}
