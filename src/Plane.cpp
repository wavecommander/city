#include <sstream>

#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

#include "Plane.h"
#include "utils.h"

GLuint Plane::m_vao = 0, Plane::m_vbo = 0, Plane::m_shader = 0;

Plane::Plane(GLuint shader, uint subdivisions, glm::vec3 position, float scale, glm::mat4 transformMatrix, glm::vec4 color, float rotationSpeed1, float rotationSpeed2, glm::vec3 rotationVector1, glm::vec3 rotationVector2)
    : m_subdivisions(subdivisions)
    , m_position(position)
    , m_scale(scale)
    , m_transformMatrix(transformMatrix)
    , m_color(color)
    , m_rotationSpeed1(rotationSpeed1)
    , m_rotationSpeed2(rotationSpeed2)
    , m_rotationVector1(rotationVector1)
    , m_rotationVector2(rotationVector2)
{
    _generateVertices();

    if (!m_vao) {
        _glInit(shader);
    }
}

Plane::Plane(GLuint shader, uint subdivisions)
    : m_subdivisions(subdivisions)
    , m_position(glm::vec3(0.0f, 0.0f, 0.0f))
    , m_scale(wolf::randFloat(1.0f, 5.0f))
    , m_color(glm::vec4(wolf::randFloat(0.0f, 1.0f), wolf::randFloat(0.0f, 1.0f), wolf::randFloat(0.0f, 1.0f), 1.0f))
    , m_rotationSpeed1(0.0f)
    , m_rotationSpeed2(0.0f)
    , m_rotationVector1(glm::vec3(wolf::randFloat(-1.0f, 1.0f), wolf::randFloat(-1.0f, 1.0f), wolf::randFloat(-1.0f, 1.0f)))
    , m_rotationVector2(glm::vec3(wolf::randFloat(-1.0f, 1.0f), wolf::randFloat(-1.0f, 1.0f), wolf::randFloat(-1.0f, 1.0f)))
{
    _generateVertices();

    if (!m_vao) {
        _glInit(shader);
    }
}

void Plane::_generateVertices()
{
    int num_squares = pow(4, (m_subdivisions - 1));
    int rows_cols = sqrt(num_squares);
    int half = rows_cols / 2;
    float outer_len = 1.0f;
    float inner_len = outer_len / (float) rows_cols;

    int num_verts = num_squares * 6;
    m_vertices.reserve(num_verts);

    for(int i = -half; i < half; ++i) {
        float anchor_z = i / (float) rows_cols;

        for(int j = -half; j < half; ++j) {
            float anchor_x = j / (float) rows_cols;

            m_vertices.insert(m_vertices.cend(), {anchor_x, 0.0f, anchor_z});
            m_vertices.insert(m_vertices.cend(), {anchor_x + inner_len, 0.0f, anchor_z});
            m_vertices.insert(m_vertices.cend(), {anchor_x + inner_len, 0.0f, anchor_z + inner_len});
            m_vertices.insert(m_vertices.cend(), {anchor_x + inner_len, 0.0f, anchor_z + inner_len});
            m_vertices.insert(m_vertices.cend(), {anchor_x, 0.0f, anchor_z + inner_len});
            m_vertices.insert(m_vertices.cend(), {anchor_x, 0.0f, anchor_z});
        }
    }
}

void Plane::_glInit(GLuint shader)
{
    m_shader = shader;

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

    int posAttr = glGetAttribLocation(shader, "a_position");

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(posAttr, // Attribute location
        3, // Number of components
        GL_FLOAT, // Type of each component
        GL_FALSE, // Normalize?
        0, // Stride
        0); // Offset
    glEnableVertexAttribArray(posAttr);
}

Plane::~Plane()
{
    if (m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        m_vao = 0;
        m_vbo = 0;
    }
}

void Plane::update(float dt)
{
    m_currentRotation += dt;
    m_time += dt;
}

void Plane::render(int width, int height, const glm::mat4 &mProj, const glm::mat4 &mView) const
{
    glm::mat4 mWorld = glm::translate(glm::mat4(1.0f), m_position);
    glm::mat4 mRotate = glm::rotate(glm::mat4(1.0f), m_currentRotation * m_rotationSpeed1, m_rotationVector1);
    mRotate = mRotate * glm::rotate(glm::mat4(1.0f), m_currentRotation * m_rotationSpeed2, m_rotationVector2);
    glm::mat4 mScale = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale, m_scale, m_scale));
    glm::mat4 mWorldViewProj = mProj * mView * mWorld * mRotate * mScale * m_transformMatrix;

    glUseProgram(m_shader);

    glUniformMatrix4fv(glGetUniformLocation(m_shader, "u_world_view_proj"), 1, GL_FALSE, glm::value_ptr(mWorldViewProj));

    glUniform1f(glGetUniformLocation(m_shader, "u_time"), m_time);

    glUniform4f(glGetUniformLocation(m_shader, "u_color"), m_color.r, m_color.g, m_color.b, m_color.a);

    glBindVertexArray(m_vao);

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 3);
}

std::string Plane::toString(uint id) const
{
    std::ostringstream out;
    out.precision(4);

    out << "Plane" << id << ": Pos:X" << m_position.x << ",Y" << m_position.y << ",Z" << m_position.z
        << "; Scale:" << m_scale
        << "; Current Rotation:" << m_currentRotation
        << "; Rotation Vector 1:X" << m_rotationVector1.x << ",Y" << m_rotationVector1.y << ",Z" << m_rotationVector1.z
        << "; Rotation Vector 2:X" << m_rotationVector2.x << ",Y" << m_rotationVector2.y << ",Z" << m_rotationVector2.z
        << "; Color:R" << m_color.r << ",G" << m_color.g << ",B" << m_color.b
        << "\n";

    return out.str();
}

void Plane::setPosition(const glm::vec3 &position)
{
    this->m_position = position;
}

void Plane::setScale(float scale)
{
    this->m_scale = scale;
}

void Plane::setTransformMatrix(const glm::mat4 &transformMatrix)
{
    this->m_transformMatrix = transformMatrix;
}

void Plane::setColor(const glm::vec4 &color)
{
    this->m_color = color;
}

void Plane::setRotationSpeed1(float rot_speed)
{
    this->m_rotationSpeed1 = rot_speed;
}

void Plane::setRotationSpeed2(float rot_speed)
{
    this->m_rotationSpeed2 = rot_speed;
}

void Plane::setRotationVector1(const glm::vec3 &rot_vec)
{
    this->m_rotationVector1 = rot_vec;
}

void Plane::setRotationVector2(const glm::vec3 &rot_vec)
{
    this->m_rotationVector2 = rot_vec;
}

glm::vec3 Plane::getPosition() const
{
    return m_position;
}

float Plane::getScale() const
{
    return m_scale;
}

glm::mat4 Plane::getTransformMatrix() const
{
    return m_transformMatrix;
}

glm::vec4 Plane::getColor() const
{
    return m_color;
}

float Plane::getRotationSpeed1() const
{
    return m_rotationSpeed1;
}

float Plane::getRotationSpeed2() const
{
    return m_rotationSpeed2;
}

glm::vec3 Plane::getRotationVector1() const
{
    return m_rotationVector1;
}

glm::vec3 Plane::getRotationVector2() const
{
    return m_rotationVector2;
}
