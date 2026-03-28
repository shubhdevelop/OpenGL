#include <Shader.hpp>
#include <VertexBufferLayout.hpp>
#include <Window.hpp>
#include <glad/glad.h>
// IMPORTANT: GLAD must come before GLFW
#include <GLFW/glfw3.h>
#include <IndexBuffer.hpp>
#include <Renderer.hpp>
#include <Shader.hpp>
#include <VertexArray.hpp>
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
    w.HandleWindowResize();
    w.PrintInfo();

    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    IndexBuffer ib(indices, 6);
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    Shader shader("resource/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.7f, 0.8f);

    va.Unbind();
    shader.UnBind();
    vb.Unbind();
    ib.Unbind();

    float r = 0.0f;
    float increment = 0.05f;

    while (!w.ShouldClose()) {
      w.ProcessInput();

      GLCall(glClear(GL_COLOR_BUFFER_BIT));
      shader.Bind();
      shader.SetUniform4f("u_Color", r, 0.3f, 0.7f, 0.8f);
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
