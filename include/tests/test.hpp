#pragma once

#include <glm/glm.hpp>

namespace test {
class Test {
public:
  Test();
  virtual ~Test();

  virtual void onUpdate(float deltaTime) {};
  virtual void onRender(glm::mat4 view, glm::mat4 projection) {};
  virtual void onImGuiRender() {};
};
}; // namespace test
