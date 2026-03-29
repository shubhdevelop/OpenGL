#pragma once

class Texture {
private:
  unsigned int m_rendererId;
  const char *m_FilePath;
  unsigned char *m_LocalBuffer;
  int m_Width, m_Height, m_BPP;

public:
  Texture(const char *filePath);
  ~Texture();

  void Bind(unsigned int slot = 0);
  void UnBind();

  inline int GetWidth() const { return m_Width; };
  inline int GetHeight() const { return m_Height; };
  inline int GetBPP() const { return m_BPP; };
};
