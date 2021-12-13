#include <iostream>
#include <math.h>
#include <vector>

#include "../wolf/wolf.h"
#include "glm/fwd.hpp"
#include <imgui.h>

#include "Building.h"
#include "Grid.h"
#include "Plane.h"
#include "types.h"
#include "utils.h"

#define VPAIR_T std::pair<glm::vec3, glm::vec3>

int buildingIDStart = 8;

float Grid::bldgMinHeight = 15.0f;
float Grid::bldgMaxHeight = 175.f;

float Grid::centreX = 0.0f;
float Grid::centreZ = 0.0f;
float Grid::cellSideLength = 1.0f;

int Grid::extraCitadelBlockPaddingX = 0; // 0 means just centre is reserved
int Grid::extraCitadelBlockPaddingZ = 0;

float Grid::roadWidth = 15.0f * cellSideLength;
int Grid::blockWidth = 64; // in cells
int Grid::blockLength = 64; // in cells

int Grid::blockCornerSpecial = 16; // in cells

float Grid::blockWidthTrue = blockWidth * cellSideLength;
float Grid::blockLengthTrue = blockLength * cellSideLength;

float Grid::blockWidthOffsetTrue = -blockWidthTrue / 2.0f;
float Grid::blockLengthOffsetTrue = -blockLengthTrue / 2.0f;

float Grid::blockRoadWidthTrue = blockWidthTrue + roadWidth;
float Grid::blockRoadLengthTrue = blockLengthTrue + roadWidth;

std::vector<VPAIR_T> plots;

