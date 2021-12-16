in vec3 TexCoords;

uniform samplerCube skybox;

out vec4 PixelColor;

void main()
{
    PixelColor = texture(skybox, TexCoords);
}
