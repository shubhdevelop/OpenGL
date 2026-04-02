
#include <IndexBuffer.hpp>
#include <VertexBuffer.hpp>
#include <glad/glad.h>
// Window needs to be after glad
#include <Window.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <tests/WaterEffect.hpp>

namespace test {
WaterEffect::WaterEffect()
      : m_positions{
            -0.9f, -0.9f, 0.0f, 0.0f, // bottom left
             0.9f, -0.9f, 1.0f, 0.0f,  // bottom right
             0.9f,  0.9f, 1.0f, 1.0f,   // top
            -0.9f,  0.9f, 0.0f, 1.0f   // top left
        },
        m_indexes{0, 1, 2, 2, 3, 0},
        m_vb(m_positions, 4 * 4 * sizeof(float)),
        m_ib(m_indexes, 6),
        m_shader("resource/shaders/water.shader"),
        m_texture("resource/water_texture.jpg"),
        m_Freq(glm::vec2(1,1)),
        m_Amp(glm::vec2(0.2,0.2))
  {
  m_layout.Push<float>(2);
  m_layout.Push<float>(2);
  m_va.AddBuffer(m_vb, m_layout);

  GLCall(glEnable(GL_DEPTH_TEST));


  m_shader.Bind();

  m_texture.Bind(0);
  m_shader.SetUniform1i("u_Texture", 0);

  m_va.Unbind();
  m_vb.Unbind();
  m_ib.Unbind();
  m_shader.UnBind();

  m_translationModelA = glm::vec3(0, 0, 0);
};

WaterEffect::~WaterEffect() {};

void WaterEffect::onUpdate(float deltaTime) {};

void WaterEffect::onRender(glm::mat4 view, glm::mat4 projection) {

  m_shader.Bind();
  m_texture.Bind(0);

  {

    m_model = glm::translate(glm::mat4(1.0f), m_translationModelA);

    m_shader.SetUniformMat4f("u_Proj", projection);
    m_shader.SetUniformMat4f("u_View", view);
    m_shader.SetUniformMat4f("u_Model", m_model);

    m_shader.SetUniform1f("u_Time", glfwGetTime());
    m_shader.SetUniform4f("u_Freq_Amp", m_Freq[0], m_Freq[1], m_Amp[0],
                          m_Amp[1]);
    m_renderer.Draw(m_va, m_ib, m_shader);
  }
};

void WaterEffect::onImGuiRender() {
  ImGui::Begin("Water Effect!");
  ImGui::SliderFloat3("ModelTranslation", &m_translationModelA.x, -1.0f, 1.0f);
  ImGui::SliderFloat2("Water Frequency", &m_Freq.x, 0.0f, 100.0f);
  ImGui::SliderFloat2("Water Amplititude", &m_Amp.x, 0.0f, 2.0f);
  ImGui::End();
};
}; // namespace test
