#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;


out vec2 v_TexCoord;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform float u_Time;

void main()
{
    vec3 pos = position.xyz;

    float time = u_Time;
    //wave
    pos.y += sin(pos.x*2.0 + time) * 0.2;
    pos.y += cos(pos.z*2.0 + time) * 0.2;

    // MVP translation
    vec4 final = u_Proj * u_View * u_Model * vec4(pos, 1.0);
    gl_Position = final;
    v_TexCoord = texCoord;
}

#shader fragment

#version 330 core

layout (location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor;
};
