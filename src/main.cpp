#include <Shader.hpp>
#include <Texture.hpp>
#include <VertexBufferLayout.hpp>
#include <Window.hpp>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <tests/CubeGeo.hpp>
#include <tests/TestE2E.hpp>
#include <tests/WaterEffect.hpp>
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
#include <chrono>
#include <stdlib.h>

int main(void)
{
  {
    Window window(960, 540, "OpenGl Window");
    window.HandleWindowResize();
    window.PrintInfo();

    // Enable GL_BLENDING
    GLCall(glEnable(GL_BLEND));
    GLCall(glEnable(GL_DEPTH_TEST));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Renderer renderer;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    test::TestE2E e2eTest;
    test::WaterEffect waterEffect;
    test::CubeGeo cubeGeo;

    int currentTest = 2; // 0=E2E, 1=Water, 2=Cube

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!window.ShouldClose())
    {
      auto currentTime = std::chrono::high_resolution_clock::now();
      float deltaTime =
          std::chrono::duration<float>(currentTime - lastTime).count();
      lastTime = currentTime;

      window.ProcessInput();
      renderer.Clear();

      // ImGuiStuff
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      ImGui::Begin("Test Selector");
      const char* items[] = { "E2E Test", "Water Effect", "Cube Geo" };
      ImGui::Combo("Select Test", &currentTest, items, 3);
      ImGui::End();

      renderer.Clear();

      if (currentTest == 0) {
        e2eTest.onRender();
        e2eTest.onImGuiRender();
      } else if (currentTest == 1) {
        waterEffect.onRender();
        waterEffect.onImGuiRender();
      } else if (currentTest == 2) {
        cubeGeo.onUpdate(deltaTime);
        cubeGeo.onRender();
        cubeGeo.onImGuiRender();
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
