#include <math.h>
#include <vector>

#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"
#include "../wolf/wolf.h"

#include "utils.h"
#include "Building.h"
#include "Road.h"
#include "Grid.h"

int Grid::blockLen = 5;
int Grid::subblockLen = 32;
float Grid::cellLen = 1.0f;
float Grid::realSubblockLen = subblockLen * cellLen;

int Grid::maxInland = subblockLen / 8;

float Grid::bldgMinArea = cellLen * 8;
float Grid::bldgMinHeight = 10.0f;
float Grid::bldgMaxHeight = 500.f;

Grid::Grid(int length, int width)
{
    if(subblockLen % 2 != 0) ++subblockLen;

    maxRecurse = 0;
    float f = (blockLen-1) * realSubblockLen / 2.0;
    while(f > bldgMinArea) {
        f /= 2;
        ++maxRecurse;
    }

    int tlbBlockLength = length * blockLen;
    int tlbBlockWidth = width * blockLen;
    float tlbUnitLen = blockLen * subblockLen * cellLen;

    int citadelStartY = tlbBlockWidth / 2;
    int citadelStartX = tlbBlockLength / 2;
    int citadelEndY = citadelStartY;
    int citadelEndX = citadelStartX;

    if(length % 2 == 0) {
        citadelStartX--;
    }
    if(width % 2 == 0) {
        citadelStartY--;
    }

    for(int tlbZ = 0; tlbZ < width; ++tlbZ) {
        float bZ = tlbZ * tlbUnitLen;
        for(int tlbX = 0; tlbX < length; ++tlbX) {
            float bX = tlbX * tlbUnitLen;

            if((citadelStartX <= tlbX && tlbX <= citadelEndX) && (citadelStartY <= tlbZ && tlbZ <= citadelEndY)) { // if block is reserved for citadel

            } else {
                _generateTLBRoads(bX, bZ);
                _generateTLBBldgs(bX, bZ);
            }
        }
    }
}

Grid::~Grid()
{

}

void Grid::_generateTLBRoads(float bX, float bZ)
{
    glm::vec3 rd0tl = glm::vec3(bX, 0.0f, bZ);
    glm::vec3 rd0br = rd0tl + glm::vec3((blockLen-1) * realSubblockLen, 0.0f, realSubblockLen);
    m_roads.push_back(Road(rd0tl, rd0br, false));

    glm::vec3 intertl = glm::vec3(rd0br.x, 0.0f, rd0tl.z);
    glm::vec3 interbr = intertl + glm::vec3(realSubblockLen, 0.0f, realSubblockLen);
    m_roads.push_back(Road(intertl, interbr, true));

    glm::vec3 rd1br = rd0br + glm::vec3(realSubblockLen, 0.0f, (blockLen-1) * realSubblockLen);
    m_roads.push_back(Road(rd0br, rd1br, false));
}

void Grid::_generateTLBBldgs(float bX, float bZ)
{
    glm::vec4 roadsDir = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    int level = 0;

    float totalLen = (blockLen-1) * realSubblockLen;
    float halfLen = totalLen / 2.0f;
    glm::vec3 middle = glm::vec3(bX + halfLen, 0.0f, bZ + realSubblockLen + halfLen);

    glm::vec3 b0tl = glm::vec3(bX, 0.0f, bZ + realSubblockLen);
    _fillSubsection(b0tl, middle, roadsDir, level);

    glm::vec3 b1tl = b0tl + glm::vec3(totalLen, 0.0f, 0.0f);
    _fillSubsection(b1tl, middle, roadsDir, level);

    glm::vec3 b2tl = b0tl + glm::vec3(0.0f, 0.0f, totalLen);
    _fillSubsection(b2tl, middle, roadsDir, level);

    glm::vec3 b3tl = b2tl + glm::vec3(totalLen, 0.0f, 0.0f);
    _fillSubsection(b3tl, middle, roadsDir, level);
}

void Grid::_fillSubsection(const glm::vec3 &tl, const glm::vec3 &br, const glm::vec4 & roadsDir, int level)
{
    float length = br.x - tl.x;
    float width = br.z - tl.z;

    float halfLength = length / 2.0f;
    float halfWidth = width / 2.0f;

    float area = length * width;

    if(area <= bldgMinArea) {
        glm::vec3 center = tl + glm::vec3(halfLength, 0.0f, halfWidth);
        m_buildings.push_back(Building(center, length, width, wolf::randFloat(bldgMinHeight, bldgMaxHeight)));
        return;
    }

    if(level >= maxRecurse) { // subdivide subsection into bldgs
        float plotSize = wolf::randFloat(sqrt(bldgMinArea), halfLength);
        int len = (int)(length / plotSize);

        int stZ, endZ, stX, endX;

        for(int i = stZ; i < endZ; ++i) {
            for(int j = stX; j < endX; ++j) {
                int inland = randInt(0, maxInland);

                glm::vec3 pos = glm::vec3();
                m_buildings.push_back(Building(pos, length, width, wolf::randFloat(bldgMinHeight, bldgMaxHeight)));
            }
        }

        return;
    }

    glm::vec3 ss0br = tl + glm::vec3(halfLength, 0.0f, halfWidth);
    glm::vec3 ss1tl, ss1br, ss2tl, ss2br, ss3tl, ss3br;

    //
    _fillSubsection(tl, ss0br, level+1);

    if(rand() % 2 == 0) {
        ss1tl = tl + glm::vec3(halfLength, 0.0f, 0.0f);
        ss1br = ss0br + glm::vec3(0.0f, 0.0f, halfLength);
    } else {

    }
    _fillSubsection(ss1tl, ss1br, level+1);
    _fillSubsection(ss2tl, ss2br, level+1);

}

void Grid::_fillssblock(const glm::vec3 &tl, const glm::vec3 &br)
{
}

void Grid::renderImGui()
{

}
