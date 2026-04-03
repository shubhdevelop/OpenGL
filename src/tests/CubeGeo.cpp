#include "tests/CubeGeo.hpp"
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "glad/glad.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <tests/TestE2E.hpp>

namespace test
{
  CubeGeo::CubeGeo()
      : m_positions{
            // Front face
            -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
             1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            // Back face
            -1.0f, -1.0f, -3.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
             1.0f, -1.0f, -3.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
             1.0f,  1.0f, -3.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
            -1.0f,  1.0f, -3.0f,  0.5f, 0.5f, 0.5f,  1.0f, 1.0f,
            // Top face
            -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
             1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, -3.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
            -1.0f,  1.0f, -3.0f,  0.5f, 0.5f, 0.5f,  0.0f, 1.0f,
            // Bottom face
            -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
             1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
             1.0f, -1.0f, -3.0f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, -3.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
            // Right face
             1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
             1.0f, -1.0f, -3.0f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, -3.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
             1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
            // Left face
            -1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, -3.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
            -1.0f,  1.0f, -3.0f,  0.5f, 0.5f, 0.5f,  0.0f, 1.0f,
            -1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        },
        m_indexes{
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            4, 5, 6, 6, 7, 4,
            // Top face
            8, 9, 10, 10, 11, 8,
            // Bottom face
            12, 13, 14, 14, 15, 12,
            // Right face
            16, 17, 18, 18, 19, 16,
            // Left face
            20, 21, 22, 22, 23, 20},
        m_vb(m_positions, 24 * 8 * sizeof(float)),
        m_ib(m_indexes, 36),
        m_shader("resource/shaders/cubeWaterTexture.shader.glsl"),
        m_texture("resource/water_texture.jpg"),
        m_renderer()
  {
  m_layout.Push<float>(3);
  m_layout.Push<float>(3);
  m_layout.Push<float>(2);
  m_va.AddBuffer(m_vb, m_layout);

  GLCall(glEnable(GL_DEPTH_TEST));

  m_shader.Bind();

  m_va.Unbind();
  m_vb.Unbind();
  m_ib.Unbind();
  m_shader.UnBind();

  m_translationModelA = glm::vec3(0, 0, 0);
  m_rotationModelA = glm::vec3(0, 0, 0);
  m_rotation = glm::mat4(1.0f);
  m_Freq = glm::vec2(1.0f, 1.0f);
  m_Amp = glm::vec2(0.1f, 0.1f);
  m_time = 0.0f;
  };

  CubeGeo::~CubeGeo() {};

  void CubeGeo::onUpdate(float deltaTime)
  {
    m_time += deltaTime;
  };

  void CubeGeo::onRender(glm::mat4 view, glm::mat4 projection)
  {
    m_shader.Bind();
    m_texture.Bind();
    glm::mat4 modelRotation =
        glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationModelA.x),
                    glm::vec3(1.0f, 0.0f, 0.0f));
    modelRotation = glm::rotate(modelRotation, glm::radians(m_rotationModelA.y),
                                glm::vec3(0.0f, 1.0f, 0.0f));
    modelRotation = glm::rotate(modelRotation, glm::radians(m_rotationModelA.z),
                                glm::vec3(0.0f, 0.0f, 1.0f));
    {
      m_model = glm::mat4(1.0f);
      m_model = glm::translate(m_model, m_translationModelA);
      m_model = m_model * modelRotation;

      m_shader.SetUniform1i("u_Texture", 0);
      m_shader.SetUniformMat4f("u_Proj", projection);
      m_shader.SetUniformMat4f("u_View", view);
      m_shader.SetUniformMat4f("u_Model", m_model);
      m_shader.SetUniform1f("u_Time", m_time);
      m_shader.SetUniform4f("u_Freq_Amp", m_Freq.x, m_Freq.y, m_Amp.x, m_Amp.y);
      m_renderer.Draw(m_va, m_ib, m_shader);
    }
  };

  void CubeGeo::onImGuiRender()
  {
    ImGui::Begin("Model 1");
    ImGui::SliderFloat3("Model Translation", &m_translationModelA.x, -1.0f, 1.0f);
    ImGui::SliderFloat3("Model Rotation", &m_rotationModelA.x, -180.0f, 180.0f);
    ImGui::Separator();
    ImGui::Text("Water Effect");
    ImGui::SliderFloat2("Frequency", &m_Freq.x, 0.0f, 10.0f);
    ImGui::SliderFloat2("Amplitude", &m_Amp.x, 0.0f, 1.0f);
    ImGui::End();
  };
};