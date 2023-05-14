#pragma once

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

#include "luka/core/file.h"
#include "luka/core/math.h"
#include "luka/core/time.h"
#include "luka/resource/resource.h"

namespace luka {
class Rendering {
 public:
  Rendering();
  ~Rendering();

  void Tick();
  [[nodiscard]] bool ShouldClose() const;

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
  void MakeColorImage();
  void MakeRenderPass();
  void MakeFramebuffer();

  void MakeVertexBuffer();
  void MakeIndexBuffer();
  void MakeUniformBuffer();
  void MakeTextureImage();
  void MakeTextureSampler();

  void MakeShaderModule();
  void MakeDescriptorSetLayout();
  void MakeDescriptorPool();
  void MakeDescriptorSet();
  void MakePipelineLayout();
  void MakePipelineCache();
  void MakePipeline();

  void RecreateSwapchin();

  static std::pair<vk::Result, uint32_t> SwapchainNextImageWrapper(
      const vk::raii::SwapchainKHR& swapchain, uint64_t timeout,
      vk::Semaphore semaphore, vk::Fence fence) {
    uint32_t image_index;
    vk::Result result = static_cast<vk::Result>(
        swapchain.getDispatcher()->vkAcquireNextImageKHR(
            static_cast<VkDevice>(swapchain.getDevice()),
            static_cast<VkSwapchainKHR>(*swapchain), timeout,
            static_cast<VkSemaphore>(semaphore), static_cast<VkFence>(fence),
            &image_index));
    return std::make_pair(result, image_index);
  }

  static vk::Result QueuePresentWrapper(
      const vk::raii::Queue& queue, const vk::PresentInfoKHR& present_info) {
    return static_cast<vk::Result>(queue.getDispatcher()->vkQueuePresentKHR(
        static_cast<VkQueue>(*queue),
        reinterpret_cast<const VkPresentInfoKHR*>(&present_info)));
  }

  static VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageTypes,
      VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
      void* pUserData);

  static void FramebufferSizeCallback(GLFWwindow* window, int width,
                                      int height);

  vk::raii::DeviceMemory AllocateDeviceMemory(
      const vk::MemoryRequirements& memory_requirements,
      vk::MemoryPropertyFlags memory_properties_flags);

  template <typename T>
  void CopyToDevice(const vk::raii::DeviceMemory& device_memory, const T* data,
                    vk::DeviceSize buffer_size) {
    uint8_t* device_data{
        static_cast<uint8_t*>(device_memory.mapMemory(0, buffer_size))};
    memcpy(device_data, data, buffer_size);
    device_memory.unmapMemory();
  }

  void CopyBuffer(const vk::raii::Buffer& src_buffer,
                  const vk::raii::Buffer& dst_buffer, vk::DeviceSize size);

  vk::raii::CommandBuffer BeginSingleTimeCommands();

  void EndSingleTimeCommands(const vk::raii::CommandBuffer& command_buffer);

  vk::raii::ShaderModule MakeShaderModule(const std::string& shader_file);

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
    GLFWwindow* glfw_window;
    vk::raii::SurfaceKHR surface{nullptr};
  };

  struct SwapchainData {
    vk::Format format;
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

  struct BufferData {
    vk::raii::Buffer buffer{nullptr};
    vk::raii::DeviceMemory device_memory{nullptr};
  };

 private:
  const uint32_t kFramesInFlight{2};
  const uint32_t kWidth{800};
  const uint32_t kHeight{600};

  bool framebuffer_resized_{false};
  uint32_t current_frame_{0};

  vk::raii::Instance instance_{nullptr};
  SurfaceData surface_data_;
  vk::raii::PhysicalDevice physical_device_{nullptr};
  vk::SampleCountFlagBits sample_count_{vk::SampleCountFlagBits::e1};
  std::pair<uint32_t, uint32_t> gp_queue_family_index_;
  vk::raii::Device device_{nullptr};
  vk::raii::Queue graphics_queue_{nullptr};
  vk::raii::Queue present_queue_{nullptr};
  std::vector<vk::raii::Fence> fence_;
  std::vector<vk::raii::Semaphore> image_available_semaphores_;
  std::vector<vk::raii::Semaphore> render_finished_semaphores_;
  vk::raii::CommandPool command_pool_{nullptr};
  std::vector<vk::raii::CommandBuffer> command_buffers_;
  SwapchainData swapchain_data_;
  ImageData depth_image_data_;
  ImageData color_image_data_;
  vk::raii::RenderPass render_pass_{nullptr};
  std::vector<vk::raii::Framebuffer> framebuffers_;

  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
  BufferData vertex_buffer_data_;
  BufferData index_buffer_data_;
  std::vector<BufferData> uniform_buffer_data_;
  std::vector<void*> uniform_buffer_mapped_;

  uint32_t mip_levels_{};
  ImageData texture_image_data_;

  vk::raii::Sampler sampler_{nullptr};

  vk::raii::DescriptorSetLayout descriptor_set_layout_{nullptr};
  vk::raii::DescriptorPool descriptor_pool_{nullptr};
  std::vector<vk::raii::DescriptorSet> descriptor_sets_;
  vk::raii::ShaderModule vertex_shader_module_{nullptr};
  vk::raii::ShaderModule fragment_shader_module_{nullptr};
  vk::raii::PipelineLayout pipeline_layout_{nullptr};
  vk::raii::PipelineCache pipeline_cache_{nullptr};
  vk::raii::Pipeline pipeline_{nullptr};
};

}  // namespace luka