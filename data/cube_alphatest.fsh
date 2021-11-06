in vec2 v_uv1;

uniform sampler2D tex;

out vec4 PixelColor;

void main()
{
    PixelColor = texture(tex, v_uv1);
	if( PixelColor.a < 0.1 )
		discard;
}