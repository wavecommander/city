uniform mat4 u_world_view_proj;
uniform float u_time;
uniform vec4 u_color;

in vec4 a_position;

out vec4 v_color;

void main()
{
    vec4 pos = a_position;
    pos.y += sin(u_time + pos.x);
    gl_Position = u_world_view_proj * pos;

    v_color = u_color;
}
