#include <GlError/glError.h>
#include <IndexBuffer.hpp>
#include <glad/glad.h>

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
    : m_count(count) {
  ASSERT(sizeof(unsigned int) == sizeof(GLuint));
  GLCall(glGenBuffers(m_count, &m_rendererId));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
                      data, GL_STATIC_DRAW));
};

IndexBuffer::~IndexBuffer() { GLCall(glDeleteBuffers(m_count, &m_rendererId)) };

void IndexBuffer::Bind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
};

void IndexBuffer::Unbind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
};
