#include "function/rendering/rendering.h"

namespace luka {

Rendering::Rendering(const std::shared_ptr<Window>& window) {
  vulkan_rhi_ = std::make_unique<VulkanRhi>(window);
}

}  // namespace luka
