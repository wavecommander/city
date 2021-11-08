#include <sstream>

#include "cube.h"
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

constexpr GLfloat Cube::VERTICES[];

GLuint Cube::m_vao = 0, Cube::m_vbo = 0, Cube::m_shader = 0;

Cube::Cube(GLuint m_shader, glm::vec3 pos, float scale, glm::vec4 color, float rotationSpeed1, float rotationSpeed2, glm::vec3 rotationVector1, glm::vec3 rotationVector2)
    : m_position(pos)
    , m_scale(scale)
    , m_color(color)
    , m_rotationSpeed1(rotationSpeed1)
    , m_rotationSpeed2(rotationSpeed2)
    , m_rotationVector1(rotationVector1)
    , m_rotationVector2(rotationVector2)
{

    if(!m_vao) {
        this->m_shader = m_shader;

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUM_VERTS * 3, VERTICES, GL_STATIC_DRAW);

        int posAttr = glGetAttribLocation(m_shader, "a_position");

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
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
    if(m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        m_vao = 0;
        m_vbo = 0;
    }
}

void Cube::update(float dt)
{
    m_currentRotation += dt;
}

void Cube::render(int width, int height, const glm::mat4 &mProj, const glm::mat4 &mView) const
{
    glm::mat4 mWorld = glm::translate(glm::mat4(1.0f), m_position);
	glm::mat4 mRotate = glm::rotate(glm::mat4(1.0f), m_currentRotation * m_rotationSpeed1, m_rotationVector1);
	mRotate = mRotate * glm::rotate(glm::mat4(1.0f), m_currentRotation * m_rotationSpeed2, m_rotationVector2);
    glm::mat4 mScale = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale, m_scale, m_scale));
	glm::mat4 mWorldViewProj = mProj * mView * mWorld * mRotate * mScale;
	
    glUniformMatrix4fv(glGetUniformLocation(m_shader, "u_world_view_proj"), 1, GL_FALSE, glm::value_ptr(mWorldViewProj));

    glUniform4f(glGetUniformLocation(m_shader, "u_color"), m_color.x, m_color.y, m_color.z, m_color.w);

    glBindVertexArray(m_vao);

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTS);
}

std::string Cube::toString(uint id) const
{
    std::ostringstream out;
    out.precision(4);

    out << "Cube" << id << ": Pos:X" << m_position.x << ",Y" << m_position.y << ",Z" << m_position.z
        << "; Scale:" << m_scale
        << "; Current Rotation:" << m_currentRotation
        << "; Rotation Vector 1:X" << m_rotationVector1.x << ",Y" << m_rotationVector1.y << ",Z" << m_rotationVector1.z
        << "; Rotation Vector 2:X" << m_rotationVector2.x << ",Y" << m_rotationVector2.y << ",Z" << m_rotationVector2.z
        << "; Color:R" << m_color.r << ",G" << m_color.g << ",B" << m_color.b
    << "\n";

    return out.str();
}

void Cube::setPosition(const glm::vec3 &position)
{
    m_position = position;
}

void Cube::setScale(float scale)
{
    m_scale = scale;
}

void Cube::setColor(const glm::vec4 &color)
{
    m_color = color;
}

void Cube::setRotationSpeed1(float rotationSpeed)
{
    m_rotationSpeed1 = rotationSpeed;
}

void Cube::setRotationSpeed2(float rotationSpeed)
{
    m_rotationSpeed2 = rotationSpeed;
}

void Cube::setRotationVector1(const glm::vec3 &rotationVector)
{
    m_rotationVector1 = rotationVector;
}

void Cube::setRotationVector2(const glm::vec3 &rotationVector)
{
    m_rotationVector2 = rotationVector;
}

glm::vec3 Cube::getPosition() const
{
    return m_position;
}

float Cube::getScale() const
{
    return m_scale;
}

glm::vec4 Cube::getColor() const
{
    return m_color;
}

float Cube::getRotationSpeed1() const
{
    return m_rotationSpeed1;
}

float Cube::getRotationSpeed2() const
{
    return m_rotationSpeed2;
}

glm::vec3 Cube::getRotationVector1() const
{
    return m_rotationVector1;
}

glm::vec3 Cube::getRotationVector2() const
{
    return m_rotationVector2;
}
