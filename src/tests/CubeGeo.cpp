
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

namespace test {
CubeGeo::CubeGeo()
      : m_positions{
            // front face (z = -1)
            -1.0f,
            -1.0f,
            -1.0f,
            1.0f,
            0.0f,
            0.0f, // bottom left - red
            1.0f,
            -1.0f,
            -1.0f,
            0.0f,
            1.0f,
            0.0f, // bottom right - green
            1.0f,
            1.0f,
            -1.0f,
            0.0f,
            0.0f,
            1.0f, // top right - blue
            -1.0f,
            1.0f,
            -1.0f,
            1.0f,
            1.0f,
            0.0f, // top left - yellow
            // Back face (z = -3.0)
            -1.0f,
            -1.0f,
            -3.0f,
            1.0f,
            0.0f,
            1.0f, // magenta
            1.0f,
            -1.0f,
            -3.0f,
            0.0f,
            1.0f,
            1.0f, // cyan
            1.0f,
            1.0f,
            -3.0f,
            1.0f,
            1.0f,
            1.0f, // white
            -1.0f,
            1.0f,
            -3.0f,
            0.5f,
            0.5f,
            0.5f, // gray
        },
        m_indexes{
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            4, 5, 6, 6, 7, 4,
            // Top face
            3, 2, 6, 6, 7, 3,
            // Bottom face
            0, 1, 5, 5, 4, 0,
            // Right face
            1, 5, 6, 6, 2, 1,
            // Left face
            0, 4, 7, 7, 3, 0},
        m_vb(m_positions, 8 * 6 * sizeof(float)),
        m_ib(m_indexes, 36),
        m_shader("resource/shaders/cube.shader.glsl")
  {
  m_layout.Push<float>(3);
  m_layout.Push<float>(3);
  m_va.AddBuffer(m_vb, m_layout);

  GLCall(glEnable(GL_DEPTH_TEST));

  m_proj_perspective =
      glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

  m_shader.Bind();

  m_va.Unbind();
  m_vb.Unbind();
  m_ib.Unbind();
  m_shader.UnBind();

  m_translationModelA = glm::vec3(0, 0, 0);
  m_translationViewA = glm::vec3(0, 0, 0);
  m_rotationModelA = glm::vec3(0, 0, 0);
  m_rotationViewA = glm::vec3(0, 0, 0);
  m_rotation = glm::mat4(1.0f);
};

CubeGeo::~CubeGeo() {};

void CubeGeo::onUpdate(float deltaTime) {};

void CubeGeo::onRender() {

  m_shader.Bind();

  glm::mat4 modelRotation =
      glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationModelA.x),
                  glm::vec3(1.0f, 0.0f, 0.0f));
  modelRotation = glm::rotate(modelRotation, glm::radians(m_rotationModelA.y),
                              glm::vec3(0.0f, 1.0f, 0.0f));
  modelRotation = glm::rotate(modelRotation, glm::radians(m_rotationModelA.z),
                              glm::vec3(0.0f, 0.0f, 1.0f));

  glm::mat4 viewRotation =
      glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationViewA.x),
                  glm::vec3(1.0f, 0.0f, 0.0f));
  viewRotation = glm::rotate(viewRotation, glm::radians(m_rotationViewA.y),
                             glm::vec3(0.0f, 1.0f, 0.0f));
  viewRotation = glm::rotate(viewRotation, glm::radians(m_rotationViewA.z),
                             glm::vec3(0.0f, 0.0f, 1.0f));

  {
    m_view = glm::translate(glm::mat4(1.0f), m_translationViewA);
    m_view = m_view * viewRotation;
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_translationModelA);
    m_model = m_model * modelRotation;

    m_shader.SetUniformMat4f("u_Proj", m_proj_perspective);
    m_shader.SetUniformMat4f("u_View", m_view);
    m_shader.SetUniformMat4f("u_Model", m_model);
    m_renderer.Draw(m_va, m_ib, m_shader);
  }
};

void CubeGeo::onImGuiRender() {
  ImGui::Begin("Object 1");
  ImGui::SliderFloat3("View Translation", &m_translationViewA.x, -50.0f, 1.0f);
  ImGui::SliderFloat3("View Rotation", &m_rotationViewA.x, -180.0f, 180.0f);
  ImGui::Separator();
  ImGui::SliderFloat3("Model Translation", &m_translationModelA.x, -1.0f, 1.0f);
  ImGui::SliderFloat3("Model Rotation", &m_rotationModelA.x, -180.0f, 180.0f);
  ImGui::End();
};
}; // namespace test
