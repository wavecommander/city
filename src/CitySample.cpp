#include <iostream>
#include <math.h>

#include "../thirdparty/irrKlang/include/irrKlang.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "CitySample.h"
#include "Plane.h"

bool showDemoWindow = false;

CitySample::CitySample(wolf::App* pApp) : Sample(pApp, "Cityscape") {}

CitySample::~CitySample()
{
	printf("Destroying City Sample\n");
    wolf::MaterialManager::DestroyMaterial(m_pMat);
    delete m_pCitadel;
    delete m_pGrid;
    delete m_pOrbitCam;

    if(m_pAudioEngine) m_pAudioEngine->drop();
}

void CitySample::handleKeyPress(char key) {
    switch (key)
    {
        case '`':
            printf("Toggling Debug Menus ... ");
            m_renderDebugUI = !m_renderDebugUI;
            m_renderDebugUI ? printf("Enabled\n") : printf("Disabled\n");
            break;
        case 'w':
            printf("W is being pressed\n");
            break;
        case 'a':
            printf("A is being pressed\n");
            break;
        case 's':
            printf("S is being pressed\n");
            break;
        case 'd':
            printf("D is being pressed\n");
            break;
    }
}

void CitySample::init()
{
    if(!m_pAudioEngine) _initAudio();

    if(!m_pCitadel)
    {
        m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        m_pCitadel = new wolf::Model("data/citadel_trimmed.fbx");
        m_pCitadelPiece = new wolf::Model("data/citadel_piece.fbx");

        glm::vec3 min = m_pCitadel->getAABBMin();
        glm::vec3 max = m_pCitadel->getAABBMax();
        glm::vec3 center = m_pCitadel->getAABBCenter();

        m_pOrbitCam = new OrbitCamera(m_pApp);
        m_pOrbitCam->focusOn(min,max);
        // m_pOrbitCam->focusOn(glm::vec3(-1.0f,-1.0f,-1.0f),glm::vec3(1.0f,1.0f,1.0f));

        std::cout << "MIN:X" << min.x << ",Y" << min.y << ",Z" << min.z << 
                   ";MAX:X" << max.x << ",Y" << max.y << ",Z" << max.z <<  '\n';

        float gridSize = 2.5f * wolf::max(max.x,max.z);
        m_pGrid = new Grid3D(10, gridSize / 10.0f);
        m_pGrid->hideAxes();

        // m_pMat = wolf::MaterialManager::CreateMaterial("default");
        // m_pMat->SetProgram("default.vsh", "default.fsh");
        // m_pMat->SetDepthTest(true);

        m_shader = wolf::LoadShaders("data/default.vsh", "data/default.fsh");

        m_pPlane = new Plane(m_shader, 6);
        m_pPlane->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        m_pPlane->setColor(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
    }

    printf("Successfully initialized City Sample\n");
}

void CitySample::goToSleep() {
    if(m_pAudioEngine) {
        m_pAudioEngine->drop();
        m_pAudioEngine = nullptr;
    }
    m_time = 0.0f;
}

void CitySample::_initAudio() {
    m_pAudioEngine = irrklang::createIrrKlangDevice();
	if (!m_pAudioEngine) {
		printf("Could not startup Audio Engine\n");
        return;
    }
	m_pAudioEngine->play2D("data/citadel_alert_cut.ogg", true);
}

constexpr float TOTAL_PERIOD = 6.494, // Citadel should hammer down every 6.494s
                PHASE_SHIFT = 1.0f,
                DEFAULT_VALUE = 23500.0f, // Start Y = 23500.0f
                TRANSLATION = -7500.0f,   // End Y = 16000.0f
                PULSE_WIDTH = 6.0f,
                SLAM_FRACTION = 0.1f;

float CitySample::_pulse(float t) {
    if (t < SLAM_FRACTION * PULSE_WIDTH)
        return DEFAULT_VALUE + TRANSLATION * t / (SLAM_FRACTION * PULSE_WIDTH);
    return DEFAULT_VALUE + TRANSLATION * (PULSE_WIDTH - t) / ((1.0 - SLAM_FRACTION) * PULSE_WIDTH);
}

float CitySample::_calculateHammer(float time) {
    float t = fmod(time + PHASE_SHIFT, TOTAL_PERIOD);
    if (t > (TOTAL_PERIOD - PULSE_WIDTH))
        return _pulse(t - TOTAL_PERIOD + PULSE_WIDTH);
    return DEFAULT_VALUE;
}

void CitySample::_renderImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (showDemoWindow)
        ImGui::ShowDemoWindow(&showDemoWindow);

    ImGui::Begin("Debug Menu");

    ImGui::Checkbox("Demo Window", &showDemoWindow);

    ImGui::ColorEdit3("glClearColor", (float*)&m_clearColor);

    ImGui::Text("m_time %f", m_time);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CitySample::update(float dt) 
{
    m_time += dt;
    m_pOrbitCam->update(dt);
    m_pGrid->update(dt);
    m_pCitadelPiece->Update(dt);
    m_pPlane->update(dt);
}

void CitySample::render(int width, int height)
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mProj = m_pOrbitCam->getProjMatrix(width, height);
	glm::mat4 mView = m_pOrbitCam->getViewMatrix();

    m_pGrid->render(mView, mProj);

  	glm::mat4 mWorldCitadel = glm::rotate(glm::mat4(1.0f), -PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorldCitadel = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 100.0f, 5.0f)) * mWorldCitadel;
    m_pCitadel->Render(mWorldCitadel, mView, mProj);

	glm::mat4 mWorldCitadelPiece = glm::rotate(glm::mat4(1.0f), -PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorldCitadelPiece = glm::rotate(glm::mat4(1.0f), PI/2.5f, glm::vec3(0.0f, 1.0f, 0.0f)) * mWorldCitadelPiece;
	mWorldCitadelPiece = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, _calculateHammer(m_time), -1000.0f)) * mWorldCitadelPiece;
    m_pCitadelPiece->Render(mWorldCitadelPiece, mView, mProj);

    glm::mat4 mWorldPlane = glm::mat4(1.0f);
    m_pPlane->render(width, height, mProj, mView);

    if(m_renderDebugUI) _renderImGui();
}
