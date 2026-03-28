#pragma once
#include <GLFW/glfw3.h>

class Window {

private:
  unsigned int Wwidth;
  unsigned int Wheight;
  static void Framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);

public:
  GLFWwindow *window;
  Window(unsigned int width, unsigned int height, const char *title);
  ~Window();
  void Setup();
  GLFWwindow *getWindow() { return window; }
  void HandleWindowResize();
  void PrintInfo();
  bool isValid() { return window != nullptr; };
  bool ShouldClose();
  void ProcessInput();
  void SwapBuffer();
  void PollEvents();
};
