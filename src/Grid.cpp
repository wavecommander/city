#include <algorithm>

#include "../thirdparty/imgui/imgui.h"
#include <glm/glm.hpp>

#include "Grid.h"

float Grid::Block::SubBlock::Cell::cellLen = 0.999f;
int Grid::Block::SubBlock::subblockLen = 32;
int Grid::Block::blockLen = 5;

Grid::Grid(int width, int height)
{
    float gridUnitLen = Block::blockLen * Block::SubBlock::subblockLen * Block::SubBlock::Cell::cellLen;
    float totalWidth = width * gridUnitLen;
    float totalHeight = height * gridUnitLen;

    for (int i = 0; i < height; ++i) {

        for (int j = 0; j < width; ++j) {
            m_blocks.push_back(Block());
        }
    }
}

Grid::~Grid() { }

void Grid::renderImGui()
{
    std::for_each(m_blocks.begin(), m_blocks.end(), [](Block block) {
        block.renderImGui();
    });
}

Grid::Block::Block()
{
    for (int i = 0; i < blockLen; ++i) {
        for (int j = 0; j < blockLen; ++j) {
            m_subBlocks.push_back(SubBlock());
        }
    }
}

Grid::Block::~Block() { }

void Grid::Block::renderImGui()
{
    std::for_each(m_subBlocks.begin(), m_subBlocks.end(), [](SubBlock subBlock) {
        subBlock.renderImGui();
    });
}

Grid::Block::SubBlock::SubBlock()
{
    for (int i = 0; i < subblockLen; ++i) {
        for (int j = 0; j < subblockLen; ++j) {
            m_cells.push_back(Cell());
        }
    }
}

Grid::Block::SubBlock::~SubBlock() { }

void Grid::Block::SubBlock::renderImGui()
{
    std::for_each(m_cells.begin(), m_cells.end(), [](Cell cell) {
        cell.renderImGui();
    });
}

Grid::Block::SubBlock::Cell::Cell() { }

Grid::Block::SubBlock::Cell::~Cell() { }

void Grid::Block::SubBlock::Cell::renderImGui()
{
}
