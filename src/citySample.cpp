#include <iostream>
#include <math.h>

#include "../thirdparty/irrKlang/include/irrKlang.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "citySample.h"
#include "plane.h"

bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

CitySample::~CitySample()
{
	printf("Destroying Model Import Sample\n");
    wolf::MaterialManager::DestroyMaterial(m_pMat);
    delete m_pCitadel;
    delete m_pGrid;
    delete m_pOrbitCam;

    m_pAudioEngine->drop();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void CitySample::init()
{
	// Only init if not already done
    if(!m_pCitadel)
    {
        constexpr char const *GLSL_VER = "#version 150";

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_pApp->getWindow(), true);
        ImGui_ImplOpenGL3_Init(GLSL_VER);

        m_pCitadel = new wolf::Model("data/citadel_trimmed.fbx");
        m_pCitadelPiece = new wolf::Model("data/citadel_piece.fbx");
        m_pCityGrid = new Plane();

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

        m_pAudioEngine = irrklang::createIrrKlangDevice();

	    if (!m_pAudioEngine)
	    {
	    	printf("Could not startup Audio Engine\n");
	    }

	    m_pAudioEngine->play2D("data/citadel_alert_cut.ogg", true);
    }

    printf("Successfully initialized Model Import Sample\n");
}

void CitySample::update(float dt) 
{
    m_pOrbitCam->update(dt);
    m_pGrid->update(dt);
    m_pCitadelPiece->Update(dt);
    m_rot += dt;
    m_hammer += dt;
}

void render_imgui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("ImGui Window 1");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void CitySample::render(int width, int height)
{
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 mProj = m_pOrbitCam->getProjMatrix(width, height);
	glm::mat4 mView = m_pOrbitCam->getViewMatrix();
  	glm::mat4 mWorld = glm::rotate(glm::mat4(1.0f), -PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorld = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 100.0f, 5.0f)) * mWorld;

    m_pGrid->render(mView, mProj);
    m_pCitadel->Render(mWorld, mView, mProj);

	glm::mat4 mWorldPiece = glm::rotate(glm::mat4(1.0f), -PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
    mWorldPiece = glm::rotate(glm::mat4(1.0f), PI/2.5f, glm::vec3(0.0f, 1.0f, 0.0f)) * mWorldPiece;
	mWorldPiece = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, _calc_hammer(m_hammer), -1000.0f)) * mWorldPiece; // Start Y = 23500.0f, End Y = 16000.0f

    m_pCitadelPiece->Render(mWorldPiece, mView, mProj);

    render_imgui();
}

constexpr float TOTAL_PERIOD = 6.494, // Citadel should hammer down every 6.494s
                PHASE_SHIFT = 1.0f,
                DEFAULT_VALUE = 23500.0f,
                TRANSLATION = -7500.0f,
                PULSE_WIDTH = 6.0f,
                SLAM_FRACTION = 0.1f;

float CitySample::_pulse(float t) {
    if (t < SLAM_FRACTION * PULSE_WIDTH) 
        return DEFAULT_VALUE + TRANSLATION * t / (SLAM_FRACTION * PULSE_WIDTH);
    return DEFAULT_VALUE + TRANSLATION * (PULSE_WIDTH - t) / ((1.0 - SLAM_FRACTION) * PULSE_WIDTH);
}

float CitySample::_calc_hammer(float time) {
    float t = fmod(time + PHASE_SHIFT, TOTAL_PERIOD);
    if (t > (TOTAL_PERIOD - PULSE_WIDTH))
        return _pulse(t - TOTAL_PERIOD + PULSE_WIDTH);
    return DEFAULT_VALUE;
}
