#include <iostream>
#include <stdio.h>

#include <glm/glm.hpp>
#include "../thirdparty/imgui/imgui.h"
#include "../thirdparty/imgui/imgui_impl_glfw.h"
#include "../thirdparty/imgui/imgui_impl_opengl3.h"
#include "../wolf/wolf.h"
#include "../samplefw/SampleRunner.h"

#include "CitySample.h"

class City : public wolf::App
{
public:
    City()
        : wolf::App("Ben Boyle - City")
    {
        _initImGui();

        m_sampleRunner.addSample(new CitySample(this));

        for (int i = 0; i < NUM_KEYS; ++i) {
            m_pressedKeys[i] = false;
        }
    }

    ~City()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void update(float dt) override
    {
        _handleKeys();
        m_sampleRunner.update(dt);
    }

    void render() override
    {
        m_sampleRunner.render(m_width, m_height);
    }

private:
    SampleRunner m_sampleRunner;

    static constexpr size_t NUM_KEYS = 256;
    bool m_pressedKeys[NUM_KEYS];
    bool m_aKeyIsPressed = false;
    bool m_renderDebugUI = false;

    void _initImGui()
    {
        constexpr char const *GLSL_VER = "#version 150";
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(this->getWindow(), true);
        ImGui_ImplOpenGL3_Init(GLSL_VER);
    }

    void _handleKeys()
    {
        if (isKeyDown('r')) {
            m_pressedKeys['r'] = true;
            m_aKeyIsPressed = true;
        }
        if (isKeyDown(' ')) {
            m_pressedKeys[' '] = true;
            m_aKeyIsPressed = true;
        }

        if (m_aKeyIsPressed) {
            for (int i = 0; i < NUM_KEYS; ++i) {
                if (!m_pressedKeys[i])
                    continue;

                // R key to 'regenerate' new City
                if (i == 'r' && !isKeyDown('r')) {
                    m_sampleRunner.addSample(new CitySample(this));
                    m_sampleRunner.switchToLastSample();
                    m_pressedKeys[i] = false;
                    m_aKeyIsPressed = false;
                }
                // SPACE to switch between Cities
                if (i == ' ' && !isKeyDown(' ')) {
                    m_sampleRunner.nextSample();
                    m_pressedKeys[i] = false;
                    m_aKeyIsPressed = false;
                }
            }
        }
    }
};

int main(int, char **)
{
    City city;
    city.run();
}
