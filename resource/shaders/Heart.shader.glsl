#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform float u_Time;
uniform float u_WobbleAmplitude;

void main()
{
    vec3 pos = aPos;
    pos.y += sin(u_Time * 2.0) * u_WobbleAmplitude;

    gl_Position = u_Proj * u_View * u_Model * vec4(pos, 1.0);
    v_FragPos = vec3(u_Model * vec4(pos, 1.0));
    v_Normal = mat3(transpose(inverse(u_Model))) * aNormal;
}

#shader fragment

#version 330 core

layout (location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;
uniform vec4 u_ObjectColor;
uniform vec4 u_LightColor;
uniform float u_Ambient;
uniform float u_SpecularStrength;

void main()
{
    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec4 diffuse = diff * u_LightColor;
    vec4 ambient = u_Ambient * u_LightColor;
    vec4 specular = u_SpecularStrength * spec * u_LightColor;

    vec3 result = u_ObjectColor.rgb * (ambient.rgb + diffuse.rgb + specular.rgb);
    color = vec4(result, 1.0);
}
