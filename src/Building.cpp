#include <string>

#include "types.h"
#include "Building.h"

float Building::minHeight = 5.0f;
float Building::maxHeight = 1000.0f;

glm::vec3 Building::downtown = glm::vec3(0.0f,0.0f,0.0f);

constexpr GLfloat Building::VERTICES[];

Building::Building(const glm::vec3 &position, int length, int width)
    :m_length(length)
    ,m_width(width)
{
    m_height = _calculateHeight();

    m_position = glm::vec3(position.x, position.y + (m_height / 2.0f), position.z);

    Vertex vertices[NUM_VERTS];

    // texture for front, back, left, right faces
    int idx = 0;
    for(int i = 0; i < NUM_VERTS - 12; i += 6) {
        idx = i * 3;
        vertices[i] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, 0.0f};

        idx = (i+1) * 3;
        vertices[i+1] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, m_repeatV};

        idx = (i+2) * 3;
        vertices[i+2] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], m_repeatU, m_repeatV};

        idx = (i+3) * 3;
        vertices[i+3] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], m_repeatU, m_repeatV};

        idx = (i+4) * 3;
        vertices[i+4] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], m_repeatU, 0.0f};

        idx = (i+5) * 3;
        vertices[i+5] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, 0.0f};
    }


    // texture for top: gravel
    int ix = NUM_VERTS - 12;
    vertices[ix] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, 0.0f};

    idx += 3;
    vertices[ix+1] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, 1.0f};

    idx += 3;
    vertices[ix+2] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 1.0f, 1.0f};

    idx += 3;
    vertices[ix+3] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 1.0f, 1.0f};

    idx += 3;
    vertices[ix+4] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 1.0f, 0.0f};

    idx += 3;
    vertices[ix+5] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, 0.0f};


    // no texture for bottom
    ix = NUM_VERTS - 6;
    vertices[ix] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, 0.0f};

    idx += 3;
    vertices[ix+1] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, 0.0f};

    idx += 3;
    vertices[ix+2] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, 0.0f};

    idx += 3;
    vertices[ix+3] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, 0.0f};

    idx += 3;
    vertices[ix+4] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, 0.0f};

    idx += 3;
    vertices[ix+5] = Vertex {VERTICES[idx], VERTICES[idx+1], VERTICES[idx+2], 0.0f, 0.0f};


    m_pVB = wolf::BufferManager::CreateVertexBuffer(vertices, sizeof(Vertex) * NUM_VERTS);

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
	m_pDecl->SetVertexBuffer(m_pVB);
	m_pDecl->End();

    // setup material for main bldg texture
    std::string matName = "bldg_mat";
    std::string texPath = "data/bldg_tex";

    std::string choice = std::to_string(rand() % NUM_TEXTURES);
    texPath += choice + ".jpg";
    matName += choice;

    m_pTex1 = wolf::TextureManager::CreateTexture(texPath);
    m_pTex1->SetWrapMode(wolf::Texture::WM_Repeat);

    m_pMat = wolf::MaterialManager::CreateMaterial(matName);

    m_pMat->SetTexture("tex", m_pTex1);
    m_pMat->SetProgram("data/bldg_tex.vsh", "data/bldg_tex.fsh");
    m_pMat->SetDepthTest(true);
    m_pMat->SetDepthWrite(true);

    // setup material for
    m_pTex2 = wolf::TextureManager::CreateTexture(texPath);
    m_pTex2->SetWrapMode(wolf::Texture::WM_Repeat);
}

void Building::render(glm::mat4 &mProj, const glm::mat4 &mView) const
{
    glm::mat4 mWorld = glm::scale(glm::mat4(1.0f), glm::vec3(m_length, m_height, m_width));
    mWorld = glm::translate(mWorld, m_position);

    m_pMat->SetUniform("projection", mProj);
    m_pMat->SetUniform("view", mView);
    m_pMat->SetUniform("world", mWorld);
    m_pMat->SetTexture("tex", m_pTex1);
    m_pMat->Apply();

	m_pDecl->Bind();

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTS-12);

    // top gravel texture
    m_pMat->SetTexture("tex", m_pTex2);
    m_pMat->Apply();

	m_pDecl->Bind();

	glDrawArrays(GL_TRIANGLES, NUM_VERTS-12, 12);
}

void Building::renderImGui()
{

}

void Building::setMinMaxHeight(float minHeight, float maxHeight)
{
    Building::minHeight = minHeight;
    Building::maxHeight = maxHeight;
}

void Building::setDowntown(const glm::vec3 &downtown)
{
    Building::downtown = downtown;
}

float Building::_calculateHeight()
{
    float dist = glm::distance(m_position, downtown);
    return dist;
}
