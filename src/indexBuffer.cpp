#include <IndexBuffer.hpp>
#include <Renderer.hpp>
#include <glad/glad.h>

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count)
    : m_count(count) {
  ASSERT(sizeof(unsigned int) == sizeof(GLuint));
  GLCall(glGenBuffers(1, &m_rendererId));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
                      data, GL_STATIC_DRAW));
};

IndexBuffer::~IndexBuffer() { GLCall(glDeleteBuffers(1, &m_rendererId)) };

void IndexBuffer::Bind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId));
};

void IndexBuffer::Unbind() const {
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
};

unsigned int IndexBuffer::GetCount() const { return m_count; }
