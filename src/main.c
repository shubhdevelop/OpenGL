#include <glad/glad.h>
// IMPORTANT: GLAD must come before GLFW
#include <GLFW/glfw3.h>
#include <shaders/shaders.h>
#include <stdio.h>

float position[6] = {
    0.0f,  0.5f,    // top
    -0.5f, -0.366f, // bottom left
    0.5f,  -0.366f  // bottom right
};

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const char *VertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec4 position;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = position;\n"
                                 "}\0";

const char *FragmentShaderSource = "#version 330 core\n"
                                   "\n"
                                   "layout (location = 0) out vec4 color;"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
                                   "}\0";

int main(void) {
  // initialize a GLFW window
  GLFWwindow *window;
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  printf("%s\n", glGetString(GL_VERSION));
  printf("%s\n", glGetString(GL_VENDOR));
  printf("%s\n", glGetString(GL_RENDERER));

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  unsigned int buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                        (const void *)0);
  glEnableVertexAttribArray(0);

  unsigned int shader = CreateShader(VertexShaderSource, FragmentShaderSource);
  glUseProgram(shader);

  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);

    // rendering command
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