Grid::Grid(int length, int width)
{
    Building::setMinMaxHeight(bldgMinHeight, bldgMaxHeight);

    int numCitadelBlocks = (1 + 2 * extraCitadelBlockPaddingX) * (1 + 2 * extraCitadelBlockPaddingZ);

    // Both corners kept to allow for future changes
    glm::vec3 blockCorners[length * width * 2 /*- numCitadelBlocks*/]; // tl(x0, z0), br(x1, z1)
    glm::vec3 intersectionCorners[(length + 1) * (width + 1) * 2];
    glm::vec3 roadsGoingXCorners[(length + 1) * (width + 1) * 2];
    glm::vec3 roadsGoingZCorners[(length + 1) * (width + 1) * 2];

    int nBlocksValid = 0;
    int nIntersectionsValid = 0;
    int nRoadsGoingXValid = 0;
    int nRoadsGoingZValid = 0;

    int blockStartXIndex = -length / 2;
    int blockStartZIndex = -width / 2;
    int citadelBlockXIndex = 0;
    int citadelBlockZIndex = 0;
    int blockEndXIndex = length + blockStartXIndex;
    int blockEndZIndex = width + blockStartZIndex;

    glm::vec3 blockSpan = glm::vec3(blockWidthTrue, 0.0f, blockWidthTrue);
    glm::vec3 intersectionSpan = glm::vec3(roadWidth, 0.0f, roadWidth);
    glm::vec3 roadsGoingXSpan = glm::vec3(blockWidthTrue, 0.0f, roadWidth);
    glm::vec3 roadsGoingZSpan = glm::vec3(roadWidth, 0.0f, blockWidthTrue);

    float totalLength = length * blockRoadLengthTrue;
    float totalWidth = width * blockRoadWidthTrue;
    glm::vec3 ttl = glm::vec3(blockStartXIndex - totalLength / 2.0f, 0.0f, blockStartZIndex - totalWidth / 2.0f);
    glm::vec3 tbr = ttl + glm::vec3(totalLength, 0.0f, totalWidth);
    m_pConcrete = new Plane(ttl, tbr, CellType::CONCRETE, totalLength / 450.0f, totalWidth / 225.0f, -3.0f);

    float x;
    float z;

    for (int i = blockStartXIndex; i < blockEndXIndex; ++i) {
        x = blockWidthOffsetTrue + i * blockRoadWidthTrue; // block + road

        for (int k = blockStartZIndex; k < blockEndZIndex; ++k) {
            z = blockLengthOffsetTrue + k * blockRoadLengthTrue; // block + road

            if (-extraCitadelBlockPaddingX <= i && i < extraCitadelBlockPaddingX
                && -extraCitadelBlockPaddingZ <= k && k < extraCitadelBlockPaddingZ) {
                // In Citadel Exclusion Zone Inner Block (if there are any
            } else if ((i == extraCitadelBlockPaddingX) && -extraCitadelBlockPaddingZ <= k && k <= extraCitadelBlockPaddingZ) {
                // In Citadel Exclusion Zone Edge

                // Should be impossible
//                if (i == blockStartZIndex) { // Before first
//                    intersectionCorners[nIntersectionsValid * 2] = glm::vec3(x-blockRoadLengthTrue, 0.0f, z) + blockSpan;
//                    intersectionCorners[nIntersectionsValid * 2 + 1] = intersectionCorners[nIntersectionsValid * 2] + intersectionSpan;
//                    ++nIntersectionsValid; // Special
//                }

                if (k == blockStartZIndex) { // Before first
                    intersectionCorners[nIntersectionsValid * 2 + 1] = glm::vec3(x, 0.0f, z);
                    intersectionCorners[nIntersectionsValid * 2] = intersectionCorners[nIntersectionsValid * 2 + 1] - intersectionSpan;
                    ++nIntersectionsValid; // Special
                }

                intersectionCorners[nIntersectionsValid * 2] = glm::vec3(x, 0.0f, z) + blockSpan;
                intersectionCorners[nIntersectionsValid * 2 + 1] = intersectionCorners[nIntersectionsValid * 2] + intersectionSpan;
                ++nIntersectionsValid;

                // Should be impossible
//                if (i == blockStartXIndex) { // Before first
//                    roadsGoingZCorners[nRoadsGoingZValid * 2] = glm::vec3(x - roadWidth, 0.0f, z);
//                    roadsGoingZCorners[nRoadsGoingZValid * 2 + 1] = roadsGoingZCorners[nRoadsGoingZValid * 2] + roadsGoingZSpan;
//                    ++nRoadsGoingZValid; // Special
//                }

                roadsGoingZCorners[nRoadsGoingZValid * 2] = glm::vec3(x + blockWidthTrue, 0.0f, z);
                roadsGoingZCorners[nRoadsGoingZValid * 2 + 1] = roadsGoingZCorners[nRoadsGoingZValid * 2] + roadsGoingZSpan;
                ++nRoadsGoingZValid;

            } else if (-extraCitadelBlockPaddingX <= i && i <= extraCitadelBlockPaddingX && (k == extraCitadelBlockPaddingZ)) {
                // In Citadel Exclusion Zone Edge

                if (i == blockStartZIndex) { // Before first
                    intersectionCorners[nIntersectionsValid * 2] = glm::vec3(x-blockRoadLengthTrue, 0.0f, z) + blockSpan;
                    intersectionCorners[nIntersectionsValid * 2 + 1] = intersectionCorners[nIntersectionsValid * 2] + intersectionSpan;
                    ++nIntersectionsValid; // Special
                }

                // Should be impossible
//                if (k == blockStartZIndex) { // Before first
//                    intersectionCorners[nIntersectionsValid * 2 + 1] = glm::vec3(x, 0.0f, z);
//                    intersectionCorners[nIntersectionsValid * 2] = intersectionCorners[nIntersectionsValid * 2 + 1] - intersectionSpan;
//                    ++nIntersectionsValid; // Special
//                }

                intersectionCorners[nIntersectionsValid * 2] = glm::vec3(x, 0.0f, z) + blockSpan;
                intersectionCorners[nIntersectionsValid * 2 + 1] = intersectionCorners[nIntersectionsValid * 2] + intersectionSpan;
                ++nIntersectionsValid;

                // Should be impossible
//                if (k == blockStartZIndex) { // Before first
//                    roadsGoingXCorners[nRoadsGoingXValid * 2] = glm::vec3(x, 0.0f, z - roadWidth);
//                    roadsGoingXCorners[nRoadsGoingXValid * 2 + 1] = roadsGoingXCorners[nRoadsGoingXValid * 2] + roadsGoingXSpan;
//                    ++nRoadsGoingXValid; // Special
//                }

                roadsGoingXCorners[nRoadsGoingXValid * 2] = glm::vec3(x, 0.0f, z + blockWidthTrue);
                roadsGoingXCorners[nRoadsGoingXValid * 2 + 1] = roadsGoingXCorners[nRoadsGoingXValid * 2] + roadsGoingXSpan;
                ++nRoadsGoingXValid;

            } else {
                blockCorners[nBlocksValid * 2] = glm::vec3(x, 0.0f, z);
                blockCorners[nBlocksValid * 2 + 1] = blockCorners[nBlocksValid * 2] + blockSpan;
                ++nBlocksValid;

                if (i == blockStartZIndex) { // Before first
                    intersectionCorners[nIntersectionsValid * 2] = glm::vec3(x-blockRoadLengthTrue, 0.0f, z) + blockSpan;
                    intersectionCorners[nIntersectionsValid * 2 + 1] = intersectionCorners[nIntersectionsValid * 2] + intersectionSpan;
                    ++nIntersectionsValid; // Special
                }

                if (k == blockStartZIndex) { // Before first
                    intersectionCorners[nIntersectionsValid * 2 + 1] = glm::vec3(x, 0.0f, z);
                    intersectionCorners[nIntersectionsValid * 2] = intersectionCorners[nIntersectionsValid * 2 + 1] - intersectionSpan;
                    ++nIntersectionsValid; // Special
                }

                intersectionCorners[nIntersectionsValid * 2] = glm::vec3(x, 0.0f, z) + blockSpan;
                intersectionCorners[nIntersectionsValid * 2 + 1] = intersectionCorners[nIntersectionsValid * 2] + intersectionSpan;
                ++nIntersectionsValid;

                if (k == blockStartZIndex) { // Before first
                    roadsGoingXCorners[nRoadsGoingXValid * 2] = glm::vec3(x, 0.0f, z - roadWidth);
                    roadsGoingXCorners[nRoadsGoingXValid * 2 + 1] = roadsGoingXCorners[nRoadsGoingXValid * 2] + roadsGoingXSpan;
                    ++nRoadsGoingXValid; // Special
                }

                roadsGoingXCorners[nRoadsGoingXValid * 2] = glm::vec3(x, 0.0f, z + blockWidthTrue);
                roadsGoingXCorners[nRoadsGoingXValid * 2 + 1] = roadsGoingXCorners[nRoadsGoingXValid * 2] + roadsGoingXSpan;
                ++nRoadsGoingXValid;

                if (i == blockStartXIndex) { // Before first
                    roadsGoingZCorners[nRoadsGoingZValid * 2] = glm::vec3(x - roadWidth, 0.0f, z);
                    roadsGoingZCorners[nRoadsGoingZValid * 2 + 1] = roadsGoingZCorners[nRoadsGoingZValid * 2] + roadsGoingZSpan;
                    ++nRoadsGoingZValid; // Special
                }

                roadsGoingZCorners[nRoadsGoingZValid * 2] = glm::vec3(x + blockWidthTrue, 0.0f, z);
                roadsGoingZCorners[nRoadsGoingZValid * 2 + 1] = roadsGoingZCorners[nRoadsGoingZValid * 2] + roadsGoingZSpan;
                ++nRoadsGoingZValid;
            }
        }
    }

    // Make buildings
    for (int i = 0; i < nBlocksValid; ++i) {
        _fillBlock(blockCorners[i * 2], blockCorners[i * 2 + 1]);
    }

    for (int i = 0; i < nIntersectionsValid; ++i) {
        int tli = i * 2;
        int bri = tli + 1;
        m_planes.push_back(Plane(intersectionCorners[tli], intersectionCorners[bri], CellType::INTER));
    }

    for (int i = 0; i < nRoadsGoingXValid; ++i) {
        int tli = i * 2;
        int bri = tli + 1;
        m_planes.push_back(Plane(roadsGoingXCorners[tli], roadsGoingXCorners[bri], CellType::ROAD_X, 2.0f, 2.0f));
    }

    for (int i = 0; i < nRoadsGoingZValid; ++i) {
        int tli = i * 2;
        int bri = tli + 1;
        m_planes.push_back(Plane(roadsGoingZCorners[tli], roadsGoingZCorners[bri], CellType::ROAD_Z, 2.0f, 2.0f));
    }
}

