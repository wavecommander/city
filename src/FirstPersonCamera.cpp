#include <imgui.h>
#include "../wolf/wolf.h"

#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(wolf::App *pApp, const glm::vec3 &position, const glm::vec3 &up, float rotX, float rotY, const glm::vec3 &minPosition, const glm::vec3 &maxPosition)
    : m_pApp(pApp)
    , m_position(position)
    , m_up(up)
    , m_minPosition(minPosition)
    , m_maxPosition(maxPosition)
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

void FirstPersonCamera::_limitFOV()
{
    m_fov = wolf::min(m_fov, MAX_FOV);
    m_fov = wolf::max(m_fov, MIN_FOV);
}

void FirstPersonCamera::_limitPosition()
{
    m_position.x = wolf::min(m_position.x, m_maxPosition.x);
    m_position.x = wolf::max(m_position.x, m_minPosition.x);

    m_position.y = wolf::min(m_position.y, m_maxPosition.y);
    m_position.y = wolf::max(m_position.y, m_minPosition.y);

    m_position.z = wolf::min(m_position.z, m_maxPosition.z);
    m_position.z = wolf::max(m_position.z, m_minPosition.z);
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

    _limitFOV();
}

void FirstPersonCamera::_processKeyboardMovement(float dt)
{
    float velocity = m_speed * dt;

    if (m_pApp->isKeyDown(340) || m_pApp->isKeyDown(344)) // if right or left SHIFT, 'sprint'
    {
        velocity *= SPRINT_FACTOR;

        if (!m_wasSprinting) {
            m_prior_FOV = m_fov;
            m_fov += SPRINT_FOV;
            _limitFOV();
        }

        m_wasSprinting = true;

    } else {
        if (m_wasSprinting) {
            m_fov = m_prior_FOV;
            _limitFOV();
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

    _limitPosition();
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

    ImGui::InputFloat3("Position", &(m_position[0]));

    ImGui::Text("Rot X: %.1f Rot Y: %.1f", m_rotX, m_rotY);
    ImGui::Text("FOV %.2f", m_fov);
    ImGui::Text("Last Mouse Position: X:%.0f Y:%.0f", m_lastMousePos.x, m_lastMousePos.y);
}
