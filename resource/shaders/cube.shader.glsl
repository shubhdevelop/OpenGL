#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aColor;


out vec3 v_Color;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Proj * u_View * u_Model * vec4(position, 1.0);
    v_Color = aColor;
}

#shader fragment

#version 330 core

layout (location = 0) out vec4 color;

in vec3 v_Color;

void main()
{
    color = vec4(v_Color, 1.0);
}
