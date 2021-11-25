#include <iostream>
#include <sstream>
#include <string>

#include "../thirdparty/imgui/imgui.h"
#include "../wolf/wolf.h"

#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(wolf::App *pApp, const glm::vec3 &position, const glm::vec3 &up, float rotX, float rotY)
    : m_pApp(pApp)
    , m_position(position)
    , m_up(up)
    , m_rotX(rotX)
    , m_rotY(rotY)
    , m_front(glm::vec3(0.0f, 0.0f, -1.0f))
    , m_speed(DEF_SPEED)
    , m_mouseSens(MOUSE_SENS)
    , m_fov(DEF_FOV)
    , m_wasSprinting(false)
{
    updateMousePosition();
}

void FirstPersonCamera::_limit_fov()
{
    m_fov = wolf::min(m_fov, MAX_FOV);
    m_fov = wolf::max(m_fov, MIN_FOV);
}

void FirstPersonCamera::_processMouseMovement()
{
    glm::vec2 mousePos = m_pApp->getMousePos();
    glm::vec2 mouseMovement = (mousePos - m_lastMousePos) * m_mouseSens;
    m_rotY += mouseMovement.x;
    m_rotX -= mouseMovement.y;

    m_rotX = wolf::min(m_rotX, MAX_ROT);
    m_rotX = wolf::max(m_rotX, -MAX_ROT);
}

void FirstPersonCamera::_processMouseScroll()
{
    glm::vec2 mouseScroll = m_pApp->getMouseScroll();

    m_fov -= mouseScroll.y * FOV_STEP;

    _limit_fov();
}

void FirstPersonCamera::_processKeyboardMovement(float dt)
{
    float velocity = m_speed * dt;

    if (m_pApp->isKeyDown(340) || m_pApp->isKeyDown(344)) // if right or left SHIFT, 'sprint'
    {
        velocity *= SPRINT_FACTOR;

        if (!m_wasSprinting) {
            m_fov += SPRINT_FOV;
            _limit_fov();
        }

        m_wasSprinting = true;
    } else {
        if (m_wasSprinting) {
            m_fov -= SPRINT_FOV;
            _limit_fov();
            m_wasSprinting = false;
        }
    }

    if (m_pApp->isKeyDown('w')) {
        m_position += m_front * velocity;
    }
    if (m_pApp->isKeyDown('a')) {
        m_position -= m_right * velocity;
    }
    if (m_pApp->isKeyDown('s')) {
        m_position -= m_front * velocity;
    }
    if (m_pApp->isKeyDown('d')) {
        m_position += m_right * velocity;
    }
}

void FirstPersonCamera::_updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_rotY)) * cos(glm::radians(m_rotX));
    front.y = sin(glm::radians(m_rotX));
    front.z = sin(glm::radians(m_rotY)) * cos(glm::radians(m_rotX));
    m_front = glm::normalize(front);

    m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::mat4 FirstPersonCamera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 FirstPersonCamera::getProjMatrix(int width, int height) const
{
    return glm::perspective(m_fov, (float)width / (float)height, NEAR_PLANE, FAR_PLANE);
}

std::string FirstPersonCamera::toString() const
{
    std::ostringstream out;
    out.precision(4);

    out << "Pos:X" << m_position.x << ";Y" << m_position.y << ";Z" << m_position.z
        << "\nROTX:" << m_rotX << "; ROTY:" << m_rotY << "; FOV:" << m_fov
        << "\nLast Mouse Pos: X" << m_lastMousePos.x << ";Y" << m_lastMousePos.y
        << "\n";

    return out.str();
}

void FirstPersonCamera::update(float dt)
{
    _updateCameraVectors();
    _processMouseMovement();
    _processMouseScroll();
    _processKeyboardMovement(dt);
    updateMousePosition();
}

void FirstPersonCamera::updateMousePosition()
{
    m_lastMousePos = m_pApp->getMousePos();
}

void FirstPersonCamera::renderImGui()
{
    if (!ImGui::CollapsingHeader("Camera"))
        return;

    ImGui::Text("%s", this->toString().c_str());

    ImGui::InputFloat3("Position", &(m_position[0]));
}
