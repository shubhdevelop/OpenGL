class VertexBuffer {
private:
  unsigned int m_rendererId;

public:
  VertexBuffer(const void *data, int size);
  ~VertexBuffer();

  void Unbind() const;
  void Bind() const;
};
