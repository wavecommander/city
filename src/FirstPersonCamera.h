#ifndef S_FPS_CAMERA_H
#define S_FPS_CAMERA_H

#include "../wolf/wolf.h"
#include <glm/glm.hpp>

class FirstPersonCamera
{
public:
    FirstPersonCamera(wolf::App *pApp, const glm::vec3 &position, const glm::vec3 &up, float rotX = DEF_ROT_X, float rotY = DEF_ROT_Y);
    ~FirstPersonCamera() { }

    void focusOn(const glm::vec3 &min, const glm::vec3 &max);
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjMatrix(int width, int height) const;
    std::string toString() const;

    void update(float dt);
    void updateMousePosition();
    void renderImGui();

private:
    void _limit_fov();
    void _processMouseMovement();
    void _processMouseScroll();
    void _processKeyboardMovement(float dt);
    void _updateCameraVectors();

    wolf::App *m_pApp;

    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_right = glm::vec3(0.0f, 0.0f, -1.0f);

    bool m_wasSprinting = false;

    static constexpr float
        DEF_ROT_Y = 0.0f,
        DEF_ROT_X = 0.0f,
        MAX_ROT = 89.9f,
        DEF_SPEED = 50.0f,
        SPRINT_FACTOR = 6.0f,
        SPRINT_FOV = 0.5f,
        MOUSE_SENS = 0.1f,
        DEF_FOV = 1.0f,
        FOV_STEP = 0.05f,
        MAX_FOV = 1.5f,
        MIN_FOV = 0.01f,
        NEAR_PLANE = 0.1f,
        FAR_PLANE = 100000.0f;

    float m_rotX = 0.0f;
    float m_rotY = 0.0f;
    float m_speed = DEF_SPEED;
    float m_mouseSens = MOUSE_SENS;
    glm::vec2 m_lastMousePos;
    float m_fov = DEF_FOV;
};

#endif // S_FPS_CAMERA_H
