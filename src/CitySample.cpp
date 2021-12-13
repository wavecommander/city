#include <glm/fwd.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Grid.h"
#include "Plane.h"
#include "Skybox.h"
#include "types.h"
#include "utils.h"
#include "CitySample.h"

int CitySample::gridLength = 15;
int CitySample::gridWidth = 15;

CitySample::CitySample(wolf::App *pApp)
    : Sample(pApp, "Cityscape")
{
}

CitySample::~CitySample()
{
    printf("Destroying City Sample\n");
    delete m_pCamera;

    delete m_pGrid;

    delete m_pCitadel;
    delete m_pCitadelPiece;

    if (m_pAudioEngine)
        m_pAudioEngine->drop();
}

void CitySample::init()
{
    if (!m_pCitadel) {
        m_renderDebugUI = false;

        //TODO: remove line; just for debugging
        m_pApp->setCaptureCursor(false);

        m_pCitadel = new wolf::Model("data/citadel_trimmed.fbx");
        m_pCitadelPiece = new wolf::Model("data/citadel_piece.fbx");
        _setCitadelToDefaultValues();

        // m_pHouse = new wolf::Model("data/house.fbx");
        // m_pHouse1 = new wolf::Model("data/house1.fbx");
        // m_pOffice = new wolf::Model("data/office.fbx");
        // m_pFactory = new wolf::Model("data/factor.fbx");
        // m_pSkyscraper = new wolf::Model("data/skyscraper.fbx");

        m_pCamera = new FirstPersonCamera(m_pApp, glm::vec3(200.0f, 320.0f, -100.0f), glm::vec3(0.0f, 1.0f, 0.0f), -36.0f, 156.0f, glm::vec3(-10000.0f, 0.1f, -10000.0f), glm::vec3(10000.0f, 10000.0f, 10000.0f));

        m_pGrid = new Grid(gridLength, gridWidth);

        glm::vec3 tl = glm::vec3(-10000.0f, -10000.0f, -10000.0f);
        m_pGrass = new Plane(tl, -tl, CellType::GRASS, tl.x / 100.0f, tl.x / 100.0f, -5.0f);

        m_pSkybox = new Skybox();
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

    ImGui::DragInt("Grid Length", &gridLength);
    ImGui::DragInt("Grid Width", &gridWidth);

    m_pCamera->renderImGui();

    ImGui::Separator();

    if (ImGui::CollapsingHeader("Citadel")) {
        ImGui::DragFloat("Scale", &m_citadelScale, 0.0001, 0.0f, 1.0f, "%.4f");
        ImGui::DragFloat("Total Period", &m_citadelTotalPeriod, 0.01, 0.0f, 10.0f, "%.2f");
        ImGui::DragFloat("Phase Shift", &m_citadelPhaseShift, 0.01, 0.0f, 2.0f, "%.2f");
        ImGui::DragFloat("Pulse Width", &m_citadelPulseWidth, 0.01, 0.0f, 10.0f, "%.2f");
        ImGui::DragFloat("Slam Fraction", &m_citadelSlamFraction, 0.01, 0.0f, 1.0f, "%.2f");
        if(ImGui::Button("Reset")) _setCitadelToDefaultValues();
    }

    ImGui::Separator();

    m_pGrid->renderImGui();

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

float CitySample::_citadelPiecePulse(float t) const
{
    if (t < m_citadelSlamFraction * m_citadelPulseWidth)
        return m_citadelStartingYValue + m_citadelTranslation * t / (m_citadelSlamFraction * m_citadelPulseWidth);
    return m_citadelStartingYValue + m_citadelTranslation * (m_citadelPulseWidth - t) / ((1.0 - m_citadelSlamFraction) * m_citadelPulseWidth);
}

float CitySample::_calculateCitadelPieceHammer() const
{
    float t = fmod(m_time + m_citadelPhaseShift, m_citadelTotalPeriod);
    if (t > (m_citadelTotalPeriod - m_citadelPulseWidth))
        return _citadelPiecePulse(t - m_citadelTotalPeriod + m_citadelPulseWidth);
    return m_citadelStartingYValue;
}

void CitySample::_setCitadelToDefaultValues()
{
    m_citadelScale = 0.01f;
    m_citadelTotalPeriod = 6.494; // Citadel should hammer down every 6.494s
    m_citadelPhaseShift = 1.0f; // Need offset to sync up with sound
    m_citadelStartingYValue = (23500.0f * m_citadelScale);
    m_citadelTranslation = (-7500.0f * m_citadelScale);
    m_citadelPulseWidth = 6.0f;
    m_citadelSlamFraction = 0.1f;
}

void CitySample::update(float dt)
{
    _handle_keys();

    if (!m_pAudioEngine)
        _initAudio(); // moved init here to deal with audio syncing problem; shouldn't be too bad with branch prediction

    m_time += dt;

    if (!m_renderDebugUI || (m_pApp->isKeyDown(341) || m_pApp->isKeyDown(345))) { // also probably not the best, but it works!
        m_pCamera->update(dt);
        //TODO: switch this back \|/ -> m_pApp->setCaptureCursor(true);
        m_pApp->setCaptureCursor(false);
    } else {
        m_pCamera->updateMousePosition(); // this fixes the camera angle changing wildly with the mouse pos upon toggling debug UI
        m_pApp->setCaptureCursor(false);
    }

    m_citadelStartingYValue = (23500.0f * m_citadelScale);
    m_citadelTranslation = (-7500.0f * m_citadelScale);
}

void CitySample::render(int width, int height)
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 mProj = m_pCamera->getProjMatrix(width, height);
    glm::mat4 mView = m_pCamera->getViewMatrix();

    // Render the Citadel, minus the moving piece
    glm::mat4 mWorldCitadel = glm::rotate(glm::mat4(1.0f), PI / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    mWorldCitadel = glm::scale(mWorldCitadel, glm::vec3(m_citadelScale, m_citadelScale, m_citadelScale));
    m_pCitadel->Render(mWorldCitadel, mView, mProj);

    // Render the moving piece of Citadel
    glm::mat4 mWorldCitadelPiece = glm::rotate(glm::mat4(1.0f), -PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorldCitadelPiece = glm::rotate(glm::mat4(1.0f), PI / 2.5f, glm::vec3(0.0f, 1.0f, 0.0f)) * mWorldCitadelPiece;
    mWorldCitadelPiece = glm::scale(mWorldCitadelPiece, glm::vec3(m_citadelScale, m_citadelScale, m_citadelScale));
    mWorldCitadelPiece = glm::translate(glm::mat4(1.0f), glm::vec3((250.0f * m_citadelScale), _calculateCitadelPieceHammer(), (-1000.0f * m_citadelScale))) * mWorldCitadelPiece;
    m_pCitadelPiece->Render(mWorldCitadelPiece, mView, mProj);


    // CODE FOR TRANSFOMRING IMPORTED MODELS
    // glm::mat4 mWorldHouse = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    // mWorldHouse = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 0.0f, 100.0f)) * mWorldHouse;
    // m_pHouse->Render(mWorldHouse, mView, mProj);

    // glm::mat4 mWorldHouse1 = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    // mWorldHouse1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * mWorldHouse1;
    // m_pHouse1->Render(mWorldHouse1, mView, mProj);

    // glm::mat4 mWorldOffice = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    // mWorldOffice = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * mWorldOffice;
    // m_pOffice->Render(mWorldOffice, mView, mProj);

    // glm::mat4 mWorldFactory = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    // mWorldFactory = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * mWorldFactory;
    // m_pFactory->Render(mWorldFactory, mView, mProj);

    // glm::mat4 mWorldSkyscraper = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    // mWorldSkyscraper = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * mWorldSkyscraper;
    // m_pSkyscraper->Render(mWorldSkyscraper, mView, mProj);

    m_pGrid->render(mProj, mView);

    m_pGrass->render(mProj, mView);

    m_pSkybox->render(mProj, mView);

    if (m_renderDebugUI)
        _renderImGui();
}
