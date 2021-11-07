#include <sstream>

#include "plane.h"
#include "../wolf/wolf.h"
#include "../samplefw/Sample.h"

GLuint Plane::vao = 0, Plane::vbo = 0, Plane::shader = 0;

Plane::Plane(GLuint shader, uint subdivision, glm::vec3 position, float scale, glm::mat4 transform_mat, glm::vec4 color, float rot_speed1, float rot_speed2, glm::vec3 rot_vec1, glm::vec3 rot_vec2)
    : subdivisions(subdivisions)
    , position(position)
    , scale(scale)
    , transform_mat(transform_mat)
    , color(color)
    , rot_speed1(rot_speed1)
    , rot_speed2(rot_speed2)
    , rot_vec1(rot_vec1)
    , rot_vec2(rot_vec2)
{
    generate_vertices();

    if(!vao) {
        gl_init(shader);
    }
}

Plane::Plane(GLuint shader, uint subdivisions) {
    this->subdivisions = subdivisions;

    generate_vertices();

    if(!vao) {
        gl_init(shader);
    }

    position = glm::vec3(0.0f, 1.0f, 0.0f);
	scale = rand_f(1.0f,5.0f);
	color = glm::vec4(rand_f(0.0f,1.0f), rand_f(0.0f,1.0f), rand_f(0.0f,1.0f), 1.0f);

	rot_speed1 = 0.0f;
	rot_speed2 = 0.0f;
	rot_vec1 = glm::vec3(rand_f(-1.0f,1.0f), rand_f(-1.0f,1.0f), rand_f(-1.0f,1.0f));
	rot_vec2 = glm::vec3(rand_f(-1.0f,1.0f), rand_f(-1.0f,1.0f), rand_f(-1.0f,1.0f));
}

void Plane::generate_vertices() {
    int num_squares = pow(4, (subdivisions - 1));
    int rows_cols = sqrt(num_squares);
    int half = rows_cols / 2;
    float side_len = 0.99;

    for(int i = -half; i < half; ++i) {
        float anchor_z = i;

        for(int j = -half; j < half; ++j) {
            float anchor_x = j;

            vertices.insert(vertices.cend(), {anchor_x, 0.0f, anchor_z});
            vertices.insert(vertices.cend(), {anchor_x + side_len, 0.0f, anchor_z});
            vertices.insert(vertices.cend(), {anchor_x + side_len, 0.0f, anchor_z + side_len});
            vertices.insert(vertices.cend(), {anchor_x + side_len, 0.0f, anchor_z + side_len});
            vertices.insert(vertices.cend(), {anchor_x, 0.0f, anchor_z + side_len});
            vertices.insert(vertices.cend(), {anchor_x, 0.0f, anchor_z});
        }
    }
}

void Plane::gl_init(GLuint shader) {
    this->shader = shader;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    int posAttr = glGetAttribLocation(shader, "a_position");

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
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
    if(vao) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        vao = 0;
        vbo = 0;
    }
}

float Plane::rand_f(float lower, float upper)
{
	return lower + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(upper-lower)));
}

void Plane::update(float dt)
{
    cur_rot += dt;
    time += dt;
}

void Plane::render(int width, int height, const glm::mat4 &mProj, const glm::mat4 &mView) const
{
    glm::mat4 mWorld = glm::translate(glm::mat4(1.0f), position);
	glm::mat4 mRotate = glm::rotate(glm::mat4(1.0f), cur_rot * rot_speed1, rot_vec1);
	mRotate = mRotate * glm::rotate(glm::mat4(1.0f), cur_rot * rot_speed2, rot_vec2);
    glm::mat4 mScale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
	glm::mat4 mWorldViewProj = mProj * mView * mWorld * mRotate * mScale * transform_mat;

    glUniformMatrix4fv(glGetUniformLocation(shader, "world_view_proj"), 1, GL_FALSE, glm::value_ptr(mWorldViewProj));

    glUniform1f(glGetUniformLocation(shader, "u_time"), time);

    glUniform4f(glGetUniformLocation(shader, "c_color"), color.r, color.g, color.b, color.a);

    glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
}

std::string Plane::to_string(uint id) const
{
    std::ostringstream out;
    out.precision(4);

    out << "Plane" << id << ": Pos:X" << position.x << ",Y" << position.y << ",Z" << position.z
    << "; Scale:" << scale
    << "; Current Rotation:" << cur_rot
    << "; Rotation Vector 1:X" << rot_vec1.x << ",Y" << rot_vec1.y << ",Z" << rot_vec1.z
    << "; Rotation Vector 2:X" << rot_vec2.x << ",Y" << rot_vec2.y << ",Z" << rot_vec2.z
    << "; Color:R" << color.r << ",G" << color.g << ",B" << color.b
    << "\n";

    return out.str();
}

void Plane::set_position(const glm::vec3 &position)
{
    this->position = position;
}

void Plane::set_scale(float scale)
{
    this->scale = scale;
}

void Plane::set_transform_mat(const glm::mat4 &transform_mat)
{
    this->transform_mat = transform_mat;
}

void Plane::set_color(const glm::vec4 &color)
{
    this->color = color;
}

void Plane::set_rotation_speed_1(float rot_speed)
{
    this->rot_speed1 = rot_speed;
}

void Plane::set_rotation_speed_2(float rot_speed)
{
    this->rot_speed2 = rot_speed;
}

void Plane::set_rotation_vector_1(const glm::vec3 &rot_vec)
{
    this->rot_vec1 = rot_vec;
}

void Plane::set_rotation_vector_2(const glm::vec3 &rot_vec)
{
    this->rot_vec2 = rot_vec;
}

glm::vec3 Plane::get_position() const
{
    return position;
}

float Plane::get_scale() const
{
    return scale;
}

glm::mat4 Plane::get_transform_mat() const
{
    return transform_mat;
}

glm::vec4 Plane::get_color() const
{
    return color;
}

float Plane::get_rotation_speed_1() const
{
    return rot_speed1;
}

float Plane::get_rotation_speed_2() const
{
    return rot_speed2;
}

glm::vec3 Plane::get_rotation_vector_1() const
{
    return rot_vec1;
}

glm::vec3 Plane::get_rotation_vector_2() const
{
    return rot_vec2;
}
