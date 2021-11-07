#ifndef S_CUBE_H
#define S_CUBE_H

#include <string>
#include "../wolf/wolf.h"

class Cube {
public:
    Cube(GLuint shader, glm::vec3 pos, float scale, glm::vec4 color, float rot_speed1, float rot_speed2, glm::vec3 rot_vec1, glm::vec3 rot_vec2);
    ~Cube();

    void render(int width, int height, const glm::mat4 &mProj, const glm::mat4 &mView) const;
    void update(float dt);
    std::string to_string(uint id) const;

    void set_position(const glm::vec3 &pos);
    void set_scale(float scale);
    void set_color(const glm::vec4 &color);

    void set_rotation_speed_1(float rot_speed);
    void set_rotation_speed_2(float rot_speed);
    void set_rotation_vector_1(const glm::vec3 &rot_vec);
    void set_rotation_vector_2(const glm::vec3 &rot_vec);

    glm::vec3 get_position() const;
    float get_scale() const;
    glm::vec4 get_color() const;

    float get_rotation_speed_1() const;
    float get_rotation_speed_2() const;
    glm::vec3 get_rotation_vector_1() const;
    glm::vec3 get_rotation_vector_2() const;


private:
    static GLuint vao, vbo, shader;

    glm::vec3 pos = glm::vec3(0.0f,0.0f,0.0f);
    float scale = 1.0f;

    float cur_rot = 0.0f;
    float rot_speed1 = 1.0f;
    float rot_speed2 = 1.0f;
    glm::vec3 rot_vec1 = glm::vec3(1.0f,0.0f,0.0f);
    glm::vec3 rot_vec2 = glm::vec3(0.0f,1.0f,0.0f);

    glm::vec4 color = glm::vec4(1.0f,0.0f,1.0f,1.0f);

    static constexpr uint NUM_VERTS = 3 * 2 * 6;
    static constexpr GLfloat vertices[] = {

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
