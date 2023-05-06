#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "luka/core/math.h"

namespace luka {
class Rendering {
 public:
  Rendering();

  void Tick();
  bool ShouldClose();

 private:
  void MakeInstance();
  void MakeDebugUtilsMessenger();
  void MakeSurface();
  void MakePhysicalDevice();
  void MakeDevice();
  void MakeCommandPool();
  void MakeCommandBuffer();
  void MakeQueue();
  void MakeSwapchain();

  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageTypes,
      VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
      void* pUserData);

 private:
  vk::raii::Instance instance_{nullptr};

  vk::raii::SurfaceKHR surface_{nullptr};
  vk::Extent2D extent_;
  GLFWwindow* glfw_window_;

  vk::raii::PhysicalDevice physical_device_{nullptr};
  std::pair<uint32_t, uint32_t> gp_queue_family_index_;

  vk::raii::Device device_{nullptr};

  vk::raii::CommandPool command_pool_{nullptr};

  vk::raii::CommandBuffer command_buffer_{nullptr};

  vk::raii::Queue graphics_queue_{nullptr};
  vk::raii::Queue present_queue_{nullptr};

  vk::raii::SwapchainKHR swapchain_{nullptr};
  vk::Format color_format_;
  std::vector<vk::Image> images_;
  std::vector<vk::raii::ImageView> image_views_;
};
};  // namespace luka
