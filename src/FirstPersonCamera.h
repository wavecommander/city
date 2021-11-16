#ifndef S_FPS_CAMERA_H
#define S_FPS_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../wolf/wolf.h"

static constexpr float
    ROT_Y = 0.0f,
    ROT_X = 0.0f,
    SPEED = 200.0f,
    MOUSE_SENS = 0.1f,
    FOV = 90.0f,
    FOV_STEP = 0.01f,
    MAX_FOV = 120.0f,
    MIN_FOV = 10.0f,
    NEAR_PLANE = 0.1f,
    FAR_PLANE = 100000.0f;

class FirstPersonCamera
{
public:
    FirstPersonCamera(wolf::App *pApp, glm::vec3 position = glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float rotY = ROT_Y, float rotX = ROT_X);
    ~FirstPersonCamera(){}

    void focusOn(const glm::vec3& min, const glm::vec3& max);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjMatrix(int width, int height);
    void update(float dt);

private:
    wolf::App *m_pApp;

    glm::vec3 m_position = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 m_front = glm::vec3(0.0f,0.0f,-1.0f);
    glm::vec3 m_up = glm::vec3(0.0f,1.0f,0.0f);
    glm::vec3 m_right = glm::vec3(0.0f,0.0f,-1.0f);

    float m_rotX = 0.0f;
    float m_rotY = 0.0f;
    float m_speed = SPEED;
    float m_mouseSens;
    glm::vec2 m_lastMousePos;
    float m_fov;

    bool m_wasSprinting;

    void _processMouseMovement();
    void _processMouseScroll();
    void _processKeyboardMovement(float dt);
    void _updateCameraVectors();
};

#endif // S_FPS_CAMERA_H
