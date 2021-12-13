#ifndef S_PLANE_H
#define S_PLANE_H

#include <cstddef>
#include <glm/fwd.hpp>
#include "../wolf/wolf.h"

class Plane
{
public:
    enum class PLANE_TYPE {
        PT_ROAD,
        PT_INTER,
        PT_GRASS,
        PT_ASPHALT
    };

    Plane(const glm::vec3 &topLeft, const glm::vec3 &bottomRight,  PLANE_TYPE type);

    void render(glm::mat4 &mProj, const glm::mat4 &mView) const;
    void renderImGui();

private:
    static constexpr int NUM_VERTS = 3 * 2;

    float m_repeatU = 1.0f;
    float m_repeatV = 1.0f;

    wolf::VertexBuffer *m_pVB = nullptr;
    wolf::VertexDeclaration *m_pDecl = nullptr;
    wolf::Texture *m_pTex = nullptr;
    wolf::Material *m_pMat = nullptr;
};

#endif // S_PLANE_H
