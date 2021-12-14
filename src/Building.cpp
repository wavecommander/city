#include <string>

#include <imgui.h>

#include "glm/fwd.hpp"
#include "types.h"
#include "Building.h"

float Building::minHeight = 15.0f;
float Building::maxHeight = 180.0f;
float Building::heightSpread = 30.0f;

float Building::minUV = 1.0f;
float Building::maxUV = 1.75f;

glm::vec3 Building::downtown = glm::vec3(0.0f,0.0f,0.0f);

constexpr GLfloat Building::VERTS[];

Building::Building(const glm::vec3 &position, int length, int width)
    :m_length(length)
    ,m_width(width)
{
    _init(position);
}

void Building::_init(const glm::vec3 &position)
{
    m_position = glm::vec3(position.x, position.y, position.z);
    m_height = _calculateHeight();
    m_position.y += (m_height / 2.0f);

    // setup material for main bldg texture
    std::string matName = "bldg_mat";
    std::string texPath = "data/bldg_tex";

    int texID = rand() % NUM_TEXTURES;
    std::string texIDStr = std::to_string(texID);
    texPath += texIDStr + ".png";
    matName += texIDStr;

    //TODO: dynamically choose U V values for building dimensions and texture chosen
    switch (texID) {
        case 0:
        break;

        case 1:
        default:
        break;

        case 2:
        break;
    }

    m_pTex1 = wolf::TextureManager::CreateTexture(texPath);
    m_pTex1->SetWrapMode(wolf::Texture::WM_Repeat);

    m_pMat = wolf::MaterialManager::CreateMaterial(matName);

    m_pMat->SetTexture("tex", m_pTex1);
    m_pMat->SetProgram("data/bldg_tex.vsh", "data/bldg_tex.fsh");
    m_pMat->SetDepthTest(true);
    m_pMat->SetDepthWrite(true);

    // setup roof gravel texture
    m_pTex2 = wolf::TextureManager::CreateTexture("data/gravel_tex.png");
    m_pTex2->SetWrapMode(wolf::Texture::WM_Repeat);


    Vertex vertices[NUM_VERTS];
    m_uv = wolf::randFloat(minUV, maxUV);

    // texture for front, back, left, right faces
    int idx = 0;
    for(int i = 0; i < ROOF_CUTOFF; i += 6) {
        idx = i * 3;
        vertices[i] = Vertex {VERTS[idx], VERTS[idx+1], VERTS[idx+2], 0.0f, 0.0f};

        idx = (i+1) * 3;
        vertices[i+1] = Vertex {VERTS[idx], VERTS[idx+1], VERTS[idx+2], 0.0f, m_uv};

        idx = (i+2) * 3;
        vertices[i+2] = Vertex {VERTS[idx], VERTS[idx+1], VERTS[idx+2], m_uv, m_uv};

        idx = (i+3) * 3;
        vertices[i+3] = vertices[i+2];

        idx = (i+4) * 3;
        vertices[i+4] = Vertex {VERTS[idx], VERTS[idx+1], VERTS[idx+2], m_uv, 0.0f};

        idx = (i+5) * 3;
        vertices[i+5] = vertices[i];
    }


    // texture for top: gravel
    int st = ROOF_CUTOFF;
    idx = st * 3;
    vertices[st] = Vertex {VERTS[idx], VERTS[idx+1], VERTS[idx+2], 0.0f, 0.0f};

    idx += 3;
    vertices[st+1] = Vertex {VERTS[idx], VERTS[idx+1], VERTS[idx+2], 0.0f, 1.0f};

    idx += 3;
    vertices[st+2] = Vertex {VERTS[idx], VERTS[idx+1], VERTS[idx+2], 1.0f, 1.0f};

    idx += 3;
    vertices[st+3] = vertices[st+2];

    idx += 3;
    vertices[st+4] = Vertex {VERTS[idx], VERTS[idx+1], VERTS[idx+2], 1.0f, 0.0f};

    idx += 3;
    vertices[st+5] = vertices[st];

    m_pVB = wolf::BufferManager::CreateVertexBuffer(vertices, sizeof(Vertex) * NUM_VERTS);

	m_pDecl = new wolf::VertexDeclaration();
	m_pDecl->Begin();
	m_pDecl->AppendAttribute(wolf::AT_Position, 3, wolf::CT_Float);
	m_pDecl->AppendAttribute(wolf::AT_TexCoord1, 2, wolf::CT_Float);
	m_pDecl->SetVertexBuffer(m_pVB);
	m_pDecl->End();
}

void Building::render(glm::mat4 &mView, const glm::mat4 &mProj) const
{
    glm::mat4 mWorld = glm::scale(glm::mat4(1.0f), glm::vec3(m_length, m_height, m_width));
    mWorld = glm::translate(glm::mat4(1.0f), m_position) * mWorld;

    m_pMat->SetUniform("projection", mProj);
    m_pMat->SetUniform("view", mView);
    m_pMat->SetUniform("world", mWorld);
    m_pMat->SetTexture("tex", m_pTex1);
    m_pMat->Apply();

	m_pDecl->Bind();

	glDrawArrays(GL_TRIANGLES, 0, ROOF_CUTOFF);

    // top gravel texture
    m_pMat->SetTexture("tex", m_pTex2);
    m_pMat->Apply();

	m_pDecl->Bind();

	glDrawArrays(GL_TRIANGLES, ROOF_CUTOFF, 6);
}

void Building::_destroyAll() // dangerous, should only be used for quick dirty debugging
{
    delete m_pDecl;
    wolf::BufferManager::DestroyBuffer(m_pVB);
    wolf::TextureManager::DestroyTexture(m_pTex1);
    wolf::TextureManager::DestroyTexture(m_pTex2);
    wolf::MaterialManager::DestroyMaterial(m_pMat);
}

void Building::_reinit() // dangerous, should only be used for quick dirty debugging
{
        _destroyAll();
        _init(glm::vec3(m_position.x, m_position.y - (m_height / 2.0f), m_position.z));
}

void Building::renderImGui()
{
    float fltSpeed = 0.1;

    ImGui::DragFloat("Length", &m_length, fltSpeed, 0.0f);
    ImGui::DragFloat("Width", &m_width, fltSpeed, 0.0f);
    ImGui::DragFloat("Height", &m_height, fltSpeed, 0.0f);

    ImGui::DragFloat3("Position", &m_position.x, fltSpeed);

    ImGui::Separator();

    ImGui::Text("Properties that require reinitialization");

    ImGui::DragFloat("Min UV", &minUV, fltSpeed);
    ImGui::DragFloat("Max UV", &maxUV, fltSpeed);

    if(ImGui::Button("Reinitialze")) {
        _reinit();
    }
}

float Building::_calculateHeight() const
{
    float dist = glm::distance(m_position, downtown);
    float base = glm::clamp(maxHeight - dist, 0.0f, maxHeight);

    float higher = std::min(base + heightSpread, maxHeight);
    float lower = std::max(higher - heightSpread, minHeight);

    return wolf::randFloat(lower, higher);
}
