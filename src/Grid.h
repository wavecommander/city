#ifndef S_GRID_H
#define S_GRID_H

#include <vector>

#include <glm/glm.hpp>
#include "glm/fwd.hpp"
#include "../wolf/wolf.h"

struct Grid
{
    Grid(int width, int height);
    Grid(int width, int height, glm::vec3 &position);
    ~Grid();

    void render();
    void update(float dt);
    void renderImGui();

    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);

    struct Block {
        Block();
        ~Block();

        void render();
        void update(float dt);
        void renderImGui();

        static int blockLen;

        struct SubBlock {
            SubBlock();
            ~SubBlock();

            void render();
            void update(float dt);
            void renderImGui();

            static int subblockLen;

            struct Cell {
                Cell();
                ~Cell();

                void render();
                void update(float dt);
                void renderImGui();

                static float cellLen;
            };
            std::vector<Cell> m_cells;
        };
        std::vector<SubBlock> m_subBlocks;
    };
    std::vector<Block> m_blocks;
};

#endif // S_GRID_H
