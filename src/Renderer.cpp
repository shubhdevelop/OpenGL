#include <IndexBuffer.hpp>
#include <Renderer.hpp>
#include <Shader.hpp>
#include <VertexArray.hpp>
#include <glad/glad.h>
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

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib,
                    const Shader &shader) const {
  shader.Bind();
  va.Bind();
  ib.Bind();
  GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, NULL));
}

void Renderer::Clear() const { GLCall(glClear(GL_COLOR_BUFFER_BIT)); }
