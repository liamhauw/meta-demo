#include "luka/function/rendering/rendering.h"

namespace luka {

Rendering::Rendering() {
  vulkan_rhi_ = std::make_unique<VulkanRhi>();
}

}  // namespace luka
