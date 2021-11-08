#include <sstream>

#include "cube.h"
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

constexpr GLfloat Cube::m_VERTICES[];

GLuint Cube::m_Vao = 0, Cube::m_Vbo = 0, Cube::m_Shader = 0;

Cube::Cube(GLuint m_Shader, glm::vec3 pos, float scale, glm::vec4 color, float rotationSpeed1, float rotationSpeed2, glm::vec3 rotationVector1, glm::vec3 rotationVector2)
    : m_Position(pos)
    , m_Scale(scale)
    , m_Color(color)
    , m_RotationSpeed1(rotationSpeed1)
    , m_RotationSpeed2(rotationSpeed2)
    , m_RotationVector1(rotationVector1)
    , m_RotationVector2(rotationVector2)
{

    if(!m_Vao) {
        this->m_Shader = m_Shader;

        glGenVertexArrays(1, &m_Vao);
        glBindVertexArray(m_Vao);

        glGenBuffers(1, &m_Vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_NUM_VERTS * 3, m_VERTICES, GL_STATIC_DRAW);

        int posAttr = glGetAttribLocation(m_Shader, "a_position");

        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glVertexAttribPointer(posAttr,  // Attribute location
                            3,          // Number of components
                            GL_FLOAT,   // Type of each component
                            GL_FALSE,   // Normalize?
                            0,          // Stride
                            0);         // Offset
        glEnableVertexAttribArray(posAttr);
    }
}

Cube::~Cube()
{
    if(m_Vao) {
        glDeleteVertexArrays(1, &m_Vao);
        glDeleteBuffers(1, &m_Vbo);
        m_Vao = 0;
        m_Vbo = 0;
    }
}

void Cube::update(float dt)
{
    m_CurrentRotation += dt;
}

void Cube::render(int width, int height, const glm::mat4 &mProj, const glm::mat4 &mView) const
{
    glm::mat4 mWorld = glm::translate(glm::mat4(1.0f), m_Position);
	glm::mat4 mRotate = glm::rotate(glm::mat4(1.0f), m_CurrentRotation * m_RotationSpeed1, m_RotationVector1);
	mRotate = mRotate * glm::rotate(glm::mat4(1.0f), m_CurrentRotation * m_RotationSpeed2, m_RotationVector2);
    glm::mat4 mScale = glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, m_Scale, m_Scale));
	glm::mat4 mWorldViewProj = mProj * mView * mWorld * mRotate * mScale;
	
    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "u_world_view_proj"), 1, GL_FALSE, glm::value_ptr(mWorldViewProj));

    glUniform4f(glGetUniformLocation(m_Shader, "u_color"), m_Color.x, m_Color.y, m_Color.z, m_Color.w);

    glBindVertexArray(m_Vao);

	glDrawArrays(GL_TRIANGLES, 0, m_NUM_VERTS);
}

std::string Cube::toString(uint id) const
{
    std::ostringstream out;
    out.precision(4);

    out << "Cube" << id << ": Pos:X" << m_Position.x << ",Y" << m_Position.y << ",Z" << m_Position.z
        << "; Scale:" << m_Scale
        << "; Current Rotation:" << m_CurrentRotation
        << "; Rotation Vector 1:X" << m_RotationVector1.x << ",Y" << m_RotationVector1.y << ",Z" << m_RotationVector1.z
        << "; Rotation Vector 2:X" << m_RotationVector2.x << ",Y" << m_RotationVector2.y << ",Z" << m_RotationVector2.z
        << "; Color:R" << m_Color.r << ",G" << m_Color.g << ",B" << m_Color.b
    << "\n";

    return out.str();
}

void Cube::setPosition(const glm::vec3 &position)
{
    this->m_Position = position;
}

void Cube::setScale(float scale)
{
    this->m_Scale = scale;
}

void Cube::setColor(const glm::vec4 &color)
{
    this->m_Color = color;
}

void Cube::setRotationSpeed1(float rotationSpeed)
{
    this->m_RotationSpeed1 = rot_speed;
}

void Cube::setRotationSpeed2(float rotationSpeed)
{
    this->m_RotationSpeed2 = rot_speed;
}

void Cube::setRotationVector1(const glm::vec3 &rotationVector)
{
    this->m_RotationVector1 = rot_vec;
}

void Cube::setRotationVector2(const glm::vec3 &rotationVector)
{
    this->m_RotationVector2 = rot_vec;
}

glm::vec3 Cube::getPosition() const
{
    return m_Position;
}

float Cube::getScale() const
{
    return m_Scale;
}

glm::vec4 Cube::getColor() const
{
    return m_Color;
}

float Cube::getRotationSpeed1() const
{
    return m_RotationSpeed1;
}

float Cube::getRotationSpeed2() const
{
    return m_RotationSpeed2;
}

glm::vec3 Cube::getRotationVector1() const
{
    return m_RotationVector1;
}

glm::vec3 Cube::getRotationVector2() const
{
    return m_RotationVector2;
}
