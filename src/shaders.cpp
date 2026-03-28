#include "glad/glad.h"
#include <stddef.h>
#include <stdio.h>

#include <shaders/shaders.h>
#include <stdlib.h>
#include <string.h>

enum ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

ShaderProgramSource ParseShader(const char *filepath) {
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

unsigned int compileShader(unsigned int type, const char *source[]) {
  unsigned int id = glCreateShader(type);
  glShaderSource(id, 1, source, NULL);
  glCompileShader(id);
  int success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    const char *typeStr = (type == GL_VERTEX_SHADER)     ? "VERTEX"
                          : (type == GL_FRAGMENT_SHADER) ? "FRAGMENT"
                                                         : "UNKNOWN";
    printf("ERROR:SHADER:%s:::COMPILATION_FAILED\n", typeStr);
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char message[length];
    glGetShaderInfoLog(id, length, &length, message);
    printf("%s \n", message);

    glDeleteShader(id);
    return 0;
  }

  return id;
}

unsigned int CreateShader(const char vertextShader[],
                          const char fragmentShader[]) {
  unsigned int program = glCreateProgram();
  unsigned int vs = compileShader(GL_VERTEX_SHADER, &vertextShader);
  unsigned int fs = compileShader(GL_FRAGMENT_SHADER, &fragmentShader);

  if (vs == 0 || fs == 0) {
    if (vs != 0)
      glDeleteShader(vs);
    if (fs != 0)
      glDeleteShader(fs);
    return 0;
  }

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(fs);
  glDeleteShader(vs);

  return program;
}
