#pragma once

#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "luka/core/math.h"
#include "luka/resource/resource.h"

namespace luka {
class Rendering {
 public:
  Rendering();

  void Tick();
  bool ShouldClose();

 private:
  void MakeInstance();
  void MakeSurface();
  void MakePhysicalDevice();
  void MakeDevice();
  void MakeQueue();
  void MakeSyncObjects();
  void MakeCommandPool();
  void MakeCommandBuffer();
  void MakeSwapchain();
  void MakeDepthImage();
  void MakeRenderPass();
  void MakeFramebuffer();

  void MakeShaderModule();
  void MakeDescriptorSetLayout();
  void MakeDescriptorPool();
  void MakeDescriptorSet();
  void MakePipelineLayout();
  void MakePipelineCache();
  void MakePipeline();

  void MakeVertexBuffer();
  void MakeUniformBuffer();
  void UpdateDescriptorSets();

  vk::SurfaceFormatKHR PickSurfaceFormat();

  vk::raii::DeviceMemory AllocateDeviceMemory(const vk::MemoryRequirements& memory_requirements,
                                              vk::MemoryPropertyFlags memory_properties_flags);

  vk::raii::ShaderModule MakeShaderModule(vk::ShaderStageFlagBits shader_stage,
                                          const std::string& shader_text);

  static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

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
  struct GlfwContext {
    GlfwContext() {
      glfwInit();
      glfwSetErrorCallback([](int error, const char* msg) {
        std::cout << "glfw: "
                  << "(" << error << ") " << msg << std::endl;
      });
    }
    ~GlfwContext() { glfwTerminate(); }
  };

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
  const size_t kFrameInFlight{2};
  const uint32_t kWidth{1280};
  const uint32_t kHeight{720};
  
  bool framebuffer_resized_{false};
  // uint32_t current_frame_{0};

  vk::raii::Instance instance_{nullptr};
  SurfaceData surface_data_;
  vk::raii::PhysicalDevice physical_device_{nullptr};
  std::pair<uint32_t, uint32_t> gp_queue_family_index_;
  vk::raii::Device device_{nullptr};
  vk::raii::Queue graphics_queue_{nullptr};
  vk::raii::Queue present_queue_{nullptr};
  // std::vector<vk::raii::Fence> fence_;
  // std::vector<vk::raii::Semaphore> image_available_semaphores_;
  // std::vector<vk::raii::Semaphore> render_finished_semaphores_;
  vk::raii::CommandPool command_pool_{nullptr};
  vk::raii::CommandBuffer command_buffer_{nullptr};
  SwapchainData swapchain_data_;
  ImageData depth_image_data_;
  vk::raii::RenderPass render_pass_{nullptr};
  std::vector<vk::raii::Framebuffer> framebuffers_;

  vk::raii::ShaderModule vertex_shader_module_{nullptr};
  vk::raii::ShaderModule fragment_shader_module_{nullptr};
  vk::raii::DescriptorSetLayout descriptor_set_layout_{nullptr};
  vk::raii::DescriptorPool descriptor_pool_{nullptr};
  vk::raii::DescriptorSet descriptor_set_{nullptr};
  vk::raii::PipelineLayout pipeline_layout_{nullptr};
  vk::raii::PipelineCache pipeline_cache_{nullptr};
  vk::raii::Pipeline graphics_pipeline_{nullptr};

  BufferData vertex_buffer_data_;
  BufferData uniform_buffer_data_;
};
};  // namespace luka
