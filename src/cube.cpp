#include <sstream>

#include "cube.h"
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

constexpr GLfloat Cube::vertices[];

GLuint Cube::vao = 0, Cube::vbo = 0, Cube::shader = 0;

Cube::Cube(GLuint shader, glm::vec3 pos, float scale, glm::vec4 color, float rot_speed1, float rot_speed2, glm::vec3 rot_vec1, glm::vec3 rot_vec2)
    : pos(pos)
    , scale(scale)
    , color(color)
    , rot_speed1(rot_speed1)
    , rot_speed2(rot_speed2)
    , rot_vec1(rot_vec1)
    , rot_vec2(rot_vec2)
{

    if(!vao) {
        this->shader = shader;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * NUM_VERTS * 3, vertices, GL_STATIC_DRAW);

        int posAttr = glGetAttribLocation(shader, "a_position");

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
    if(vao) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        vao = 0;
        vbo = 0;
    }
}

void Cube::update(float dt)
{
    cur_rot += dt;
}

void Cube::render(int width, int height, const glm::mat4 &mProj, const glm::mat4 &mView) const
{
    glm::mat4 mWorld = glm::translate(glm::mat4(1.0f), pos);
	glm::mat4 mRotate = glm::rotate(glm::mat4(1.0f), cur_rot * rot_speed1, rot_vec1);
	mRotate = mRotate * glm::rotate(glm::mat4(1.0f), cur_rot * rot_speed2, rot_vec2);
    glm::mat4 mScale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
	glm::mat4 mWorldViewProj = mProj * mView * mWorld * mRotate * mScale;
	
    glUniformMatrix4fv(glGetUniformLocation(shader, "world_view_proj"), 1, GL_FALSE, glm::value_ptr(mWorldViewProj));

    glUniform4f(glGetUniformLocation(shader, "c_color"), color.x, color.y, color.z, color.w);

    glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTS);
}

std::string Cube::to_string(uint id) const
{
    std::ostringstream out;
    out.precision(4);

    out << "Cube" << id << ": Pos:X" << pos.x << ",Y" << pos.y << ",Z" << pos.z
    << "; Scale:" << scale
    << "; Current Rotation:" << cur_rot
    << "; Rotation Vector 1:X" << rot_vec1.x << ",Y" << rot_vec1.y << ",Z" << rot_vec1.z
    << "; Rotation Vector 2:X" << rot_vec2.x << ",Y" << rot_vec2.y << ",Z" << rot_vec2.z
    << "; Color:R" << color.r << ",G" << color.g << ",B" << color.b
    << "\n";

    return out.str();
}

void Cube::set_position(const glm::vec3 &pos)
{
    this->pos = pos;
}

void Cube::set_scale(float scale)
{
    this->scale = scale;
}

void Cube::set_color(const glm::vec4 &color)
{
    this->color = color;
}

void Cube::set_rotation_speed_1(float rot_speed)
{
    this->rot_speed1 = rot_speed;
}

void Cube::set_rotation_speed_2(float rot_speed)
{
    this->rot_speed2 = rot_speed;
}

void Cube::set_rotation_vector_1(const glm::vec3 &rot_vec)
{
    this->rot_vec1 = rot_vec;
}

void Cube::set_rotation_vector_2(const glm::vec3 &rot_vec)
{
    this->rot_vec2 = rot_vec;
}

glm::vec3 Cube::get_position() const
{
    return pos;
}

float Cube::get_scale() const
{
    return scale;
}

glm::vec4 Cube::get_color() const
{
    return color;
}

float Cube::get_rotation_speed_1() const
{
    return rot_speed1;
}

float Cube::get_rotation_speed_2() const
{
    return rot_speed2;
}

glm::vec3 Cube::get_rotation_vector_1() const
{
    return rot_vec1;
}

glm::vec3 Cube::get_rotation_vector_2() const
{
    return rot_vec2;
}
