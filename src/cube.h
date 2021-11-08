#ifndef S_CUBE_H
#define S_CUBE_H

#include <string>
#include "../wolf/wolf.h"

class Cube {
public:
    Cube(GLuint shader, glm::vec3 position, float scale, glm::vec4 color, float rotationSpeed1, float rotationSpeed2, glm::vec3 rotationVector1, glm::vec3 rotationVector2);
    ~Cube();

    void render(int width, int height, const glm::mat4 &mProj, const glm::mat4 &mView) const;
    void update(float dt);
    std::string toString(uint id) const;

    void setPosition(const glm::vec3 &position);
    void setScale(float scale);
    void setColor(const glm::vec4 &color);

    void setRotationSpeed1(float rotationSpeed);
    void setRotationSpeed2(float rotationSpeed);
    void setRotationVector1(const glm::vec3 &rotationVector);
    void setRotationVector2(const glm::vec3 &rotationVector);

    glm::vec3 getPosition() const;
    float getScale() const;
    glm::vec4 getColor() const;

    float getRotationSpeed1() const;
    float getRotationSpeed2() const;
    glm::vec3 getRotationVector1() const;
    glm::vec3 getRotationVector2() const;


private:
    static GLuint m_vao, m_vbo, m_shader;

    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    float m_scale = 1.0f;

    float m_currentRotation = 0.0f;
    float m_rotationSpeed1 = 1.0f;
    float m_rotationSpeed2 = 1.0f;
    glm::vec3 m_rotationVector1 = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 m_rotationVector2 = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec4 m_color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

    static constexpr uint NUM_VERTS = 3 * 2 * 6;
    static constexpr GLfloat VERTICES[] = {

        // Front
        -0.5f, -0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        // Back
        0.5f,  0.5f,-0.5f,
        -0.5f,  0.5f,-0.5f,
        -0.5f, -0.5f,-0.5f,
        -0.5f, -0.5f,-0.5f,
        0.5f, -0.5f,-0.5f,
        0.5f,  0.5f,-0.5f,

        // Left
        -0.5f,  0.5f,-0.5f,
        -0.5f,  0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f,-0.5f,
        -0.5f,  0.5f,-0.5f,

        // Right
        0.5f,  0.5f, 0.5f,
        0.5f,  0.5f,-0.5f,
        0.5f, -0.5f,-0.5f,
        0.5f, -0.5f,-0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,

        // Top
        -0.5f,  0.5f, 0.5f,
        -0.5f,  0.5f,-0.5f,
        0.5f,  0.5f,-0.5f,
        0.5f,  0.5f,-0.5f,
        0.5f,  0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,

        // Bottom
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f,-0.5f,
        0.5f, -0.5f,-0.5f,
        -0.5f, -0.5f,-0.5f,
        -0.5f, -0.5f, 0.5f,
    };
};

#endif // S_CUBE_H
