#include <sstream>

#include "plane.h"
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

GLuint Plane::m_Vao = 0, Plane::m_Vbo = 0, Plane::m_Shader = 0;

Plane::Plane(GLuint shader, uint subdivisions, glm::vec3 position, float scale, glm::mat4 transform_mat, glm::vec4 color, float rot_speed1, float rot_speed2, glm::vec3 rot_vec1, glm::vec3 rot_vec2)
    : m_Subdivisions(subdivisions)
    , m_Position(position)
    , m_Scale(scale)
    , m_TransformMatrix(transform_mat)
    , m_Color(color)
    , m_RotationSpeed1(rot_speed1)
    , m_RotationSpeed2(rot_speed2)
    , m_RotationVector1(rot_vec1)
    , m_RotationVector2(rot_vec2)
{
    generateVertices();

    if(!m_Vao) {
        glInit(shader);
    }
}

Plane::Plane(GLuint shader, uint subdivisions) : m_Subdivisions(subdivisions) {
    generateVertices();

    if(!m_Vao) {
        glInit(shader);
    }

    m_Position = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Scale = randomFloat(1.0f,5.0f);
	m_Color = glm::vec4(randomFloat(0.0f,1.0f), randomFloat(0.0f,1.0f), randomFloat(0.0f,1.0f), 1.0f);

	m_RotationSpeed1 = 0.0f;
	m_RotationSpeed2 = 0.0f;
	m_RotationVector1 = glm::vec3(randomFloat(-1.0f,1.0f), randomFloat(-1.0f,1.0f), randomFloat(-1.0f,1.0f));
	m_RotationVector2 = glm::vec3(randomFloat(-1.0f,1.0f), randomFloat(-1.0f,1.0f), randomFloat(-1.0f,1.0f));
}

void Plane::generateVertices() {
    int numSquares = pow(4, (m_Subdivisions - 1));
    int rowsCols = sqrt(numSquares);
    int half = rowsCols / 2;
    float sideLen = 0.99;

    for(int i = -half; i < half; ++i) {
        float anchorZ = i;

        for(int j = -half; j < half; ++j) {
            float anchorX = j;

            vertices.insert(vertices.cend(), {anchorX, 0.0f, anchorZ});
            vertices.insert(vertices.cend(), {anchorX + sideLen, 0.0f, anchorZ});
            vertices.insert(vertices.cend(), {anchorX + sideLen, 0.0f, anchorZ + sideLen});
            vertices.insert(vertices.cend(), {anchorX + sideLen, 0.0f, anchorZ + sideLen});
            vertices.insert(vertices.cend(), {anchorX, 0.0f, anchorZ + sideLen});
            vertices.insert(vertices.cend(), {anchorX, 0.0f, anchorZ});
        }
    }
}

void Plane::glInit(GLuint shader) {
    this->m_Shader = shader;

    glGenVertexArrays(1, &m_Vao);
    glBindVertexArray(m_Vao);

    glGenBuffers(1, &m_Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    int posAttr = glGetAttribLocation(shader, "a_position");

    glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
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
    if(m_Vao) {
        glDeleteVertexArrays(1, &m_Vao);
        glDeleteBuffers(1, &m_Vbo);
        m_Vao = 0;
        m_Vbo = 0;
    }
}

float Plane::randomFloat(float lower, float upper)
{
	return lower + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(upper-lower)));
}

void Plane::update(float dt)
{
    m_CurrentRotation += dt;
    m_Time += dt;
}

void Plane::render(int width, int height, const glm::mat4 &mProj, const glm::mat4 &mView) const
{
    glm::mat4 mWorld = glm::translate(glm::mat4(1.0f), m_Position);
	glm::mat4 mRotate = glm::rotate(glm::mat4(1.0f), m_CurrentRotation * m_RotationSpeed1, m_RotationVector1);
	mRotate = mRotate * glm::rotate(glm::mat4(1.0f), m_CurrentRotation * m_RotationSpeed2, m_RotationVector2);
    glm::mat4 mScale = glm::scale(glm::mat4(1.0f), glm::vec3(m_Scale, m_Scale, m_Scale));
	glm::mat4 mWorldViewProj = mProj * mView * mWorld * mRotate * mScale * m_TransformMatrix;

    glUniformMatrix4fv(glGetUniformLocation(m_Shader, "u_world_view_proj"), 1, GL_FALSE, glm::value_ptr(mWorldViewProj));

    glUniform1f(glGetUniformLocation(m_Shader, "u_time"), m_Time);

    glUniform4f(glGetUniformLocation(m_Shader, "u_color"), m_Color.r, m_Color.g, m_Color.b, m_Color.a);

    glBindVertexArray(m_Vao);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
}

std::string Plane::to_string(uint id) const
{
    std::ostringstream out;
    out.precision(4);

    out << "Plane" << id << ": Pos:X" << m_Position.x << ",Y" << m_Position.y << ",Z" << m_Position.z
    << "; Scale:" << m_Scale
    << "; Current Rotation:" << m_CurrentRotation
    << "; Rotation Vector 1:X" << m_RotationVector1.x << ",Y" << m_RotationVector1.y << ",Z" << m_RotationVector1.z
    << "; Rotation Vector 2:X" << m_RotationVector2.x << ",Y" << m_RotationVector2.y << ",Z" << m_RotationVector2.z
    << "; Color:R" << m_Color.r << ",G" << m_Color.g << ",B" << m_Color.b
    << "\n";

    return out.str();
}

void Plane::setPosition(const glm::vec3 &position)
{
    this->m_Position = position;
}

void Plane::setScale(float scale)
{
    this->m_Scale = scale;
}

void Plane::setTransformMatrix(const glm::mat4 &transform_mat)
{
    this->m_TransformMatrix = transform_mat;
}

void Plane::setColor(const glm::vec4 &color)
{
    this->m_Color = color;
}

void Plane::setRotationSpeed1(float rot_speed)
{
    this->m_RotationSpeed1 = rot_speed;
}

void Plane::setRotationSpeed2(float rot_speed)
{
    this->m_RotationSpeed2 = rot_speed;
}

void Plane::setRotationVector1(const glm::vec3 &rot_vec)
{
    this->m_RotationVector1 = rot_vec;
}

void Plane::setRotationVector2(const glm::vec3 &rot_vec)
{
    this->m_RotationVector2 = rot_vec;
}

glm::vec3 Plane::getPosition() const
{
    return m_Position;
}

float Plane::getScale() const
{
    return m_Scale;
}

glm::mat4 Plane::getTransformMatrix() const
{
    return m_TransformMatrix;
}

glm::vec4 Plane::getColor() const
{
    return m_Color;
}

float Plane::getRotationSpeed1() const
{
    return m_RotationSpeed1;
}

float Plane::getRotationSpeed2() const
{
    return m_RotationSpeed2;
}

glm::vec3 Plane::getRotationVector1() const
{
    return m_RotationVector1;
}

glm::vec3 Plane::getRotationVector2() const
{
    return m_RotationVector2;
}
