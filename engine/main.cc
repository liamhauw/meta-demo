/*
  SPDX license identifier: MIT

  Copyright (C) 2023-present Liam Hauw.

  Engine entry.
*/

#include <vulkan/vulkan_raii.hpp>

#include "core/util/log.h"
#include "engine.h"

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