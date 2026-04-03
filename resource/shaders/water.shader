#shader vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoord;


out vec2 v_TexCoord;
out float v_Time;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform float u_Time;

void main()
{
    vec3 pos = position.xyz;

    float time = u_Time;

    // MVP translation
    vec4 final = u_Proj * u_View * u_Model * vec4(pos, 1.0);
    gl_Position = final;
    v_TexCoord = texCoord;
    v_Time = time;
}

#shader fragment

#version 330 core

layout (location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_Time;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform vec4 u_Freq_Amp;

void main()
{
    vec2 uv = v_TexCoord;
    float time = v_Time;

    // Wave distortion
    uv.x += sin(uv.y * u_Freq_Amp.x + time) * u_Freq_Amp.z;
    uv.y += cos(uv.x * u_Freq_Amp.y + time) * u_Freq_Amp.w;
    vec4 texColor = texture(u_Texture, uv);
    color = texColor;
}
