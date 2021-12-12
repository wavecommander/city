#ifndef S_GRID_H
#define S_GRID_H

#include <set>
#include <vector>

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "Road.h"
#include "Building.h"

class Grid
{
public:
    Grid(int width, int height);
    ~Grid();

    void renderImGui();

private:
    void _generateTLBRoads(float bX, float bZ);
    void _generateTLBBldgs(float bX, float bZ);

    // tl: top left
    // br: bottom right
    void _fillSubsection(const glm::vec3 &tl, const glm::vec3 &br, const glm::vec4 & roadsDir, int level);
    void _fillssblock(const glm::vec3 &tl, const glm::vec3 &br);

    static int blockLen;
    static int subblockLen;
    static float cellLen;
    static float realSubblockLen;

    static int maxInland;
    static int maxRecurse;

    static float bldgMinArea;
    static float bldgMinHeight;
    static float bldgMaxHeight;

    glm::vec3 m_position = glm::vec3(0.0f,0.0f,0.0f);

    std::vector<Building> m_buildings;
    std::vector<Road> m_roads;
};

#endif // S_GRID_H
