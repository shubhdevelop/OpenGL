
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <Renderer.hpp>
#include <Window.hpp>
#include <iostream>

Window::Window(unsigned int width, unsigned int height, const char *title) {
  Wwidth = width;
  Wheight = height;
  
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW" << std::endl;
    return;
  }

  // Set OpenGL version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  
#ifdef __APPLE__
  // macOS specific: use core profile and forward compatibility
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
  // Linux and Windows: use compatibility profile
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#endif

  window = glfwCreateWindow(Wwidth, Wheight, title, NULL, NULL);
  if (!window) {
    std::cout << "Failed to create window" << std::endl;
    glfwTerminate();
    return;
  }
  
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
  }
  
  int framebufferWidth = 0;
  int framebufferHeight = 0;
  glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
  GLCall(glViewport(0, 0, framebufferWidth, framebufferHeight));
  std::cout << "initializing window!!" << std::endl;
}

Window::~Window() { glfwTerminate(); }

void Window::Framebuffer_size_callback(GLFWwindow *window, int width,
                                       int height) {
  GLCall(glViewport(0, 0, width, height));
}

void Window::PrintInfo() {
  printf("%s\n", glGetString(GL_VERSION));
  printf("%s\n", glGetString(GL_VENDOR));
  printf("%s\n", glGetString(GL_RENDERER));
}

void Window::HandleWindowResize() {
  glfwSetFramebufferSizeCallback(window, Framebuffer_size_callback);
}

bool Window::ShouldClose() {
  if (!window) {
    return true;
  }
  return glfwWindowShouldClose(window);
};

void Window::SwapBuffer() { glfwSwapBuffers(getWindow()); }

void Window::ProcessInput() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
}
void Window::PollEvents() { glfwPollEvents(); }
