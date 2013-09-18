#version 400

uniform mat4 qt_ModelViewProjectionMatrix;
// first attribute the vertex values from our VAO
layout (location=0) in vec3 inVert;
// second attribute the UV values from our VAO
layout (location=1)in vec2 inUV;
// we use this to pass the UV values to the frag shader
out vec2 vertUV;

void main(void)
{
    gl_Position = qt_ModelViewProjectionMatrix*vec4(inVert, 1.0);;

    // pass the UV values to the frag shader
    vertUV=inUV.st;
}
