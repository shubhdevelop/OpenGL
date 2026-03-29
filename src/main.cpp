#include "Texture.hpp"
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
#include <stdlib.h>

float positions[16] = {
    -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
    0.5f,  -0.5f, 1.0f, 0.0f, // bottom right
    0.5f,  0.5f,  1.0f, 1.0f, // top
    -0.5f, 0.5f,  0.0f, 1.0f  // top left
};

unsigned int indices[6] = {0, 1, 2, 2, 3, 0};

int main(void) {
  {
    Window window(800, 600, "OpenGl Window");
    window.HandleWindowResize();
    window.PrintInfo();

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    IndexBuffer ib(indices, 6);
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    Shader shader("resource/shaders/Basic.shader");
    shader.Bind();
    // shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.7f, 0.8f);

    Texture Texture("resource/pp.jpg");
    Texture.Bind(0);
    shader.SetUniform1i("u_Texture", 0);

    va.Unbind();
    shader.UnBind();
    vb.Unbind();
    ib.Unbind();

    float r = 0.0f;
    float increment = 0.05f;

    Renderer renderer;
    while (!window.ShouldClose()) {
      window.ProcessInput();
      renderer.Clear();
      shader.Bind();
      // shader.SetUniform4f("u_Color", r, 0.3f, 0.7f, 0.8f);
      renderer.Draw(va, ib, shader);

      if (r > 1.0f) {
        increment = -0.01f;
      } else if (r < 0.0f) {
        increment = 0.01f;
      }
      r += increment;

      window.SwapBuffer();
      window.PollEvents();
    }
  }
  return 0;
}
