#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

#include "luka/function/window.h"
#include "luka/resource/gltf_model.h"
#include "luka/resource/obj_model.h"

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
  void MakeCommandObject();
  void MakeSyncObject();

  void MakeSwapchain();
  void MakeColorImage();
  void MakeDepthImage();
  void MakeRenderPass();
  void MakeFramebuffer();

  void MakeModel();
  GltfModel gltf_model_;

  void MakeVertexBuffer();
  void MakeIndexBuffer();
  void MakeUniformBuffer();
  void MakeTextureImage();
  void MakeTextureSampler();

  void MakeDescriptorSetLayout();
  void MakeDescriptorPool();
  void MakeDescriptorSet();
  void MakePipeline();

  static std::pair<vk::Result, uint32_t> AcquireNextImageWrapper(
      const vk::raii::SwapchainKHR& swapchain, uint64_t timeout,
      vk::Semaphore semaphore, vk::Fence fence) {
    uint32_t image_index;
    vk::Result result{static_cast<vk::Result>(
        swapchain.getDispatcher()->vkAcquireNextImageKHR(
            static_cast<VkDevice>(swapchain.getDevice()),
            static_cast<VkSwapchainKHR>(*swapchain), timeout,
            static_cast<VkSemaphore>(semaphore), static_cast<VkFence>(fence),
            &image_index))};
    return std::make_pair(result, image_index);
  }

  static vk::Result QueuePresentWrapper(
      const vk::raii::Queue& queue, const vk::PresentInfoKHR& present_info) {
    return static_cast<vk::Result>(queue.getDispatcher()->vkQueuePresentKHR(
        static_cast<VkQueue>(*queue),
        reinterpret_cast<const VkPresentInfoKHR*>(&present_info)));
  }

  void RecreateSwapchin();

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

  vk::raii::CommandBuffer BeginSingleTimeCommand();

  void EndSingleTimeCommand(const vk::raii::CommandBuffer& command_buffer);

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

  struct QueueFamliy {
    std::optional<uint32_t> graphics_index;
    std::optional<uint32_t> present_index;

    [[nodiscard]] bool IsComplete() const {
      return graphics_index.has_value() && present_index.has_value();
    }
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
  uint32_t current_frame_{0};

  vk::raii::Context context_;
  vk::raii::Instance instance_{nullptr};
#ifndef NDEBUG
  vk::raii::DebugUtilsMessengerEXT debug_utils_messenger_{nullptr};
#endif

  SurfaceData surface_data_;
  const uint32_t kWidth{1280};
  const uint32_t kHeight{720};
  bool framebuffer_resized_{false};

  vk::raii::PhysicalDevice physical_device_{nullptr};
  QueueFamliy queue_family_;
  const std::vector<const char*> kDeviceExtensions{
      VK_KHR_SWAPCHAIN_EXTENSION_NAME};
  vk::SampleCountFlagBits sample_count_{vk::SampleCountFlagBits::e1};
  float max_anisotropy_{0.0f};

  vk::raii::Device device_{nullptr};

  vk::raii::Queue graphics_queue_{nullptr};
  vk::raii::Queue present_queue_{nullptr};
  vk::raii::CommandPool command_pool_{nullptr};
  std::vector<vk::raii::CommandBuffer> command_buffers_;

  std::vector<vk::raii::Fence> fences_;
  std::vector<vk::raii::Semaphore> image_available_semaphores_;
  std::vector<vk::raii::Semaphore> render_finished_semaphores_;

  SwapchainData swapchain_data_;

  ImageData color_image_data_;

  ImageData depth_image_data_;

  vk::raii::RenderPass render_pass_{nullptr};

  std::vector<vk::raii::Framebuffer> framebuffers_;

  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
  BufferData vertex_buffer_data_;
  BufferData index_buffer_data_;
  std::vector<BufferData> uniform_buffer_datas_;
  std::vector<void*> uniform_buffer_mapped_dates_;

  uint32_t mip_level_count_{};
  ImageData texture_image_data_;

  vk::raii::Sampler sampler_{nullptr};

  vk::raii::DescriptorSetLayout descriptor_set_layout_{nullptr};
  vk::raii::DescriptorPool descriptor_pool_{nullptr};
  std::vector<vk::raii::DescriptorSet> descriptor_sets_;
  vk::raii::PipelineLayout pipeline_layout_{nullptr};
  vk::raii::PipelineCache pipeline_cache_{nullptr};
  vk::raii::Pipeline pipeline_{nullptr};
};

}  // namespace luka