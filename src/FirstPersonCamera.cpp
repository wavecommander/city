#include "../wolf/wolf.h"

#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(wolf::App *pApp, glm::vec3 position, glm::vec3 up, float rotY, float rotX)
    : m_pApp(pApp)
    , m_position(position)
    , m_up(up)
    , m_rotX(rotX)
    , m_rotY(rotY)
    , m_front(glm::vec3(0.0f,0.0f,-1.0f))
    , m_speed(SPEED)
    , m_mouseSens(MOUSE_SENS)
    , m_fov(FOV)
    , m_wasSprinting(false)
{
    m_lastMousePos = m_pApp->getMousePos();
}

void FirstPersonCamera::_processMouseMovement()
{
    glm::vec2 mousePos = m_pApp->getMousePos();
    glm::vec2 mouseMovement = (mousePos - m_lastMousePos) * m_mouseSens;
    m_rotY += mouseMovement.x;
    m_rotX -= mouseMovement.y;

    if (m_rotX > 89.0f) m_rotX = 89.0f;
    if (m_rotX < -89.0f) m_rotX = -89.0f;
}

void FirstPersonCamera::_processMouseScroll()
{
    glm::vec2 mouseScroll = m_pApp->getMouseScroll();

    m_fov -= mouseScroll.y * FOV_STEP;
    if (m_fov < MIN_FOV) m_fov = MIN_FOV;
    if (m_fov > MAX_FOV) m_fov = MAX_FOV; 
}

void FirstPersonCamera::_processKeyboardMovement(float dt)
{
    float velocity = m_speed * dt;
    
    if(m_pApp->isKeyDown(340) || m_pApp->isKeyDown(344)) // if right or left SHIFT, 'sprint'
    {
        velocity *= 2;
    }

    if(m_pApp->isKeyDown('w'))
    {
        m_position += m_front * velocity;
    }
    if(m_pApp->isKeyDown('a'))
    {
        m_position -= m_right * velocity;
    }
    if(m_pApp->isKeyDown('s'))
    {
        m_position -= m_front * velocity;
    }
    if(m_pApp->isKeyDown('d'))
    {
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

    m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f,1.0f,0.0f)));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::mat4 FirstPersonCamera::getViewMatrix()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 FirstPersonCamera::getProjMatrix(int width, int height)
{
	return glm::perspective(m_fov, (float)width / (float)height, NEAR_PLANE, FAR_PLANE);
}

void FirstPersonCamera::update(float dt)
{
    _updateCameraVectors();
    _processMouseMovement();
    _processMouseScroll();
    _processKeyboardMovement(dt);

    m_lastMousePos = m_pApp->getMousePos();
}
