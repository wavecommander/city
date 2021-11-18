uniform mat4 u_world_view_proj;
uniform float u_time;
uniform vec4 u_color;

in vec4 a_position;

out vec4 v_color;

void main()
{
    gl_Position = u_world_view_proj * a_position;

    v_color = u_color;
}
