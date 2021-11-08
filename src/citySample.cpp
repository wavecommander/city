#include <iostream>
#include <math.h>

#include "../thirdparty/irrKlang/include/irrKlang.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "citySample.h"
#include "plane.h"

bool showDemoWindow = false;

CitySample::~CitySample()
{
	printf("Destroying City Sample\n");
    wolf::MaterialManager::DestroyMaterial(m_pMat);
    delete m_pCitadel;
    delete m_pGrid;
    delete m_pOrbitCam;

    if(m_pAudioEngine)
        m_pAudioEngine->drop();
}

void CitySample::pressedKey(char key) {
    switch (key)
    {
        case 'd':
            m_renderDebugUI = !m_renderDebugUI;
            break;
    }
}

void CitySample::init()
{
    if(!m_pAudioEngine) {
        _initAudio();
    }

    if(!m_pCitadel)
    {
        m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        m_pCitadel = new wolf::Model("data/citadel_trimmed.fbx");
        m_pCitadelPiece = new wolf::Model("data/citadel_piece.fbx");
        // m_pCityGrid = new Plane();

        glm::vec3 min = m_pCitadel->getAABBMin();
        glm::vec3 max = m_pCitadel->getAABBMax();
        glm::vec3 center = m_pCitadel->getAABBCenter();

        m_pOrbitCam = new OrbitCamera(m_pApp);
        m_pOrbitCam->focusOn(min,max);

        std::cout << "MIN:X" << min.x << ",Y" << min.y << ",Z" << min.z << 
                   ";MAX:X" << max.x << ",Y" << max.y << ",Z" << max.z <<  '\n';

        float gridSize = 2.5f * wolf::max(max.x,max.z);
        m_pGrid = new Grid3D(10, gridSize / 10.0f);
        m_pGrid->hideAxes();
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
    
    ImGui::Begin("Debug Menu");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our window open/close state

    ImGui::ColorEdit3("glClearColor", (float*)&m_clearColor); // Edit 3 floats representing a color

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CitySample::update(float dt) 
{
    m_pOrbitCam->update(dt);
    m_pGrid->update(dt);
    m_pCitadelPiece->Update(dt);
    m_time += dt;
}

void CitySample::render(int width, int height)
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mProj = m_pOrbitCam->getProjMatrix(width, height);
	glm::mat4 mView = m_pOrbitCam->getViewMatrix();
  	glm::mat4 mWorld = glm::rotate(glm::mat4(1.0f), -PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorld = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 100.0f, 5.0f)) * mWorld;

    m_pGrid->render(mView, mProj);
    m_pCitadel->Render(mWorld, mView, mProj);

	glm::mat4 mWorldPiece = glm::rotate(glm::mat4(1.0f), -PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorldPiece = glm::rotate(glm::mat4(1.0f), PI/2.5f, glm::vec3(0.0f, 1.0f, 0.0f)) * mWorldPiece;
	mWorldPiece = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, _calculateHammer(m_time), -1000.0f)) * mWorldPiece;

    m_pCitadelPiece->Render(mWorldPiece, mView, mProj);

    if(m_renderDebugUI)
        _renderImGui();
}
