#include "types.h"
#include "Plane.h"

Plane::Plane(const glm::vec3 &topLeft, const glm::vec3 &bottomRight,  CellType type, float ru, float rv, float y)
{
    glm::vec3 tl = topLeft;
    glm::vec3 br = bottomRight;
    glm::vec3 bl = tl + glm::vec3(0.0f, 0.0f, (br.z - tl.z));
    glm::vec3 tr = tl + glm::vec3((br.x - tl.x), 0.0f, 0.0f);

    if(type == CellType::ROAD_Z) { // switch vectors around for texture coords to work out
        glm::vec3 tmp1, tmp2;      // could have done it the other way ... but this is what came to mind first
        tmp1 = bl;
        bl = tl;

        tmp2 = br;
        br = tmp1;

        tmp1 = tr;
        tr = tmp2;

        tl = tmp1;
    }

    Vertex vertices[] = {
        Vertex { bl.x, y, bl.z, 0.0f, 0.0f },
        Vertex { tl.x, y, tl.z, 0.0f, rv },
        Vertex { tr.x, y, tr.z, ru, rv },
        Vertex { tr.x, y, tr.z, ru, rv },
        Vertex { br.x, y, br.z, ru, 0.0f },
        Vertex { bl.x, y, bl.z, 0.0f, 0.0f },
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
        case CellType::ROAD_X:
        case CellType::ROAD_Z:
        matName += "roadMat";
        texPath += "road_tex.png";
        break;

        case CellType::INTER:
        matName += "interMat";
        texPath += "inter_tex.png";
        break;

        case CellType::GRASS:
        matName += "grassMat";
        texPath += "grass_tex.png";
        break;

        case CellType::CONCRETE:
        default:
        matName += "concreteMat";
        texPath += "concrete_tex.png";
        break;

        case CellType::ASPHALT:
        matName += "asphaltMat";
        texPath += "asphalt_tex.png";
        break;
    }

    m_pTex = wolf::TextureManager::CreateTexture(texPath);

    if(type == CellType::ROAD_X || type == CellType::ROAD_Z) {
        m_pTex->SetWrapMode(wolf::Texture::WM_MirroredRepeat);
    } else {
        m_pTex->SetWrapMode(wolf::Texture::WM_Repeat);
    }

    m_pMat = wolf::MaterialManager::CreateMaterial(matName);
    m_pMat->SetTexture("tex", m_pTex);
    m_pMat->SetProgram("data/plane.vsh", "data/plane.fsh");
    m_pMat->SetDepthTest(true);
    m_pMat->SetDepthWrite(true);
}

void Plane::render(glm::mat4 &mView, const glm::mat4 &mProj) const
{
    m_pMat->SetUniform("projection", mProj);
    m_pMat->SetUniform("view", mView);
    m_pMat->Apply();

    m_pDecl->Bind();

    glDrawArrays(GL_TRIANGLES, 0, NUM_VERTS);
}
