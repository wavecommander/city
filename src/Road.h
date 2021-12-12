#ifndef S_ROAD_H
#define S_ROAD_H

#include <glm/fwd.hpp>
#include "../wolf/wolf.h"

class Road
{
public:
    Road(const glm::vec3 &topLeft, const glm::vec3 &bottomRight,  bool intersection);
    ~Road();

private:
    static constexpr int NUM_VERTS = 3 * 2;

    static wolf::VertexBuffer *pVB;
    static wolf::VertexDeclaration *pDecl;

    static wolf::VertexBuffer *i_pVB;
    static wolf::VertexDeclaration *i_pDecl;

    wolf::Material *m_pMat = nullptr;
    wolf::Program *m_pProgram = nullptr;
    wolf::Texture *m_pTex = nullptr;
};

#endif // S_ROAD_H
