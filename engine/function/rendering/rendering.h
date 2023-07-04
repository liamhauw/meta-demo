#pragma once

#include <memory>

#include "function/rendering/vulkan_rhi.h"
#include "function/window/window.h"

namespace luka {

class Rendering {
 public:
  explicit Rendering(const std::shared_ptr<Window>& window);

 private:
  std::unique_ptr<VulkanRhi> vulkan_rhi_;
};

}  // namespace luka