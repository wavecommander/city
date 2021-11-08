#include <stdio.h>
#include <iostream>

#include <glm/glm.hpp>
#include "../wolf/wolf.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "../samplefw/SampleRunner.h"
#include "citySample.h"

class City: public wolf::App
{
public:
    City() : wolf::App("Ben Boyle - City")
    {
        _initImGui();
        m_sampleRunner.addSample(new CitySample(this));
    }

    ~City()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void interpretKeys() {
        if(isKeyDown(' '))
        {
            m_pressedKey = ' ';
        } else if(isKeyDown('d')) {
            m_pressedKey = 'd';
        } else if(isKeyDown('r')) {
            m_pressedKey = 'r';
        }
        else if(m_pressedKey)
        {

            switch (m_pressedKey)
            {
            case 'r':
                m_sampleRunner.addSample(new CitySample(this));
                m_sampleRunner.nextSample();
                m_pressedKey = (char) NULL;
                break;
            case ' ':
                m_sampleRunner.nextSample();
                m_pressedKey = (char) NULL;
                break;
            default:
                m_sampleRunner.pressedKey(m_pressedKey);
                m_pressedKey = (char) NULL;
                break;
            }
        }
    }

    void update(float dt) override
    {
        interpretKeys();
        m_sampleRunner.update(dt);
    }

    void render() override
    {
        m_sampleRunner.render(m_width, m_height);
    }

private:
    SampleRunner m_sampleRunner;
    char m_pressedKey = (char) NULL;

    void _initImGui() {
        constexpr char const *GLSL_VER = "#version 150";
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(this->getWindow(), true);
        ImGui_ImplOpenGL3_Init(GLSL_VER);
    }
};

int main(int, char**) {
    City city;
    city.run();
}