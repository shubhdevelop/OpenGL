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
    -50.0f, -50.0f, 0.0f, 0.0f, // bottom left
    50.0f,  -50.0f, 1.0f, 0.0f, // bottom right
    50.0f,  50.0f,  1.0f, 1.0f, // top
    -50.0f, 50.0f,  0.0f, 1.0f  // top left
};

unsigned int indices[6] = {0, 1, 2, 2, 3, 0};

int main(void) {
  {
    Window window(960, 540, "OpenGl Window");
    window.HandleWindowResize();
    window.PrintInfo();

    // Enable GL_BLENDING
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    IndexBuffer ib(indices, 6);
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    Shader shader("resource/shaders/Basic.shader");
    shader.Bind();

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

    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    glm::vec3 translationViewA(0, 0, 0);
    glm::vec3 translationModelA(0, 0, 0);

    glm::vec3 translationViewB(0, 0, 0);
    glm::vec3 translationModelB(0, 0, 0);

    while (!window.ShouldClose()) {
      window.ProcessInput();
      renderer.Clear();

      // ImGuiStuff
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      shader.Bind();

      {

        glm::mat4 view = glm::translate(glm::mat4(1.0f), translationViewA);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationModelA);

        shader.SetUniformMat4f("u_Proj", proj);
        shader.SetUniformMat4f("u_View", view);
        shader.SetUniformMat4f("u_Model", model);
        renderer.Draw(va, ib, shader);

        // the state control for the object 1;
        {
          ImGui::Begin("Object 1");
          ImGui::SliderFloat3("ViewTransaltion", &translationViewA.x, 0.0f,
                              960.0f);
          ImGui::SliderFloat3("ModelTranslation", &translationModelA.x, 0.0f,
                              960.0f);
          ImGui::End();
        }
      }

      {

        glm::mat4 view = glm::translate(glm::mat4(1.0f), translationViewB);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationModelB);
        shader.SetUniformMat4f("u_Proj", proj);
        shader.SetUniformMat4f("u_View", view);
        shader.SetUniformMat4f("u_Model", model);
        renderer.Draw(va, ib, shader);

        // the state control for the object 2;
        {
          ImGui::Begin("Object 2");
          ImGui::SliderFloat3("ViewTransaltion", &translationViewB.x, 0.0f,
                              960.0f);
          ImGui::SliderFloat3("ModelTranslation", &translationModelB.x, 0.0f,
                              960.0f);
          ImGui::End();
        }
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
