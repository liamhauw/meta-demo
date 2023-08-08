#pragma once

#include <stdexcept>

#include "spdlog/spdlog.h"

namespace luka {

#define LOGI(...) spdlog::info(__VA_ARGS__)
#define LOGW(...) spdlog::warn(__VA_ARGS__)
#define LOGE(...) spdlog::error(__VA_ARGS__)
#define LOGD(...) spdlog::debug(__VA_ARGS__)

#define THROW(...)                               \
  spdlog::error("[{}:{}]:", __FILE__, __LINE__); \
  throw std::runtime_error { "" }

}  // namespace luka