#include <iostream>
#include <math.h>
#include <vector>

#include "glm/fwd.hpp"
#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_glfw.h"
#include "../thirdparty/imgui/imgui_impl_opengl3.h"
#include "../thirdparty/irrKlang/include/irrKlang.h"

#include "CitySample.h"
#include "Grid.h"
#include "utils.h"

CitySample::CitySample(wolf::App *pApp)
    : Sample(pApp, "Cityscape")
{
}

CitySample::~CitySample()
{
    printf("Destroying City Sample\n");
    wolf::MaterialManager::DestroyMaterial(m_pMat);
    delete m_pCitadel;
    delete m_pCitadelPiece;

    delete m_pCamera;

    if (m_pAudioEngine)
        m_pAudioEngine->drop();
}

void CitySample::init()
{
    if (!m_pCitadel) {
        m_renderDebugUI = false;

        m_clearColor = ImVec4(0.45f, 0.55f, 0.6f, 1.0f);

        m_pCitadel = new wolf::Model("data/citadel_trimmed.fbx");
        m_pCitadelPiece = new wolf::Model("data/citadel_piece.fbx");

        m_pCamera = new FirstPersonCamera(m_pApp, glm::vec3(200.0f, 320.0f, -100.0f), glm::vec3(0.0f, 1.0f, 0.0f), -36.0f, 156.0f);

        m_shader = wolf::LoadShaders("data/default.vsh", "data/default.fsh");
    }

    printf("Successfully initialized City Sample\n");
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

void CitySample::goToSleep()
{
    if (m_pAudioEngine) {
        m_pAudioEngine->drop();
        m_pAudioEngine = nullptr;
    }
    m_time = 0.0f;
}

static constexpr float
    TOTAL_PERIOD = 6.494, // Citadel should hammer down every 6.494s
    PHASE_SHIFT = 1.0f, // Need offset to sync up with sound
    DEFAULT_VALUE = 235.0f, // Start Y = 235.0f
    TRANSLATION = -75.0f, // End Y = 160.0f
    PULSE_WIDTH = 6.0f,
    SLAM_FRACTION = 0.1f,
    CITADEL_SCALE = 0.01f;

float CitySample::_citadelPiecePulse(float t)
{
    if (t < SLAM_FRACTION * PULSE_WIDTH)
        return DEFAULT_VALUE + TRANSLATION * t / (SLAM_FRACTION * PULSE_WIDTH);
    return DEFAULT_VALUE + TRANSLATION * (PULSE_WIDTH - t) / ((1.0 - SLAM_FRACTION) * PULSE_WIDTH);
}

float CitySample::_calculateCitadelPieceHammer()
{
    float t = fmod(m_time + PHASE_SHIFT, TOTAL_PERIOD);
    if (t > (TOTAL_PERIOD - PULSE_WIDTH))
        return _citadelPiecePulse(t - TOTAL_PERIOD + PULSE_WIDTH);
    return DEFAULT_VALUE;
}

void CitySample::_renderImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("City Sample Debug Menu");

    ImGui::Checkbox("Demo Window", &m_showDemoWindow);
    if (m_showDemoWindow)
        ImGui::ShowDemoWindow(&m_showDemoWindow);

    ImGui::Separator();

    ImGui::Text("m_time %f", m_time);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::ColorEdit3("glClearColor", (float *)&m_clearColor);

    ImGui::Separator();

    m_pCamera->renderImGui();

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CitySample::_handle_keys()
{
    // ` to toggle Debug UI
    if (m_pApp->isKeyDown('`')) {
        m_pressedKeys['`'] = true;
        m_aKeyIsPressed = true;
    }

    if (m_aKeyIsPressed) {
        for (int i = 0; i < NUM_KEYS; ++i) {
            if (!m_pressedKeys[i])
                continue;

            if (i == '`' && !m_pApp->isKeyDown(i)) {
                printf("Toggling Debug Menus ... ");
                m_renderDebugUI = !m_renderDebugUI;
                m_renderDebugUI ? printf("Enabled\n") : printf("Disabled\n");
                m_pressedKeys[i] = false;
                m_aKeyIsPressed = false;
            }
        }
    }
}

void CitySample::update(float dt)
{
    _handle_keys();

    if (!m_pAudioEngine)
        _initAudio(); // moved init here to deal with audio syncing problem; shouldn't be too bad with branch prediction

    m_time += dt;

    if (!m_renderDebugUI || (m_pApp->isKeyDown(341) || m_pApp->isKeyDown(345))) { // also probably not the best, but it works!
        m_pCamera->update(dt);
        m_pApp->setCaptureCursor(true);
    } else {
        m_pCamera->updateMousePosition(); // this fixes the camera angle changing wildly with the mouse pos upon toggling debug UI
        m_pApp->setCaptureCursor(false);
    }
}

void CitySample::render(int width, int height)
{
    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 mProj = m_pCamera->getProjMatrix(width, height);
    glm::mat4 mView = m_pCamera->getViewMatrix();

    // Render the Citadel, minus the moving piece
    glm::mat4 mWorldCitadel = glm::rotate(glm::mat4(1.0f), -PI / 2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorldCitadel = glm::scale(mWorldCitadel, glm::vec3(CITADEL_SCALE, CITADEL_SCALE, CITADEL_SCALE));
    m_pCitadel->Render(mWorldCitadel, mView, mProj);

    // Render the moving piece of Citadel
    glm::mat4 mWorldCitadelPiece = glm::rotate(glm::mat4(1.0f), -PI / 2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorldCitadelPiece = glm::scale(mWorldCitadelPiece, glm::vec3(CITADEL_SCALE, CITADEL_SCALE, CITADEL_SCALE));
    mWorldCitadelPiece = glm::rotate(glm::mat4(1.0f), PI / 2.5f, glm::vec3(0.0f, 1.0f, 0.0f)) * mWorldCitadelPiece;
    mWorldCitadelPiece = glm::translate(glm::mat4(1.0f), glm::vec3(2.50f, _calculateCitadelPieceHammer(), -10.0f)) * mWorldCitadelPiece;
    m_pCitadelPiece->Render(mWorldCitadelPiece, mView, mProj);

    if (m_renderDebugUI)
        _renderImGui();
}
