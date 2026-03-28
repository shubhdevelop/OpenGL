#include "glad/glad.h"
#include <Renderer.hpp>
#include <Shader.hpp>
#include <cstring>
#include <iostream>

// "resource/shaders/Basic.shader"
Shader::Shader(const char *filepath) {
  ShaderProgramSource source = ParseShader(filepath);
  m_rendererId = CreateShader(source.VertexSource, source.FragmentSource);
  GLCall(glUseProgram(m_rendererId));
}

Shader::~Shader() { GLCall(glDeleteProgram(m_rendererId)) }
void Shader::Bind() const { GLCall(glUseProgram(m_rendererId)); }
void Shader::UnBind() const { GLCall(glUseProgram(0)); }

unsigned int Shader::GetUniformLocation(const char *name) const {
  auto it = m_uniformLocationCache.find(name);
  if (it != m_uniformLocationCache.end())
    return it->second;
  GLCall(unsigned int location = glGetUniformLocation(m_rendererId, name));
  if (location == -1) {
    std::cout << "Warning: uniform: " << name << "doesn't exist";
  } else {
    m_uniformLocationCache[name] = location;
  }
  return location;
};

void Shader::SetUniform4f(const char *name, float v0, float v1, float v2,
                          float v3) const {
  GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
};

ShaderProgramSource Shader::ParseShader(const char *filepath) {
  FILE *file = fopen(filepath, "r");
  ShaderProgramSource source = {NULL, NULL};
  if (!file) {
    printf("Failed to open shader file: %s\n", filepath);
    return source;
  }

  enum ShaderType type = NONE;
  char *lines[2] = {NULL, NULL};
  size_t lengths[2] = {0, 0};

  char line[512];
  while (fgets(line, sizeof(line), file)) {
    if (strstr(line, "#shader") != NULL) {
      if (strstr(line, "vertex") != NULL) {
        type = VERTEX;
      } else if (strstr(line, "fragment") != NULL) {
        type = FRAGMENT;
      }
    } else {
      if (type != NONE) {
        size_t lineLength = strlen(line);
        if (lines[type] == NULL) {
          lines[type] = (char *)malloc(lineLength + 1);
          strcpy(lines[type], line);
          lengths[type] = lineLength;
        } else {
          lines[type] =
              (char *)realloc(lines[type], lengths[type] + lineLength + 1);
          strcat(lines[type], line);
          lengths[type] += lineLength;
        }
      }
    }
  }

  fclose(file);
  source.VertexSource = lines[VERTEX];
  source.FragmentSource = lines[FRAGMENT];
  return source;
}

unsigned int Shader::CompileShader(unsigned int type, const char *source[]) {
  GLCall(unsigned int id = glCreateShader(type));
  GLCall(glShaderSource(id, 1, source, NULL));
  GLCall(glCompileShader(id));
  int success;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
  if (success == GL_FALSE) {
    const char *typeStr = (type == GL_VERTEX_SHADER)     ? "VERTEX"
                          : (type == GL_FRAGMENT_SHADER) ? "FRAGMENT"
                                                         : "UNKNOWN";
    printf("ERROR:SHADER:%s:::COMPILATION_FAILED\n", typeStr);
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char message[length];
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    printf("%s \n", message);

    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

unsigned int Shader::CreateShader(const char vertextShader[],
                                  const char fragmentShader[]) {
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, &vertextShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, &fragmentShader);

  if (vs == 0 || fs == 0) {
    if (vs != 0)
      GLCall(glDeleteShader(vs));
    if (fs != 0)
      GLCall(glDeleteShader(fs));
    return 0;
  }

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  GLCall(glDeleteShader(fs));
  GLCall(glDeleteShader(vs));

  return program;
}
