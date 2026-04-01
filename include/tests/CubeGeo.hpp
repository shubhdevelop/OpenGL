#pragma once

#include <IndexBuffer.hpp>
#include <Renderer.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <VertexArray.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <tests/test.hpp>

namespace test {
class CubeGeo : public Test {
public:
  CubeGeo();
  virtual ~CubeGeo();

  virtual void onUpdate(float deltaTime) override;

  virtual void onRender() override;
  virtual void onImGuiRender() override;

private:
  float m_positions[192];
  unsigned int m_indexes[36];
  VertexBuffer m_vb;
  VertexArray m_va;
  IndexBuffer m_ib;
  VertexBufferLayout m_layout;
  Shader m_shader;
  Texture m_texture;
  Renderer m_renderer;
  glm::mat4 m_proj_perspective;
  glm::mat4 m_view;
  glm::mat4 m_model;
  glm::mat4 m_rotation;
  glm::vec3 m_translationViewA;
  glm::vec3 m_translationModelA;
  glm::vec3 m_rotationModelA;
  glm::vec3 m_rotationViewA;
  glm::vec2 m_Freq;
  glm::vec2 m_Amp;
  float m_time;
};
}; // namespace test
