uniform mat4 projection;
uniform mat4 view;

in vec4 a_position;
in vec2 a_uv1;

out vec2 v_uv1;

void main()
{
    gl_Position = projection * view * a_position;
	v_uv1 = a_uv1;
}
