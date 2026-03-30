
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <tests/TestE2E.hpp>

namespace test {
TestE2E::TestE2E()
      : m_positions{
            -50.0f, -50.0f, 0.0f, 0.0f, // bottom left
            50.0f, -50.0f, 1.0f, 0.0f,  // bottom right
            50.0f, 50.0f, 1.0f, 1.0f,   // top
            -50.0f, 50.0f, 0.0f, 1.0f   // top left
        },
        m_indexes{0, 1, 2, 2, 3, 0},
        m_vb(m_positions, 4 * 4 * sizeof(float)),
        m_ib(m_indexes, 6),
        m_shader("resource/shaders/Basic.shader"),
        m_texture("resource/pp.jpg")
  {
  m_layout.Push<float>(2);
  m_layout.Push<float>(2);
  m_va.AddBuffer(m_vb, m_layout);

  m_proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

  m_shader.Bind();

  m_texture.Bind(0);
  m_shader.SetUniform1i("u_Texture", 0);

  m_va.Unbind();
  m_vb.Unbind();
  m_ib.Unbind();
  m_shader.UnBind();

  m_translationModelA = glm::vec3(0, 0, 0);
  m_translationViewA = glm::vec3(0, 0, 0);
};

TestE2E::~TestE2E() {};

void TestE2E::onUpdate(float deltaTime) {};

void TestE2E::onRender() {

  m_shader.Bind();

  {

    m_view = glm::translate(glm::mat4(1.0f), m_translationViewA);
    m_model = glm::translate(glm::mat4(1.0f), m_translationModelA);

    m_shader.SetUniformMat4f("u_Proj", m_proj);
    m_shader.SetUniformMat4f("u_View", m_view);
    m_shader.SetUniformMat4f("u_Model", m_model);
    m_renderer.Draw(m_va, m_ib, m_shader);
  }
};

void TestE2E::onImGuiRender() {
  ImGui::Begin("Object 1");
  ImGui::SliderFloat3("ViewTransaltion", &m_translationViewA.x, 0.0f, 960.0f);
  ImGui::SliderFloat3("ModelTranslation", &m_translationModelA.x, 0.0f, 960.0f);
  ImGui::End();
};
}; // namespace test
