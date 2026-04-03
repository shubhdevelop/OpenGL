#include "tests/CubeWithLighting.hpp"
#include <Camera.hpp>
#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"
#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace test {
CubeWithLighting::CubeWithLighting()
    : Test(),
      m_positions{
          // Front face (normals pointing +Z)
          -1.0f,
          -1.0f,
          1.0f,
          0.0f,
          0.0f,
          1.0f,
          1.0f,
          -1.0f,
          1.0f,
          0.0f,
          0.0f,
          1.0f,
          1.0f,
          1.0f,
          1.0f,
          0.0f,
          0.0f,
          1.0f,
          -1.0f,
          1.0f,
          1.0f,
          0.0f,
          0.0f,
          1.0f,
          // Back face (normals pointing -Z)
          -1.0f,
          -1.0f,
          -1.0f,
          0.0f,
          0.0f,
          -1.0f,
          1.0f,
          -1.0f,
          -1.0f,
          0.0f,
          0.0f,
          -1.0f,
          1.0f,
          1.0f,
          -1.0f,
          0.0f,
          0.0f,
          -1.0f,
          -1.0f,
          1.0f,
          -1.0f,
          0.0f,
          0.0f,
          -1.0f,
          // Top face (normals pointing +Y)
          -1.0f,
          1.0f,
          1.0f,
          0.0f,
          1.0f,
          0.0f,
          1.0f,
          1.0f,
          1.0f,
          0.0f,
          1.0f,
          0.0f,
          1.0f,
          1.0f,
          -1.0f,
          0.0f,
          1.0f,
          0.0f,
          -1.0f,
          1.0f,
          -1.0f,
          0.0f,
          1.0f,
          0.0f,
          // Bottom face (normals pointing -Y)
          -1.0f,
          -1.0f,
          1.0f,
          0.0f,
          -1.0f,
          0.0f,
          1.0f,
          -1.0f,
          1.0f,
          0.0f,
          -1.0f,
          0.0f,
          1.0f,
          -1.0f,
          -1.0f,
          0.0f,
          -1.0f,
          0.0f,
          -1.0f,
          -1.0f,
          -1.0f,
          0.0f,
          -1.0f,
          0.0f,
          // Right face (normals pointing +X)
          1.0f,
          -1.0f,
          1.0f,
          1.0f,
          0.0f,
          0.0f,
          1.0f,
          -1.0f,
          -1.0f,
          1.0f,
          0.0f,
          0.0f,
          1.0f,
          1.0f,
          -1.0f,
          1.0f,
          0.0f,
          0.0f,
          1.0f,
          1.0f,
          1.0f,
          1.0f,
          0.0f,
          0.0f,
          // Left face (normals pointing -X)
          -1.0f,
          -1.0f,
          1.0f,
          -1.0f,
          0.0f,
          0.0f,
          -1.0f,
          -1.0f,
          -1.0f,
          -1.0f,
          0.0f,
          0.0f,
          -1.0f,
          1.0f,
          -1.0f,
          -1.0f,
          0.0f,
          0.0f,
          -1.0f,
          1.0f,
          1.0f,
          -1.0f,
          0.0f,
          0.0f,
      },
      m_indexes{// Front face
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
      m_vb(m_positions, 24 * 6 * sizeof(float)), m_ib(m_indexes, 36),
      m_shader("resource/shaders/cubeWithLighting.shader.glsl"),
      m_LightShader("resource/shaders/Light.shader.glsl"), m_renderer(),
      m_LightColor(1.0f, 1.0f, 1.0f, 1.0f),
      m_objectColor(1.0f, 0.5f, 0.3f, 1.0f) {
  m_layout.Push<float>(3);
  m_layout.Push<float>(3);
  m_va.AddBuffer(m_vb, m_layout);

  GLCall(glEnable(GL_DEPTH_TEST));

  m_shader.Bind();
  m_LightShader.Bind();

  m_va.Unbind();
  m_vb.Unbind();
  m_ib.Unbind();
  m_shader.UnBind();
  m_LightShader.UnBind();

  m_translationModelA = glm::vec3(0, 0, 0);
  m_rotationModelA = glm::vec3(0, 0, 0);
  m_translationModelLight = glm::vec3(0, 0, 0);
  m_rotationModelLight = glm::vec3(0, 0, 0);
  m_rotation = glm::mat4(1.0f);
  m_time = 0.0f;
  m_translationModelLight = glm::vec3(2.0f, 2.0f, 2.0f);
  m_scaleModelA = glm::vec3(1.0f, 1.0f, 1.0f);
  m_scaleModelLight = glm::vec3(1.0f, 1.0f, 1.0f);
  m_ambient = 0.1f;
  m_specularStrength = 0.5f;
};

CubeWithLighting::~CubeWithLighting() {};

void CubeWithLighting::onUpdate(float deltaTime) { m_time += deltaTime; };

void CubeWithLighting::onRender(glm::mat4 view, glm::mat4 projection) {
  GLCall(glEnable(GL_DEPTH_TEST));
  glm::mat4 modelRotation =
      glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationModelA.x),
                  glm::vec3(1.0f, 0.0f, 0.0f));
  modelRotation = glm::rotate(modelRotation, glm::radians(m_rotationModelA.y),
                              glm::vec3(0.0f, 1.0f, 0.0f));
  modelRotation = glm::rotate(modelRotation, glm::radians(m_rotationModelA.z),
                              glm::vec3(0.0f, 0.0f, 1.0f));

  glm::mat4 modelRotationLight =
      glm::rotate(glm::mat4(1.0f), glm::radians(m_rotationModelLight.x),
                  glm::vec3(1.0f, 0.0f, 0.0f));
  modelRotationLight =
      glm::rotate(modelRotationLight, glm::radians(m_rotationModelLight.y),
                  glm::vec3(0.0f, 1.0f, 0.0f));
  modelRotationLight =
      glm::rotate(modelRotationLight, glm::radians(m_rotationModelLight.z),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  {
    m_shader.Bind();
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_translationModelA);
    m_model = m_model * modelRotation;
    m_model = glm::scale(m_model, m_scaleModelA);

    m_shader.SetUniformMat4f("u_Proj", projection);
    m_shader.SetUniformMat4f("u_View", view);
    m_shader.SetUniformMat4f("u_Model", m_model);
    m_shader.SetUniform1f("u_Time", m_time);
    m_shader.SetUniform1f("u_Ambient", m_ambient);
    m_shader.SetUniform4f("u_ObjectColor", m_objectColor.r, m_objectColor.g,
                          m_objectColor.b, m_objectColor.a);
    m_shader.SetUniform4f("u_LightColor", m_LightColor.r, m_LightColor.g,
                          m_LightColor.b, m_LightColor.a);
    m_shader.SetUniform3f("u_LightPos", m_translationModelLight.x,
                          m_translationModelLight.y, m_translationModelLight.z);
    glm::vec3 cameraPos = Camera::getInstance().getPosition();
    m_shader.SetUniform3f("u_ViewPos", cameraPos.x, cameraPos.y, cameraPos.z);
    m_shader.SetUniform1f("u_SpecularStrength", m_specularStrength);
    m_renderer.Draw(m_va, m_ib, m_shader);

    m_LightShader.Bind();
    m_LightShader.SetUniformMat4f("u_Proj", projection);
    m_LightShader.SetUniformMat4f("u_View", view);
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_translationModelLight);
    m_model = m_model * modelRotationLight;
    m_model = glm::scale(m_model, m_scaleModelLight);
    m_LightShader.SetUniformMat4f("u_Model", m_model);
    m_LightShader.SetUniform4f("u_LightColor", m_LightColor.r, m_LightColor.g,
                               m_LightColor.b, m_LightColor.a);
    m_renderer.Draw(m_va, m_ib, m_LightShader);
  }
};

