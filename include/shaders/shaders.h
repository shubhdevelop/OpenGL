typedef struct {
    char* VertexSource;
    char* FragmentSource;
} ShaderProgramSource;

ShaderProgramSource ParseShader(const char* filepath);

unsigned int compileShader(unsigned int type, const char *source[]);
unsigned int CreateShader(const char vertextShader[],
                          const char fragmentShader[]);
