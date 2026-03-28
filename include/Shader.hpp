#pragma once
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
  std::unordered_map<std::string, unsigned int> m_uniformLocationCache;

public:
  Shader(const char *filepath);
  ~Shader();
  void Bind();
  void UnBind();
  void SetUniform4f(const char *name, float v0, float v1, float v2, float v3);
  unsigned int GetUniformLocation(const char *name);

private:
  ShaderProgramSource ParseShader(const char *filepath);
  unsigned int CompileShader(unsigned int type, const char *source[]);
  unsigned int CreateShader(const char vertextShader[],
                            const char fragmentShader[]);
};
