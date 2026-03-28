#include "VertexBufferLayout.hpp"
#include "Window.hpp"
#include <glad/glad.h>
// IMPORTANT: GLAD must come before GLFW
#include <GLFW/glfw3.h>
#include <GlError/glError.h>
#include <IndexBuffer.hpp>
#include <VertexArray.hpp>
#include <iostream>
#include <shaders/shaders.h>
#include <stdio.h>
#include <stdlib.h>

float positions[8] = {
    -0.5f, -0.5f, // bottom left
    0.5f,  -0.5f, // bottom right
    0.5f,  0.5f,  // top
    -0.5f, 0.5f,  // top left
};

unsigned int indices[6] = {0, 1, 2, 2, 3, 0};

int main(void) {
  {
    Window w(800, 600, "OpenGl Window");
    if (!w.isValid()) {
      std::cerr << "Window construction failed!" << std::endl;
      return -1;
    }
    w.HandleWindowResize();
    w.PrintInfo();

    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    IndexBuffer ib(indices, 6);
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    ShaderProgramSource source = ParseShader("resource/shaders/Basic.shader");
    unsigned int shader =
        CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));

    if (source.VertexSource)
      free(source.VertexSource);
    if (source.FragmentSource)
      free(source.FragmentSource);

    int u_Color_Location = glGetUniformLocation(shader, "u_Color");
    ASSERT(u_Color_Location != -1);

    float r = 0.0f;
    float increment = 0.05f;

    while (!w.ShouldClose()) {
      w.ProcessInput();
      GLCall(glClear(GL_COLOR_BUFFER_BIT));
      GLCall(glUseProgram(shader));
      GLCall(glUniform4f(u_Color_Location, r, 0.3f, 0.7f, 0.8f));
      va.Bind();
      ib.Bind();
      GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

      if (r > 1.0f) {
        increment = -0.01f;
      } else if (r < 0.0f) {
        increment = 0.01f;
      }
      r += increment;

      w.SwapBuffer();
      w.PollEvents();
    }
  }
  return 0;
}
