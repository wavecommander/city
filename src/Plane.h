#ifndef S_PLANE_H
#define S_PLANE_H

#include <glm/fwd.hpp>
#include "../wolf/wolf.h"

#include "types.h"

class Plane
{
public:
    Plane(const glm::vec3 &topLeft, const glm::vec3 &bottomRight,  CellType type, float ru=1.0f, float rv=1.0f, float y=0.0f);
    ~Plane();

    void render(glm::mat4 &mProj, const glm::mat4 &mView) const;

private:
    static constexpr int NUM_VERTS = 3 * 2;

    wolf::VertexBuffer *m_pVB = nullptr;
    wolf::VertexDeclaration *m_pDecl = nullptr;
    wolf::Texture *m_pTex = nullptr;
    wolf::Material *m_pMat = nullptr;
};

#endif // S_PLANE_H