void CubeWithLighting::onImGuiRender() {
  ImGui::Begin("Settings");
  ImGui::Text("Global Settings");
  ImGui::SliderFloat("Ambient Light", &m_ambient, -1.0f, 1.0f);
  ImGui::SliderFloat("Specular Strength", &m_specularStrength, 0.0f, 1.0f);
  ImGui::Separator();
  ImGui::Text("Light Color Transfomation");
  ImGui::SliderFloat3("Model Translation", &m_translationModelA.x, -10.0f,
                      10.0f);
  ImGui::SliderFloat3("Model Rotation", &m_rotationModelA.x, -180.0f, 180.0f);
  ImGui::SliderFloat3("Model Scale", &m_scaleModelA.x, 0.1f, 5.0f);
  ImGui::Separator();
  ImGui::Text("Light Model Transfomation");
  ImGui::SliderFloat3("Model Translation light", &m_translationModelLight.x,
                      -10.0f, 10.0f);
  ImGui::SliderFloat3("Model Rotation Light", &m_rotationModelLight.x, -180.0f,
                      180.0f);
  ImGui::SliderFloat3("Light Scale", &m_scaleModelLight.x, 0.1f, 5.0f);
  ImGui::Text("Light Color Transfomation");
  ImGui::ColorPicker4("Light Color", &m_LightColor.r,
                      ImGuiColorEditFlags_InputRGB);
  ImGui::ColorPicker4("Object color", &m_objectColor.r,
                      ImGuiColorEditFlags_InputRGB);
  ImGui::End();
};
}; // namespace test
