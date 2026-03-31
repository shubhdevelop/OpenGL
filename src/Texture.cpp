#include <Renderer.hpp>
#include <Texture.hpp>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

Texture::Texture(const char *filePath)
    : m_rendererId(0), m_FilePath(filePath), m_Width(0), m_Height(0), m_BPP(0) {

  stbi_set_flip_vertically_on_load(true);
  m_LocalBuffer = stbi_load(m_FilePath, &m_Width, &m_Height, &m_BPP, 4);
  GLCall(glGenTextures(1, &m_rendererId));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA,
                      GL_UNSIGNED_BYTE, m_LocalBuffer));

  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
  if (m_LocalBuffer) {
    stbi_image_free(m_LocalBuffer);
  }
}
Texture::~Texture() { GLCall(glDeleteTextures(1, &m_rendererId)) };

void Texture::Bind(unsigned int slot) {
  GLCall(glActiveTexture(GL_TEXTURE0 + slot));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
}

void Texture::UnBind() { GLCall(glBindTexture(GL_TEXTURE_2D, 0)) }
