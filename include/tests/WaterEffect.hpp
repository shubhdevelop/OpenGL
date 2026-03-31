#pragma once

#include <IndexBuffer.hpp>
#include <Renderer.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <VertexArray.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <tests/test.hpp>

namespace test
{
  class WaterEffect : public Test
  {
  public:
    WaterEffect();
    virtual ~WaterEffect();

    virtual void onUpdate(float deltaTime) override;

    virtual void onRender() override;
    virtual void onImGuiRender() override;

  private:
    float m_positions[16];
    unsigned int m_indexes[6];
    VertexBuffer m_vb;
    VertexArray m_va;
    IndexBuffer m_ib;
    VertexBufferLayout m_layout;
    Shader m_shader;
    Texture m_texture;
    Renderer m_renderer;
    glm::mat4 m_proj;
    glm::mat4 m_view;
    glm::mat4 m_model;
    glm::vec3 m_translationViewA;
    glm::vec3 m_translationModelA;
  };
}; // namespace test