void Grid::_fillBlock(const glm::vec3 &tl, const glm::vec3 &br)
{
    int w = 8, l = 16;

    int nX = blockLength / w;
    int nZ = blockWidth / w;

    for (int i = 0; i < nX; ++i) {
        glm::vec3 shift = glm::vec3(i * w * cellSideLength, 0.0f, 0.0f);
        glm::vec3 span = glm::vec3(w * cellSideLength, 0.0f, l * cellSideLength);

        _makeBldg(tl + shift, tl + shift + span);
        _makeBldg(br - shift - span, br - shift);
    }
    for (int j = l / w; j < nZ - l / w; ++j) {
        glm::vec3 shift = glm::vec3(0.0f, 0.0f, j * w * cellSideLength);
        glm::vec3 span = glm::vec3(l * cellSideLength, 0.0f, w * cellSideLength);

        _makeBldg(tl + shift, tl + shift + span);
        _makeBldg(br - shift - span, br - shift);
    }
}

void Grid::_makeBldg(const glm::vec3 &ntl, const glm::vec3 &nbr)
{
    if (rand() % 5 >= 3) {
        glm::vec3 nc = ntl + nbr;
        glm::vec3 center = glm::vec3(nc.x / 2.0f, nc.y / 2.0f, nc.z / 2.0f);
        m_buildings.push_back(Building(center, nbr.x - ntl.x, nbr.z - ntl.z));
    }
}

