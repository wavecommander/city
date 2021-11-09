#ifndef S_PLANE_H
#define S_PLANE_H

#include <string>
#include <vector>

#include "../wolf/wolf.h"

class Plane {
public:
    Plane(GLuint shader, uint subdivisions, glm::vec3 position, float scale, glm::mat4 transformMatrix, glm::vec4 color, float rotationSpeed1, float rotationSpeed2, glm::vec3 rotationVector1, glm::vec3 rotationVector2);
    Plane(GLuint shader, uint subdivisions);
    ~Plane();

    void render(int width, int height, const glm::mat4 &mProj, const glm::mat4 &mView) const;
    void update(float dt);
    std::string toString(uint id) const;

    void setPosition(const glm::vec3 &position);
    void setScale(float scale);
    void setTransformMatrix(const glm::mat4 &transformMatrix);
    void setColor(const glm::vec4 &color);

    void setRotationSpeed1(float rotationSpeed);
    void setRotationSpeed2(float rotationSpeed);
    void setRotationVector1(const glm::vec3 &rotationVector);
    void setRotationVector2(const glm::vec3 &rotationVector);

    glm::vec3 getPosition() const;
    float getScale() const;
    glm::mat4 getTransformMatrix() const;
    glm::vec4 getColor() const;

    float getRotationSpeed1() const;
    float getRotationSpeed2() const;
    glm::vec3 getRotationVector1() const;
    glm::vec3 getRotationVector2() const;


private:
    uint m_subdivisions = 1;

    std::vector<GLfloat> m_vertices;
    void _generateVertices();

    static GLuint m_vao, m_vbo, m_shader;
    void _glInit(GLuint shader);

    glm::vec3 m_position = glm::vec3(0.0f,0.0f,0.0f);
    float m_scale = 1.0f;
    glm::mat4 m_transformMatrix = glm::mat4(1.0f);

    float m_time = 0.0f;

    float m_currentRotation = 0.0f;
    float m_rotationSpeed1 = 1.0f;
    float m_rotationSpeed2 = 1.0f;
    glm::vec3 m_rotationVector1 = glm::vec3(1.0f,0.0f,0.0f);
    glm::vec3 m_rotationVector2 = glm::vec3(0.0f,1.0f,0.0f);

    glm::vec4 m_color = glm::vec4(1.0f,0.0f,1.0f,1.0f);
};

#endif // S_PLANE_H
