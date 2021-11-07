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

    void set_position(const glm::vec3 &position);
    void set_scale(float scale);
    void set_transform_mat(const glm::mat4 &transform_mat);
    void set_color(const glm::vec4 &color);

    void set_rotation_speed_1(float rot_speed);
    void set_rotation_speed_2(float rot_speed);
    void set_rotation_vector_1(const glm::vec3 &rot_vec);
    void set_rotation_vector_2(const glm::vec3 &rot_vec);

    glm::vec3 get_position() const;
    float get_scale() const;
    glm::mat4 get_transform_mat() const;
    glm::vec4 get_color() const;

    float get_rotation_speed_1() const;
    float get_rotation_speed_2() const;
    glm::vec3 get_rotation_vector_1() const;
    glm::vec3 get_rotation_vector_2() const;


private:
    static float rand_f(float lower, float upper);

    uint subdivisions = 1;

    std::vector<GLfloat> vertices;
    void generate_vertices();

    static GLuint vao, vbo, shader;
    void gl_init(GLuint shader);

    glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
    float scale = 1.0f;
    glm::mat4 transform_mat = glm::mat4(1.0f);

    float time = 0.0f;

    float cur_rot = 0.0f;
    float rot_speed1 = 1.0f;
    float rot_speed2 = 1.0f;
    glm::vec3 rot_vec1 = glm::vec3(1.0f,0.0f,0.0f);
    glm::vec3 rot_vec2 = glm::vec3(0.0f,1.0f,0.0f);

    glm::vec4 color = glm::vec4(1.0f,0.0f,1.0f,1.0f);
};

#endif // S_PLANE_H
