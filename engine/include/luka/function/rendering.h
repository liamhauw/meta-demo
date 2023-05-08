#pragma once

#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>

#include <numeric>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
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
  void MakeDepthImage();
  void MakeUniformBuffer();
  void MakeDescriptorSetLayout();
  void MakePipelineLayout();
  void MakeRenderPass();
  void MakeShaderModule();
  void MakeFramebuffer();
  void MakeVertexBuffer();
  void MakeDescriptorPool();
  void MakeDescriptorSet();
  void UpdateDescriptorSets();
  void MakeGraphicsPipeline();

  vk::SurfaceFormatKHR PickSurfaceFormat();

  vk::raii::DeviceMemory AllocateDeviceMemory(const vk::MemoryRequirements& memory_requirements,
                                              vk::MemoryPropertyFlags memory_properties_flags);

  vk::raii::ShaderModule MakeShaderModule(vk::ShaderStageFlagBits shader_stage,
                                          const std::string& shader_text);

  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageTypes,
      VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, void* pUserData);

  template <typename T>
  void CopyToDevice(const vk::raii::DeviceMemory& device_memory, const T* data, size_t count = 1,
                    vk::DeviceSize stride = sizeof(T)) {
    assert(sizeof(T) <= stride);
    uint8_t* device_data{static_cast<uint8_t*>(device_memory.mapMemory(0, count * stride))};
    if (stride == sizeof(T)) {
      memcpy(device_data, data, count * sizeof(T));
    } else {
      for (size_t i = 0; i < count; i++) {
        memcpy(device_data, &data[i], sizeof(T));
        device_data += stride;
      }
    }
    device_memory.unmapMemory();
  }

 private:
  struct SurfaceData {
    vk::raii::SurfaceKHR surface{nullptr};
    vk::Extent2D extent;
    GLFWwindow* glfw_window;
  };

  struct SwapchainData {
    vk::Format format;
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

  struct BufferData {
    vk::raii::Buffer buffer{nullptr};
    vk::raii::DeviceMemory device_memory{nullptr};
  };

 private:
  vk::raii::Instance instance_{nullptr};

  SurfaceData surface_data_;

  vk::raii::PhysicalDevice physical_device_{nullptr};
  std::pair<uint32_t, uint32_t> gp_queue_family_index_;

  vk::raii::Device device_{nullptr};

  vk::raii::CommandPool command_pool_{nullptr};

  vk::raii::CommandBuffer command_buffer_{nullptr};

  vk::raii::Queue graphics_queue_{nullptr};
  vk::raii::Queue present_queue_{nullptr};

  SwapchainData swapchain_data_;

  ImageData depth_image_data_;

  BufferData uniform_buffer_data_;

  vk::raii::DescriptorSetLayout descriptor_set_layout_{nullptr};

  vk::raii::PipelineLayout pipeline_layout_{nullptr};

  vk::raii::RenderPass render_pass_{nullptr};

  vk::raii::ShaderModule vertex_shader_module_{nullptr};
  vk::raii::ShaderModule fragment_shader_module_{nullptr};

  std::vector<vk::raii::Framebuffer> framebuffers_;

  BufferData vertex_buffer_data_;

  vk::raii::DescriptorPool descriptor_pool_{nullptr};

  vk::raii::DescriptorSet descriptor_set_{nullptr};

  vk::raii::Pipeline graphics_pipeline_{nullptr};
};
};  // namespace luka
