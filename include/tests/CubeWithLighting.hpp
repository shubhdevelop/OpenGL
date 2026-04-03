#pragma once

#include "Renderer.hpp"
#include <IndexBuffer.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <VertexArray.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <tests/test.hpp>

namespace test {
class CubeWithLighting : public Test {
public:
  CubeWithLighting();
  virtual ~CubeWithLighting();

  virtual void onUpdate(float deltaTime) override;

  virtual void onRender(glm::mat4 view, glm::mat4 projection) override;
  virtual void onImGuiRender() override;

private:
  float m_positions[264];
  unsigned int m_indexes[36];
  VertexBuffer m_vb;
  VertexArray m_va;
  IndexBuffer m_ib;
  VertexBufferLayout m_layout;
  Shader m_shader;
  Shader m_LightShader;
  Renderer m_renderer;
  glm::mat4 m_model;
  glm::mat4 m_rotation;
  glm::vec3 m_translationModelA;
  glm::vec3 m_rotationModelA;
  glm::vec3 m_translationModelLight;
  glm::vec3 m_rotationModelLight;
  glm::vec3 m_scaleModelA;
  glm::vec3 m_scaleModelLight;
  glm::vec4 m_LightColor;
  glm::vec4 m_objectColor;
  float m_time;
  float m_ambient;
  float m_specularStrength;
};
}; // namespace test
