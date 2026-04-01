#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 v_Color;
out vec2 v_TexCoord;
out vec4 v_Freq_Amp;
out float v_Time;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform float u_Time;
uniform vec4 u_Freq_Amp;

void main()
{
    vec3 pos = position;
    
    float wave1 = sin(pos.x * u_Freq_Amp.x * 2.0 + u_Time) * u_Freq_Amp.z;
    float wave2 = cos(pos.z * u_Freq_Amp.x * 2.0 + u_Time) * u_Freq_Amp.w;
    float wave3 = sin(pos.x * u_Freq_Amp.x * 4.0 + u_Time * 1.5) * u_Freq_Amp.z * 0.5;
    float wave4 = cos(pos.z * u_Freq_Amp.x * 4.0 + u_Time * 1.5) * u_Freq_Amp.w * 0.5;
    float wave5 = sin(pos.x * u_Freq_Amp.x * 8.0 + u_Time * 2.0) * u_Freq_Amp.z * 0.25;
    float wave6 = cos(pos.z * u_Freq_Amp.x * 8.0 + u_Time * 2.0) * u_Freq_Amp.w * 0.25;
    
    pos.y += wave1 + wave2 + wave3 + wave4 + wave5 + wave6;
    
    gl_Position = u_Proj * u_View * u_Model * vec4(pos, 1.0);
    v_Color = aColor;
    v_TexCoord = aTexCoord;
    v_Freq_Amp = u_Freq_Amp;
    v_Time = u_Time;
}

#shader fragment

#version 330 core

layout (location = 0) out vec4 color;

in vec3 v_Color;
in vec2 v_TexCoord;
in vec4 v_Freq_Amp;
in float v_Time;

uniform sampler2D u_Texture;

void main()
{
    vec2 uv = v_TexCoord;
    
    uv.x += sin(uv.y * v_Freq_Amp.x + v_Time) * v_Freq_Amp.z;
    uv.y += cos(uv.x * v_Freq_Amp.y + v_Time) * v_Freq_Amp.w;
    uv.x += sin(uv.y * v_Freq_Amp.x * 2.0 + v_Time * 1.5) * v_Freq_Amp.z * 0.5;
    uv.y += cos(uv.x * v_Freq_Amp.y * 2.0 + v_Time * 1.5) * v_Freq_Amp.w * 0.5;
    uv.x += sin(uv.y * v_Freq_Amp.x * 4.0 + v_Time * 2.0) * v_Freq_Amp.z * 0.25;
    uv.y += cos(uv.x * v_Freq_Amp.y * 4.0 + v_Time * 2.0) * v_Freq_Amp.w * 0.25;
    
    vec4 texColor = texture(u_Texture, uv);
    color = texColor;
}