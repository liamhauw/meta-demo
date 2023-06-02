#include <exception>
#include <vulkan/vulkan_raii.hpp>

#include "luka/core/log.h"
#include "luka/engine.h"

int main(int, char**) {
  try {
    luka::Engine::Run();
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