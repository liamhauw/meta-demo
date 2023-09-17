#pragma once

#include <optional>
#include <vulkan/vulkan_raii.hpp>

namespace luka {

struct QueueFamliy {
  std::optional<uint32_t> graphics_index;
  std::optional<uint32_t> compute_index;
  std::optional<uint32_t> present_index;

  bool IsComplete() const {
    return graphics_index.has_value() && compute_index.has_value() && present_index.has_value();
  }
};

struct SwapchainData {
  uint32_t count;
  vk::Format format;
  vk::ColorSpaceKHR color_space;
  vk::Extent2D extent;
  vk::raii::SwapchainKHR swapchain{nullptr};
  std::vector<vk::Image> images;
  std::vector<vk::raii::ImageView> image_views;
};

struct ImageData {
  vk::Format format;
  vk::raii::Image image{nullptr};
  vk::raii::DeviceMemory device_memory{nullptr};
  vk::raii::ImageView image_view{nullptr};
};

class VulkanUtil {
 public:
  static vk::raii::DeviceMemory AllocateDeviceMemory(
      const vk::raii::PhysicalDevice& physical_device, const vk::raii::Device& device,
      const vk::MemoryRequirements& memory_requirements,
      vk::MemoryPropertyFlags memory_properties_flags);
};

}  // namespace luka