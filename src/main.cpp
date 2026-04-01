#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <tests/WaterEffect.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <VertexBufferLayout.hpp>
#include <Window.hpp>
#include <glad/glad.h>
#include <tests/TestE2E.hpp>
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

int main(void) {
  {
    Window window(960, 540, "OpenGl Window");
    window.HandleWindowResize();
    window.PrintInfo();

    // Enable GL_BLENDING
    GLCall(glEnable(GL_BLEND));
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

    while (!window.ShouldClose()) {
      window.ProcessInput();
      renderer.Clear();

      // ImGuiStuff
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      // waterEffect
      waterEffect.onUpdate(0.0f);
      waterEffect.onRender();
      waterEffect.onImGuiRender();
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
