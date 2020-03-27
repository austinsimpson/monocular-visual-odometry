#version 410
in vec3 position;
in vec3 inColor;
uniform mat4 transformation;

out vec3 vertexColor;

void main() 
{
   vertexColor = inColor;
   gl_PointSize = 1;
   gl_Position = transformation * vec4(position, 1.0);
};