void Grid::render(glm::mat4 &mProj, const glm::mat4 &mView) const
{
    for (Building bldg : m_buildings) {
        bldg.render(mProj, mView);
    }

    for (Plane plane : m_planes) {
        plane.render(mProj, mView);
    }

    m_pConcrete->render(mProj, mView);
}

void Grid::renderImGui()
{
    if (!ImGui::CollapsingHeader("City Grid"))
        return;

    ImGui::Text("Num Buildings: %lu", m_buildings.size());
    if (ImGui::CollapsingHeader("Buildings")) {

        ImGui::DragFloat("Min Height", &bldgMinHeight);
        ImGui::DragFloat("Max Height", &bldgMaxHeight);
        ImGui::DragFloat("Height Spread", &Building::heightSpread);

        for (Building bldg : m_buildings) {
            bldg.renderImGui();
        }
    }

    ImGui::Separator();

    ImGui::Text("Num Planes: %lu", m_planes.size());
    if (ImGui::CollapsingHeader("Planes")) {
        for (Plane plane : m_planes) {
            plane.renderImGui();
        }
    }

    ImGui::Separator();

    if (ImGui::BeginTable("Member Variables", 3)) {
        int col = 0;

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragInt("extraCitadelBlockPaddingX", &extraCitadelBlockPaddingX);
        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragInt("extraCitadelBlockPaddingZ", &extraCitadelBlockPaddingZ);
        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragInt("blockCornerSpecial", &blockCornerSpecial);
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragInt("blockWidth", &blockWidth);
        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragInt("blockLength", &blockLength);
        ImGui::TableNextRow();

        float fltMin = -1000.0f;
        float fltMax = 1000.0f;
        float fltStep = 0.01f;

        ImGui::Button("Dummy");

        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("bldgMinHeight", &bldgMinHeight, fltStep, fltMin, fltMax, "%.3f");
        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("bldgMaxHeight", &bldgMaxHeight, fltStep, fltMin, fltMax, "%.3f");
        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("centreX", &centreX, fltStep, fltMin, fltMax, "%.3f");
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("centreZ", &centreZ, fltStep, fltMin, fltMax, "%.3f");
        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("cellSideLength", &cellSideLength, fltStep, fltMin, fltMax, "%.3f");
        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("roadWidth", &roadWidth, fltStep, fltMin, fltMax, "%.3f");
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("blockWidthTrue", &blockWidthTrue, fltStep, fltMin, fltMax, "%.3f");
        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("blockLengthTrue", &blockLengthTrue, fltStep, fltMin, fltMax, "%.3f");
        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("blockWidthOffsetTrue", &blockWidthOffsetTrue, fltStep, fltMin, fltMax, "%.3f");
        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("blockLengthOffsetTrue", &blockLengthOffsetTrue, fltStep, fltMin, fltMax, "%.3f");
        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("blockRoadWidthTrue", &blockRoadWidthTrue, fltStep, fltMin, fltMax, "%.3f");
        ImGui::TableSetColumnIndex((col++) % 3);
        ImGui::DragFloat("blockRoadLengthTrue", &blockRoadLengthTrue, fltStep, fltMin, fltMax, "%.3f");

        ImGui::EndTable();
    }
}
