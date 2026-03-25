#include "glad/glad.h"
#include <stddef.h>
#include <stdio.h>

unsigned int compileShader(unsigned int type, const char *source[]) {
  unsigned int id = glCreateShader(type);
  glShaderSource(id, 1, source, NULL);
  glCompileShader(id);
  int success;
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    const char *typeStr = (type == GL_VERTEX_SHADER) ? "VERTEX" : (type == GL_FRAGMENT_SHADER) ? "FRAGMENT" : "UNKNOWN";
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
    if (vs != 0) glDeleteShader(vs);
    if (fs != 0) glDeleteShader(fs);
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
