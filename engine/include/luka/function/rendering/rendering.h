#pragma once

#include <memory>

#include "luka/function/rendering/vulkan_rhi.h"

namespace luka {

class Rendering {
 public:
  Rendering();

 private:
  std::unique_ptr<VulkanRhi> vulkan_rhi_;
};

}