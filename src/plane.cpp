#include <sstream>

#include "plane.h"
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

GLuint Plane::m_vao = 0, Plane::m_vbo = 0, Plane::m_shader = 0;

Plane::Plane(GLuint shader, uint subdivisions, glm::vec3 position, float scale, glm::mat4 transform_mat, glm::vec4 color, float rot_speed1, float rot_speed2, glm::vec3 rot_vec1, glm::vec3 rot_vec2)
    : m_subdivisions(subdivisions)
    , m_position(position)
    , m_scale(scale)
    , m_transformMatrix(transform_mat)
    , m_color(color)
    , m_rotationSpeed1(rot_speed1)
    , m_rotationSpeed2(rot_speed2)
    , m_rotationVector1(rot_vec1)
    , m_rotationVector2(rot_vec2)
{
    generateVertices();

    if(!m_vao) {
        glInit(shader);
    }
}

Plane::Plane(GLuint shader, uint subdivisions) : m_subdivisions(subdivisions) {
    generateVertices();

    if(!m_vao) {
        glInit(shader);
    }

    m_position = glm::vec3(0.0f, 1.0f, 0.0f);
	m_scale = randomFloat(1.0f,5.0f);
	m_color = glm::vec4(randomFloat(0.0f,1.0f), randomFloat(0.0f,1.0f), randomFloat(0.0f,1.0f), 1.0f);

	m_rotationSpeed1 = 0.0f;
	m_rotationSpeed2 = 0.0f;
	m_rotationVector1 = glm::vec3(randomFloat(-1.0f,1.0f), randomFloat(-1.0f,1.0f), randomFloat(-1.0f,1.0f));
	m_rotationVector2 = glm::vec3(randomFloat(-1.0f,1.0f), randomFloat(-1.0f,1.0f), randomFloat(-1.0f,1.0f));
}

void Plane::generateVertices() {
    int numSquares = pow(4, (m_subdivisions - 1));
    int rowsCols = sqrt(numSquares);
    int half = rowsCols / 2;
    float sideLen = 0.99;

    for(int i = -half; i < half; ++i) {
        float anchorZ = i;

        for(int j = -half; j < half; ++j) {
            float anchorX = j;

            m_vertices.insert(m_vertices.cend(), {anchorX, 0.0f, anchorZ});
            m_vertices.insert(m_vertices.cend(), {anchorX + sideLen, 0.0f, anchorZ});
            m_vertices.insert(m_vertices.cend(), {anchorX + sideLen, 0.0f, anchorZ + sideLen});
            m_vertices.insert(m_vertices.cend(), {anchorX + sideLen, 0.0f, anchorZ + sideLen});
            m_vertices.insert(m_vertices.cend(), {anchorX, 0.0f, anchorZ + sideLen});
            m_vertices.insert(m_vertices.cend(), {anchorX, 0.0f, anchorZ});
        }
    }
}

void Plane::glInit(GLuint shader) {
    this->m_shader = shader;

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

    int posAttr = glGetAttribLocation(shader, "a_position");

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(posAttr,  // Attribute location
                          3,        // Number of components
                          GL_FLOAT, // Type of each component
                          GL_FALSE, // Normalize?
                          0,        // Stride
                          0);       // Offset
    glEnableVertexAttribArray(posAttr);
}

Plane::~Plane()
{
    if(m_vao) {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        m_vao = 0;
        m_vbo = 0;
    }
}

float Plane::randomFloat(float lower, float upper)
{
	return lower + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(upper-lower)));
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

    glUniformMatrix4fv(glGetUniformLocation(m_shader, "u_world_view_proj"), 1, GL_FALSE, glm::value_ptr(mWorldViewProj));

    glUniform1f(glGetUniformLocation(m_shader, "u_time"), m_time);

    glUniform4f(glGetUniformLocation(m_shader, "u_color"), m_color.r, m_color.g, m_color.b, m_color.a);

    glBindVertexArray(m_vao);

	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() / 3);
}

std::string Plane::to_string(uint id) const
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

void Plane::setTransformMatrix(const glm::mat4 &transform_mat)
{
    this->m_transformMatrix = transform_mat;
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
