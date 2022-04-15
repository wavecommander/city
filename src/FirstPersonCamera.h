#ifndef S_FPS_CAMERA_H
#define S_FPS_CAMERA_H

#include <glm/fwd.hpp>
#include "../wolf/wolf.h"

class FirstPersonCamera
{
public:
    FirstPersonCamera(wolf::App *pApp, const glm::vec3 &position, const glm::vec3 &up, float rotX = DEF_ROT_X, float rotY = DEF_ROT_Y, const glm::vec3 &minPosition = glm::vec3(-10000.0f,0.0f,-10000.0f), const glm::vec3 &maxPosition = glm::vec3(10000.0f,5000.0f,10000.0f));
    ~FirstPersonCamera() { }

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjMatrix(int width, int height) const;

    void update(float dt);
    void updateMousePosition();
    void renderImGui();

private:
    void _limitPosition();
    void _limitFOV();
    void _processMouseMovement();
    void _processMouseScroll();
    void _processKeyboardMovement(float dt);
    void _updateCameraVectors();

    static constexpr float
        DEF_ROT_X = 0.0f,
        DEF_ROT_Y = 0.0f,
        MAX_ROT = 89.9f,
        DEF_SPEED = 50.0f,
        SPRINT_FACTOR = 6.0f,
        SPRINT_FOV = 0.5f,
        MOUSE_SENS = 0.1f,
        DEF_FOV = 1.0f,
        FOV_STEP = 0.05f,
        MAX_FOV = 1.5f,
        MIN_FOV = 0.01f,
        NEAR_PLANE = 0.01f,
        FAR_PLANE = 3000.0f;

    float m_rotX = 0.0f;
    float m_rotY = 0.0f;
    float m_speed = DEF_SPEED;
    float m_mouseSens = MOUSE_SENS;
    glm::vec2 m_lastMousePos;
    float m_fov = DEF_FOV;

    float m_prior_FOV = DEF_FOV;
    bool m_wasSprinting = false;

    wolf::App *m_pApp;

    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_right = glm::vec3(0.0f, 0.0f, -1.0f);

    glm::vec3 m_minPosition = glm::vec3(-1000000000000.0f,-1000000000000.0f,-1000000000000.0f);
    glm::vec3 m_maxPosition = glm::vec3(1000000000000.0f,1000000000000.0f,1000000000000.0f);
};

#endif // S_FPS_CAMERA_H
