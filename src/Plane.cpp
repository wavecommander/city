#include "glm/fwd.hpp"
#include "types.h"
#include "Plane.h"

Plane::Plane(const glm::vec3 &topLeft, const glm::vec3 &bottomRight,  PLANE_TYPE type)
{
    glm::vec3 bottomLeft = topLeft + glm::vec3(0.0f, 0.0f, (bottomRight.z - topLeft.z));
    glm::vec3 topRight = topLeft + glm::vec3((bottomRight.x - topLeft.x), 0.0f, 0.0f);

    if(type == PLANE_TYPE::PT_ROAD) {
        m_repeatU = 2.0f;
        m_repeatV = 2.0f;
    }

    Vertex vertices[] = {
        Vertex { bottomLeft.x, 0.0f, bottomLeft.z, 0.0f, 0.0f },
        Vertex { topLeft.x, 0.0f, topLeft.z, 0.0f, m_repeatV },
        Vertex { topRight.x, 0.0f, topRight.z, m_repeatU, m_repeatV },
        Vertex { topRight.x, 0.0f, topRight.z, m_repeatU, m_repeatV },
        Vertex { bottomRight.x, 0.0f, bottomRight.z, m_repeatU, 0.0f },
        Vertex { bottomLeft.x, 0.0f, bottomLeft.z, 0.0f, 0.0f },
    };
    m_pVB = wolf::BufferManager::CreateVertexBuffer(vertices, sizeof(Vertex) * NUM_VERTS);

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
	m_pDecl->SetVertexBuffer(m_pVB);
	m_pDecl->End();

    std::string matName = "";
    std::string texPath = "data/";

    switch (type) {
        case PLANE_TYPE::PT_ROAD:
        matName += "roadMat";
        texPath += "road_tex0.jpg";
        break;

        case PLANE_TYPE::PT_INTER:
        matName += "interMat";
        texPath += "inter_tex.png";
        break;

        case PLANE_TYPE::PT_GRASS:
        matName += "grassMat";
        if(rand() % 2 == 0) texPath += "grass_tex0.jpg";
        else texPath += "grass_tex1.jpg";
        break;

        case PLANE_TYPE::PT_ASPHALT:
        matName += "asphaltMat";
        texPath += "asphalt_tex.jpg";
        break;
    }

    m_pTex = wolf::TextureManager::CreateTexture(texPath);
    m_pTex->SetWrapMode(wolf::Texture::WM_Repeat);

    if(type == PLANE_TYPE::PT_ROAD) m_pTex->SetWrapMode(wolf::Texture::WM_MirroredRepeat);

    m_pMat = wolf::MaterialManager::CreateMaterial(matName);
    m_pMat->SetTexture("tex", m_pTex);
    m_pMat->SetProgram("data/plane.vsh", "data/plane.fsh");
    m_pMat->SetDepthTest(true);
    m_pMat->SetDepthWrite(true);
}

void Plane::render(glm::mat4 &mProj, const glm::mat4 &mView) const
{
    m_pMat->SetUniform("projection", mProj);
    m_pMat->SetUniform("view", mView);
    m_pMat->Apply();

	m_pDecl->Bind();

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTS);
}

void Plane::renderImGui()
{

}
