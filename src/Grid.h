#ifndef S_GRID_H
#define S_GRID_H

#include <set>
#include <vector>

#include "glm/glm.hpp"
#include "Plane.h"
#include "Building.h"

class Grid
{
public:
    Grid(int width, int height);

    void render(glm::mat4 &mProj, const glm::mat4 &mView) const;
    void renderImGui();

private:
    void _fillBlock(const glm::vec3 &tl, const glm::vec3 &br);
    void _makeBldg(const glm::vec3 &ntl, const glm::vec3 &nbr);

    static int blockLen;
    static int subblockLen;
    static float cellLen;
    static float realSubblockLen;

    static int maxInland;

    static float bldgMinArea;
    static float bldgMinHeight;
    static float bldgMaxHeight;

    static float centreX;
    static float centreZ;
    static float cellSideLength;

    static int extraCitadelBlockPaddingX; // 0 means just centre is reserved
    static int extraCitadelBlockPaddingZ;

    static float roadWidth;
    static int blockWidth; // in cells
    static int blockLength; // in cells

    static int blockCornerSpecial; // in cells

    static float blockWidthTrue;
    static float blockLengthTrue;

    static float blockWidthOffsetTrue;
    static float blockLengthOffsetTrue;

    static float blockRoadWidthTrue;
    static float blockRoadLengthTrue;

    std::vector<Building> m_buildings;
    std::vector<Plane> m_planes;
};

#endif // S_GRID_H
