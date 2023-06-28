#pragma once

#include <memory>
#include <vulkan/vulkan_raii.hpp>

#include "luka/function/rendering/vulkan_util.h"
#include "luka/function/window/window.h"

namespace luka {

class VulkanRhi {
 public:
  explicit VulkanRhi(const std::shared_ptr<Window>& window);
  ~VulkanRhi();

 private:
  void MakeInstance();
  void MakeSurface();
  void MakePhysicalDevice();
  void MakeDevice();
  void MakeCommandObject();
  void MakeSyncObject();
  void MakeSwapchain();
  void MakeDepthImage();

  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
      VkDebugUtilsMessageTypeFlagsEXT message_types,
      const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data, void* p_user_data);

 private:
  std::shared_ptr<Window> window_;

  const uint32_t frames_in_flight_{2};
  uint32_t current_frame_{0};

  vk::raii::Context context_;
  vk::raii::Instance instance_{nullptr};
#ifndef NDEBUG
  vk::raii::DebugUtilsMessengerEXT debug_utils_messenger_{nullptr};
#endif

  vk::raii::SurfaceKHR surface_{nullptr};

  vk::raii::PhysicalDevice physical_device_{nullptr};
  QueueFamliy queue_family_;
  std::vector<const char*> device_extensions_{VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  vk::SampleCountFlagBits sample_count_{vk::SampleCountFlagBits::e1};
  float max_anisotropy_{0.0f};

  vk::raii::Device device_{nullptr};
  vk::raii::Queue graphics_queue_{nullptr};
  vk::raii::Queue compute_queue_{nullptr};
  vk::raii::Queue present_queue_{nullptr};

  vk::raii::CommandPool command_pool_{nullptr};
  std::vector<vk::raii::CommandBuffer> command_buffers_;

  std::vector<vk::raii::Fence> fences_;
  std::vector<vk::raii::Semaphore> image_available_semaphores_;
  std::vector<vk::raii::Semaphore> render_finished_semaphores_;

  SwapchainData swapchain_data_;

  ImageData depth_image_data_;
};

}  // namespace luka