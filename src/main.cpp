#include <Camera.hpp>
#include <Texture.hpp>
#include <VertexBufferLayout.hpp>
#include <Window.hpp>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui_impl_glfw.h>
#include <tests/CubeGeo.hpp>
#include <tests/CubeWithCamera.hpp>
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

void mouse_callback(GLFWwindow *w, double xpos, double ypos) {
  ImGui_ImplGlfw_CursorPosCallback(w, xpos, ypos);
  auto &camera = Camera::getInstance();
  if (*camera.getCursorEnabledPtr())
    return;
  if (*camera.getFirstMousePtr()) {
    *camera.getLastXPtr() = (float)xpos;
    *camera.getLastYPtr() = (float)ypos;
    *camera.getFirstMousePtr() = false;
  }
  float xoff = (float)xpos - *camera.getLastXPtr();
  float yoff = *camera.getLastYPtr() - (float)ypos;
  *camera.getLastXPtr() = (float)xpos;
  *camera.getLastYPtr() = (float)ypos;
  camera.processMouse(xoff, yoff);
}

void scroll_callback(GLFWwindow *w, double xoff, double yoff) {
  ImGui_ImplGlfw_ScrollCallback(w, xoff, yoff);
  if (!*Camera::getInstance().getCursorEnabledPtr()) {
    Camera::getInstance().processScroll((float)yoff);
  }
}

static bool cursorEnabled = true;
static bool keyPressed = false;

int main(void) {
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

    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    glfwSetMouseButtonCallback(window.getWindow(), ImGui_ImplGlfw_MouseButtonCallback);
    glfwSetScrollCallback(window.getWindow(), scroll_callback);

    glfwSetKeyCallback(window.getWindow(), [](GLFWwindow *w, int key,
                                              int scancode, int action,
                                              int mods) {
      ImGui_ImplGlfw_KeyCallback(w, key, scancode, action, mods);
      auto &camera = Camera::getInstance();
      camera.setKeyState(key, action == GLFW_PRESS || action == GLFW_REPEAT);
    });

    test::TestE2E e2eTest;
    test::WaterEffect waterEffect;
    test::CubeGeo cubeGeo;
    test::CUbeWithCamera cubeWithCamera;

    int currentTest = 3; // 0=E2E, 1=Water, 2=Cube, 3=CubeWithCamera

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!window.ShouldClose()) {
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
      const char *items[] = {"E2E Test", "Water Effect", "Cube Geo",
                             "Cube With Camera"};
      ImGui::Combo("Select Test", &currentTest, items, 4);

      auto &camera = Camera::getInstance();

      if (glfwGetKey(window.getWindow(), GLFW_KEY_TAB) == GLFW_PRESS) {
        if (!keyPressed) {
          cursorEnabled = !cursorEnabled;
          if (cursorEnabled) {
            glfwSetInputMode(window.getWindow(), GLFW_CURSOR,
                             GLFW_CURSOR_NORMAL);
          } else {
            glfwSetInputMode(window.getWindow(), GLFW_CURSOR,
                             GLFW_CURSOR_DISABLED);
            *camera.getFirstMousePtr() = true;
          }
          keyPressed = true;
        }
      } else {
        keyPressed = false;
      }

      *camera.getCursorEnabledPtr() = cursorEnabled;

      if (true) {
        camera.update(deltaTime);
      }

      ImGui::End();

      renderer.Clear();

      if (currentTest == 0) {
        e2eTest.onRender(camera.getViewMatrix(),
                         camera.getProjectionMatrix(960.0f / 540.0f));
        e2eTest.onImGuiRender();
      } else if (currentTest == 1) {
        waterEffect.onRender(camera.getViewMatrix(),
                             camera.getProjectionMatrix(960.0f / 540.0f));
        waterEffect.onImGuiRender();
      } else if (currentTest == 2) {
        cubeGeo.onUpdate(deltaTime);
        cubeGeo.onRender(camera.getViewMatrix(),
                         camera.getProjectionMatrix(960.0f / 540.0f));
        cubeGeo.onImGuiRender();
      } else if (currentTest == 3) {
        cubeWithCamera.onUpdate(deltaTime);
        cubeWithCamera.onRender(camera.getViewMatrix(),
                                camera.getProjectionMatrix(960.0f / 540.0f));
        cubeWithCamera.onImGuiRender();
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
