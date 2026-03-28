#include <GlError/glError.h>
#include <VertexBuffer.hpp>
#include <glad/glad.h>

VertexBuffer::VertexBuffer(const void *data, int size) {
  GLCall(glGenBuffers(1, &m_rendererId));
  Bind();
  GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
};

VertexBuffer::~VertexBuffer() { GLCall(glDeleteBuffers(1, &m_rendererId)) };

void VertexBuffer::Bind() const {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererId));
};
void VertexBuffer::Unbind() const { GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); };
