#ifndef S_PLANE_H
#define S_PLANE_H

#include <string>
#include <vector>

#include "../wolf/wolf.h"

class Plane {
public:
    Plane(GLuint shader, uint subdivisions, glm::vec3 position, float scale, glm::mat4 transform_mat, glm::vec4 color, float rot_speed1, float rot_speed2, glm::vec3 rot_vec1, glm::vec3 rot_vec2);
    Plane(GLuint shader, uint subdivisions);
    ~Plane();

    void render(int width, int height, const glm::mat4 &mProj, const glm::mat4 &mView) const;
    void update(float dt);
    std::string to_string(uint id) const;

    void setPosition(const glm::vec3 &position);
    void setScale(float scale);
    void setTransformMatrix(const glm::mat4 &transform_mat);
    void setColor(const glm::vec4 &color);

    void setRotationSpeed1(float rot_speed);
    void setRotationSpeed2(float rot_speed);
    void setRotationVector1(const glm::vec3 &rot_vec);
    void setRotationVector2(const glm::vec3 &rot_vec);

    glm::vec3 getPosition() const;
    float getScale() const;
    glm::mat4 getTransformMatrix() const;
    glm::vec4 getColor() const;

    float getRotationSpeed1() const;
    float getRotationSpeed2() const;
    glm::vec3 getRotationVector1() const;
    glm::vec3 getRotationVector2() const;


private:
    static float randomFloat(float lower, float upper);

    uint m_Subdivisions = 1;

    std::vector<GLfloat> vertices;
    void generateVertices();

    static GLuint m_Vao, m_Vbo, m_Shader;
    void glInit(GLuint shader);

    glm::vec3 m_Position = glm::vec3(0.0f,0.0f,0.0f);
    float m_Scale = 1.0f;
    glm::mat4 m_TransformMatrix = glm::mat4(1.0f);

    float m_Time = 0.0f;

    float m_CurrentRotation = 0.0f;
    float m_RotationSpeed1 = 1.0f;
    float m_RotationSpeed2 = 1.0f;
    glm::vec3 m_RotationVector1 = glm::vec3(1.0f,0.0f,0.0f);
    glm::vec3 m_RotationVector2 = glm::vec3(0.0f,1.0f,0.0f);

    glm::vec4 m_Color = glm::vec4(1.0f,0.0f,1.0f,1.0f);
};

#endif // S_PLANE_H
