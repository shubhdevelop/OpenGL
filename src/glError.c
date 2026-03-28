#include "glad/glad.h"
#include <GlError/glError.h>
#include <stdbool.h>
#include <stdio.h>

void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
};

bool GLLogCall(const char *function, const char *file, int line) {
  GLenum error;
  while ((error = glGetError())) {
    printf("[Function]: %s at [Line]: %d, in [File]: %s, [OpenGL Error] %u.\n",
           function, line, file, error);
    return false;
  }
  return true;
}
