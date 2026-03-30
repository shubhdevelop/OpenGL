#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "imgui/imgui_impl_glfw.h"
#include <Shader.hpp>
#include <Texture.hpp>
#include <VertexBufferLayout.hpp>
#include <Window.hpp>
#include <glad/glad.h>
// IMPORTANT: GLAD must come before GLFW
#include <IndexBuffer.hpp>
#include <Renderer.hpp>
#include <Shader.hpp>
#include <VertexArray.hpp>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
// GLFW needs to be after
#include <GLFW/glfw3.h>
#include <stdlib.h>

float positions[16] = {
    100.0f, 100.0f, 0.0f, 0.0f, // bottom left
    200.0f, 100.0f, 1.0f, 0.0f, // bottom right
    200.0f, 200.0f, 1.0f, 1.0f, // top
    100.0f, 200.0f, 0.0f, 1.0f  // top left
};

unsigned int indices[6] = {0, 1, 2, 2, 3, 0};

int main(void) {
  {
    Window window(960, 540, "OpenGl Window");
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

    float viewX = -100, viewY = 0, viewZ = 0;
    float modelX = 200, modelY = 100, modelZ = 0;

    // Projection;
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    // Camera positions;
    Shader shader("resource/shaders/Basic.shader");
    shader.Bind();

    // doing the matrix multiplication on the gpu should be faster!!

    Texture Texture("resource/pp.jpg");
    Texture.Bind(0);
    shader.SetUniform1i("u_Texture", 0);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.UnBind();

    Renderer renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style

    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    glm::vec3 translationView(200, 200, 0);
    glm::vec3 translationModel(200, 200, 0);
    float r = 0.0f;
    float increment = 0.05f;

    while (!window.ShouldClose()) {
      window.ProcessInput();
      renderer.Clear();

      // ImGuiStuff
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      shader.Bind();
      // shader.SetUniform4f("u_Color", r, 0.3f, 0.7f, 0.8f);
      glm::mat4 view = glm::translate(glm::mat4(1.0f), translationView);
      // Camera positions rotation and movement and scale;
      glm::mat4 model = glm::translate(glm::mat4(1.0f), translationModel);

      shader.SetUniformMat4f("u_Proj", proj);
      shader.SetUniformMat4f("u_View", view);
      shader.SetUniformMat4f("u_Model", model);
      renderer.Draw(va, ib, shader);

      if (r > 1.0f) {
        increment = -0.01f;
      } else if (r < 0.0f) {
        increment = 0.01f;
      }
      r += increment;
      {
        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
                                       // and append into it.
        ImGui::SliderFloat3(
            "ViewTransaltion", &translationView.x, 0.0f,
            960.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::SliderFloat3(
            "ModelTranslation", &translationModel.x, 0.0f,
            540.0f); // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::End();
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      window.SwapBuffer();
      window.PollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  return 0;
}
