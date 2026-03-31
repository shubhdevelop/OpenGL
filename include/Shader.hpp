#pragma once
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

enum ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

typedef struct {
  char *VertexSource;
  char *FragmentSource;
} ShaderProgramSource;

class Shader {

private:
  unsigned int m_rendererId;
  const char *filepath;
  mutable std::unordered_map<std::string, unsigned int> m_uniformLocationCache;

public:
  Shader(const char *filepath);
  ~Shader();
  void Bind() const;
  void UnBind() const;
  void SetUniform4f(const char *name, float v0, float v1, float v2,
                    float v3) const;
  void SetUniform1i(const char *name, int v0) const;
  void SetUniform1f(const char *name, float v0) const;
  void SetUniformMat4f(const char *name, const glm::mat4 &matrix) const;
  unsigned int GetUniformLocation(const char *name) const;

private:
  ShaderProgramSource ParseShader(const char *filepath);
  unsigned int CompileShader(unsigned int type, const char *source[]);
  unsigned int CreateShader(const char vertextShader[],
                            const char fragmentShader[]);
};
