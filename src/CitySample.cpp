#include <iostream>
#include <math.h>
#include <vector>

#include "../thirdparty/irrKlang/include/irrKlang.h"
#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_glfw.h"
#include "../thirdparty/imgui/imgui_impl_opengl3.h"

#include "CitySample.h"
#include "Plane.h"

CitySample::CitySample(wolf::App* pApp) : Sample(pApp, "Cityscape") {}

CitySample::~CitySample()
{
	printf("Destroying City Sample\n");
    wolf::MaterialManager::DestroyMaterial(m_pMat);
    delete m_pCitadel;
    delete m_pCamera;

    if(m_pAudioEngine) m_pAudioEngine->drop();
}

void CitySample::init()
{
    if(!m_pAudioEngine) _initAudio();

    if(!m_pCitadel) {
        m_renderDebugUI = false;

        m_clearColor = ImVec4(0.45f,0.55f,0.6f,1.0f);
        m_planeColor = ImVec4(1.0f,0.7686f,0.0f,1.0f);

        m_pCitadel = new wolf::Model("data/citadel_trimmed.fbx");
        m_pCitadelPiece = new wolf::Model("data/citadel_piece.fbx");

        m_pCamera = new FirstPersonCamera(m_pApp);

        m_shader = wolf::LoadShaders("data/default.vsh", "data/default.fsh");

        m_pPlane = new Plane(m_shader, 10);
        m_pPlane->setPosition(glm::vec3(0.0f,0.0f,0.0f));
    }

    printf("Successfully initialized City Sample\n");
}

void CitySample::goToSleep()
{
    if(m_pAudioEngine) {
        m_pAudioEngine->drop();
        m_pAudioEngine = nullptr;
    }
    m_time = 0.0f;
}

void CitySample::setRenderDebugUI(bool renderDebugUI)
{
    m_renderDebugUI = renderDebugUI;
    if(m_renderDebugUI) m_pApp->setCaptureCursor(false);
    else m_pApp->setCaptureCursor(true);
}

void CitySample::_initAudio()
{
    m_pAudioEngine = irrklang::createIrrKlangDevice();
	if (!m_pAudioEngine) {
		printf("Could not startup Audio Engine\n");
        return;
    }
	m_pAudioEngine->play2D("data/citadel_alert_cut.ogg", true);
}

constexpr float TOTAL_PERIOD = 6.494, // Citadel should hammer down every 6.494s
                PHASE_SHIFT = 0.8f, // Need offset to sync up with sound
                DEFAULT_VALUE = 2350.0f, // Start Y = 2350.0f
                TRANSLATION = -750.0f,   // End Y = 1600.0f
                PULSE_WIDTH = 6.0f,
                SLAM_FRACTION = 0.1f,
                CITADEL_SCALE = 0.1f;

float CitySample::_citadelPiecePulse(float t)
{
    if (t < SLAM_FRACTION * PULSE_WIDTH)
        return DEFAULT_VALUE + TRANSLATION * t / (SLAM_FRACTION * PULSE_WIDTH);
    return DEFAULT_VALUE + TRANSLATION * (PULSE_WIDTH - t) / ((1.0 - SLAM_FRACTION) * PULSE_WIDTH);
}

float CitySample::_calculateCitadelPieceHammer(float time)
{
    float t = fmod(time + PHASE_SHIFT, TOTAL_PERIOD);
    if (t > (TOTAL_PERIOD - PULSE_WIDTH))
        return _citadelPiecePulse(t - TOTAL_PERIOD + PULSE_WIDTH);
    return DEFAULT_VALUE;
}

void CitySample::_renderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debug Menu");

    ImGui::Checkbox("Demo Window", &m_showDemoWindow);

    if (m_showDemoWindow) ImGui::ShowDemoWindow(&m_showDemoWindow);

    ImGui::ColorEdit3("glClearColor", (float*)&m_clearColor);
    ImGui::ColorEdit3("Plane Color", (float*)&m_planeColor);

    ImGui::Text("m_time %f", m_time);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CitySample::update(float dt) 
{    
    m_time += dt;

    if(!m_renderDebugUI) m_pCamera->update(dt);

    m_pCitadelPiece->Update(dt);
    m_pPlane->update(dt);
}

void CitySample::render(int width, int height)
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mProj = m_pCamera->getProjMatrix(width, height);
	glm::mat4 mView = m_pCamera->getViewMatrix();

  	glm::mat4 mWorldCitadel = glm::rotate(glm::mat4(1.0f), -PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorldCitadel = glm::scale(mWorldCitadel, glm::vec3(CITADEL_SCALE, CITADEL_SCALE, CITADEL_SCALE));
    m_pCitadel->Render(mWorldCitadel, mView, mProj);

	glm::mat4 mWorldCitadelPiece = glm::rotate(glm::mat4(1.0f), -PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorldCitadelPiece = glm::scale(mWorldCitadelPiece, glm::vec3(CITADEL_SCALE, CITADEL_SCALE, CITADEL_SCALE));
    mWorldCitadelPiece = glm::rotate(glm::mat4(1.0f), PI/2.5f, glm::vec3(0.0f, 1.0f, 0.0f)) * mWorldCitadelPiece;
	mWorldCitadelPiece = glm::translate(glm::mat4(1.0f), glm::vec3(25.0f, _calculateCitadelPieceHammer(m_time), -100.0f)) * mWorldCitadelPiece;
    m_pCitadelPiece->Render(mWorldCitadelPiece, mView, mProj);

    m_pPlane->setColor(glm::vec4(m_planeColor.x,m_planeColor.y,m_planeColor.z,m_planeColor.w));
    glm::mat4 mWorldPlane = glm::mat4(1.0f);
    m_pPlane->render(width, height, mProj, mView);

    if(m_renderDebugUI) _renderImGui();
}
