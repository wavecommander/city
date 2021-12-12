#ifndef S_BUILDING_H
#define S_BUILDING_H

#include <glm/fwd.hpp>
#include "../wolf/wolf.h"

class Building
{
public:
    Building(const glm::vec3 &position, int length, int width, float height);
    ~Building();

    static void setMinMaxHeight(float minHeight, float maxHeight);
    static void setDowntown(const glm::vec3 &downtown);

private:
    void _calculateHeight();

    static constexpr int NUM_VERTS = 3 * 2 * 6;

    int m_length;
    int m_width;
    float m_height;

    static float minHeight, maxHeight;

    static glm::vec3 downtown;

    glm::vec3 m_position = glm::vec3(0.0f,0.0f,0.0f);

    static wolf::VertexBuffer *pVB;
    static wolf::VertexDeclaration *pDecl;

    wolf::Material *m_pMat = nullptr;
    wolf::Program *m_pProgram = nullptr;
    wolf::Texture *m_pTex = nullptr;
};

#endif // S_BUILDING_H
