#version 400

uniform sampler2D qt_Texture0;
// the vertex UV
in vec2 vertUV;
out vec4 outColour;


void main(void)
{
    // set the fragment colour to the current texture
     outColour = texture(qt_Texture0,vertUV);
}
