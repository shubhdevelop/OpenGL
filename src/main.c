#include <glad/glad.h>
// IMPORTANT: GLAD must come before GLFW
#include <GLFW/glfw3.h>
#include <GlError/glError.h>
#include <shaders/shaders.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

float positions[8] = {
    -0.5f, -0.5f, // bottom left
    0.5f,  -0.5f, // bottom right
    0.5f,  0.5f,  // top
    -0.5f, 0.5f,  // top left
};

unsigned int indices[6] = {0, 1, 2, 2, 3, 0};

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main(void) {
  // initialize a GLFW window
  GLFWwindow *window;
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  printf("%s\n", glGetString(GL_VERSION));
  printf("%s\n", glGetString(GL_VENDOR));
  printf("%s\n", glGetString(GL_RENDERER));

  GLCall(glViewport(0, 0, 800, 600));
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  unsigned int vao;
  GLCall(glGenVertexArrays(1, &vao));
  GLCall(glBindVertexArray(vao));

  unsigned int buffer;
  GLCall(glGenBuffers(1, &buffer));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
  GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions,
                      GL_STATIC_DRAW));
  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                               (const void *)0));

  unsigned int ibo;
  GLCall(glGenBuffers(1, &ibo));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int),
                      indices, GL_STATIC_DRAW));

  ShaderProgramSource source = ParseShader("resource/shaders/Basic.shader");
  unsigned int shader =
      CreateShader(source.VertexSource, source.FragmentSource);
  GLCall(glUseProgram(shader));

  if (source.VertexSource)
    free(source.VertexSource);
  if (source.FragmentSource)
    free(source.FragmentSource);

  int u_Color_Location = glGetUniformLocation(shader, "u_Color");
  ASSERT(u_Color_Location != -1);

  GLCall(glBindVertexArray(0));
  GLCall(glUseProgram(0));
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

  float r = 0.0f;
  float increment = 0.05f;
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    GLCall(glUseProgram(shader));
    GLCall(glUniform4f(u_Color_Location, r, 0.3f, 0.7f, 0.8f));

    GLCall(glBindVertexArray(vao));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

    GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL));

    if (r > 1.0f) {
      increment = -0.01f;
    } else if (r < 0.0f) {
      increment = 0.01f;
    }
    r += increment;
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
  GLCall(glViewport(0, 0, width, height));
}
