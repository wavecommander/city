#ifndef S_BUILDING_H
#define S_BUILDING_H

#include <glm/fwd.hpp>
#include "../wolf/wolf.h"

class Building
{
public:
    Building(const glm::vec3 &position, int length, int width);
    ~Building();

    static void setMinMaxHeight(float minHeight, float maxHeight);
    static void setDowntown(const glm::vec3 &downtown);

    void render(glm::mat4 &mProj, const glm::mat4 &mView) const;
    void renderImGui();

    static float minHeight, maxHeight, heightSpread;

private:
    float _calculateHeight() const;

    static constexpr int NUM_VERTS = (3 * 2 * 6) - 6;
    static constexpr int ROOF_CUTOFF = NUM_VERTS - 6;
    static constexpr int NUM_TEXTURES = 3;

    float m_length;
    float m_width;
    float m_height;

    float m_repeatU = 0.33f;
    float m_repeatV = 3.0f;

    static glm::vec3 downtown;

    glm::vec3 m_position = glm::vec3(0.0f,0.0f,0.0f);

    wolf::VertexBuffer *m_pVB = nullptr;
    wolf::VertexDeclaration *m_pDecl = nullptr;
    wolf::Texture *m_pTex1 = nullptr;
    wolf::Texture *m_pTex2 = nullptr;
    wolf::Material *m_pMat = nullptr;

    static constexpr GLfloat VERTS[] = {

        // Front
        -0.5f, -0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        // Back
        0.5f,  0.5f,-0.5f,
        -0.5f,  0.5f,-0.5f,
        -0.5f, -0.5f,-0.5f,
        -0.5f, -0.5f,-0.5f,
        0.5f, -0.5f,-0.5f,
        0.5f,  0.5f,-0.5f,

        // Left
        -0.5f,  0.5f,-0.5f,
        -0.5f,  0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f,-0.5f,
        -0.5f,  0.5f,-0.5f,

        // Right
        0.5f,  0.5f, 0.5f,
        0.5f,  0.5f,-0.5f,
        0.5f, -0.5f,-0.5f,
        0.5f, -0.5f,-0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,

        // Top
        -0.5f,  0.5f, 0.5f,
        -0.5f,  0.5f,-0.5f,
        0.5f,  0.5f,-0.5f,
        0.5f,  0.5f,-0.5f,
        0.5f,  0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,

        // Bottom
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f,-0.5f,
        0.5f, -0.5f,-0.5f,
        -0.5f, -0.5f,-0.5f,
        -0.5f, -0.5f, 0.5f,
    };
};

#endif // S_BUILDING_H
