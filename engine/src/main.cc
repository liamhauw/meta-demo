#include <vulkan/vulkan.hpp>

#include "luka/core/log.h"
#include "luka/engine.h"

int main(int, char**) {
  try {
    luka::Engine engine;
    engine.Run();
  } catch (const vk::SystemError& e) {
    LOGE("vk::SystemError: {}", e.what());
    return -1;
  } catch (const std::exception& e) {
    LOGE("std::exception: {}", e.what());
    return -1;
  } catch (...) {
    LOGE("unknown exception.");
    return -1;
  }
  return 0;
}