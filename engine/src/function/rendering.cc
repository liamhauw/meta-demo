#include "luka/function/rendering.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>

namespace luka {

Rendering::Rendering() {
  MakeInstance();

#ifndef NDEBUG
  MakeDebugUtilsMessenger();
#endif

  MakeSurface();

  MakePhysicalDevice();

  MakeDevice();

  MakeCommandPool();

  MakeCommandBuffer();

  MakeQueue();

  MakeSwapchain();
}

void Rendering::Tick() { glfwPollEvents(); }

bool Rendering::ShouldClose() { return glfwWindowShouldClose(glfw_window_); }

void Rendering::MakeInstance() {
  vk::raii::Context context_;

  vk::ApplicationInfo application_info{"luka", 1, "luka", 1,
                                       VK_API_VERSION_1_0};

  std::vector<const char*> layers;
  std::vector<const char*> extensions;

  // glfw extensions
  struct GlfwContext {
    GlfwContext() {
      glfwInit();
      glfwSetErrorCallback([](int error, const char* msg) {
        std::cerr << "glfw: "
                  << "(" << error << ") " << msg << std::endl;
      });
    }
    ~GlfwContext() { glfwTerminate(); }
  };
  static auto glfw_context{GlfwContext{}};

  uint32_t glfw_extension_count{0};
  const char** glfw_extensions{
      glfwGetRequiredInstanceExtensions(&glfw_extension_count)};
  extensions = {glfw_extensions, glfw_extensions + glfw_extension_count};

  // macos extensions
#ifdef __APPLE__
  extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
  extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

#ifndef NDEBUG
  const std::vector<vk::LayerProperties> layer_properties{
      context_.enumerateInstanceLayerProperties()};
  const std::vector<vk::ExtensionProperties> extension_properties{
      context_.enumerateInstanceExtensionProperties()};
#endif

  std::vector<const char*> enabled_layers;
  std::vector<const char*> enabled_extensions;

  enabled_layers.reserve(layers.size());
  for (const char* layer : layers) {
    assert(std::find_if(layer_properties.begin(), layer_properties.end(),
                        [layer](const vk::LayerProperties& lp) {
                          return (strcmp(layer, lp.layerName) == 0);
                        }) != layer_properties.end());
    enabled_layers.push_back(layer);
  }

  enabled_extensions.reserve(extensions.size());
  for (const char* extension : extensions) {
    assert(std::find_if(extension_properties.begin(),
                        extension_properties.end(),
                        [extension](const vk::ExtensionProperties& ep) {
                          return (strcmp(extension, ep.extensionName) == 0);
                        }) != extension_properties.end());
    enabled_extensions.push_back(extension);
  }

#ifndef NDEBUG
  if (std::find(layers.begin(), layers.end(), "VK_LAYER_KHRONOS_validation") ==
          layers.end() &&
      std::find_if(layer_properties.begin(), layer_properties.end(),
                   [](const vk::LayerProperties& lp) {
                     return (strcmp("VK_LAYER_KHRONOS_validation",
                                    lp.layerName) == 0);
                   }) != layer_properties.end()) {
    enabled_layers.push_back("VK_LAYER_KHRONOS_validation");
  }

  if (std::find(extensions.begin(), extensions.end(),
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == extensions.end() &&
      std::find_if(extension_properties.begin(), extension_properties.end(),
                   [](vk::ExtensionProperties const& ep) {
                     return (strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
                                    ep.extensionName) == 0);
                   }) != extension_properties.end()) {
    enabled_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
#endif

  vk::InstanceCreateFlags flags;
#ifdef __APPLE__
  flags |= vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;
#endif

#ifndef NDEBUG
  vk::DebugUtilsMessageSeverityFlagsEXT severity_flags(
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
  vk::DebugUtilsMessageTypeFlagsEXT messageType_flags(
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

  vk::StructureChain<vk::InstanceCreateInfo,
                     vk::DebugUtilsMessengerCreateInfoEXT>
      instance_info_chain{
          {flags, &application_info, enabled_layers, enabled_extensions},
          {{},
           severity_flags,
           messageType_flags,
           &DebugUtilsMessengerCallback}};
#else
  vk::StructureChain<vk::InstanceCreateInfo> instance_info_chain{
      {flags, &application_info, enabled_layers, enabled_extensions}};
#endif

  instance_ = vk::raii::Instance{
      context_, instance_info_chain.get<vk::InstanceCreateInfo>()};
}

void Rendering::MakeDebugUtilsMessenger() {
  vk::DebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info{
      {},
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
          vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
          vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
          vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
      &DebugUtilsMessengerCallback};

  vk::raii::DebugUtilsMessengerEXT debug_utils_messneger{
      instance_, debug_utils_messenger_create_info};
}

void Rendering::MakeSurface() {
  extent_ = vk::Extent2D{1280, 720};

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfw_window_ =
      glfwCreateWindow(extent_.width, extent_.height, "luka", nullptr, nullptr);

  VkSurfaceKHR surface;
  vk::Result res{glfwCreateWindowSurface(static_cast<VkInstance>(*instance_),
                                         glfw_window_, nullptr, &surface)};
  if (res != vk::Result::eSuccess) {
    throw std::runtime_error{"fail to create window surface"};
  }
  surface_ = vk::raii::SurfaceKHR{instance_, surface};
}

void Rendering::MakePhysicalDevice() {
  physical_device_ = vk::raii::PhysicalDevices(instance_).front();

  std::vector<vk::QueueFamilyProperties> queue_family_properties{
      physical_device_.getQueueFamilyProperties()};
  assert(queue_family_properties.size() < std::numeric_limits<uint32_t>::max());

  std::vector<vk::QueueFamilyProperties>::const_iterator
      graphics_queue_family_property = std::find_if(
          queue_family_properties.begin(), queue_family_properties.end(),
          [](vk::QueueFamilyProperties const& qfp) {
            return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
          });

  assert(graphics_queue_family_property != queue_family_properties.end());
  uint32_t graphics_queue_family_index{static_cast<uint32_t>(std::distance(
      queue_family_properties.cbegin(), graphics_queue_family_property))};

  if (physical_device_.getSurfaceSupportKHR(graphics_queue_family_index,
                                            *surface_)) {
    gp_queue_family_index_ = std::make_pair(graphics_queue_family_index,
                                            graphics_queue_family_index);
    return;
  }

  for (size_t i = 0; i < queue_family_properties.size(); ++i) {
    if ((queue_family_properties[i].queueFlags &
         vk::QueueFlagBits::eGraphics) &&
        physical_device_.getSurfaceSupportKHR(static_cast<uint32_t>(i),
                                              *surface_)) {
      gp_queue_family_index_ =
          std::make_pair(static_cast<uint32_t>(i), static_cast<uint32_t>(i));

      return;
    }
  }

  for (size_t i = 0; i < queue_family_properties.size(); i++) {
    if (physical_device_.getSurfaceSupportKHR(static_cast<uint32_t>(i),
                                              *surface_)) {
      gp_queue_family_index_ =
          std::make_pair(graphics_queue_family_index, static_cast<uint32_t>(i));
      return;
    }
  }

  throw std::runtime_error{"fail to find queues for both graphics and present"};
}

void Rendering::MakeDevice() {
  std::vector<const char*> extensions;

  extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

#ifdef __APPLE__
  extensions.push_back("VK_KHR_portability_subset");
#endif

  std::vector<const char*> enabled_extensions;
  enabled_extensions.reserve(extensions.size());
  for (const char* extension : extensions) {
    enabled_extensions.push_back(extension);
  }
  float queue_priority{0.0f};
  vk::DeviceQueueCreateInfo device_queue_create_info{
      {}, gp_queue_family_index_.first, 1, &queue_priority};
  vk::DeviceCreateInfo device_create_info{
      {}, device_queue_create_info, {}, enabled_extensions};

  device_ = vk::raii::Device{physical_device_, device_create_info};
}

void Rendering::MakeCommandPool() {
  command_pool_ =
      vk::raii::CommandPool{device_,
                            {vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                             gp_queue_family_index_.first}};
}

void Rendering::MakeCommandBuffer() {
  vk::CommandBufferAllocateInfo command_buffer_allocate_info{
      *command_pool_, vk::CommandBufferLevel::ePrimary, 1};
  command_buffer_ = std::move(
      vk::raii::CommandBuffers{device_, command_buffer_allocate_info}.front());
}

void Rendering::MakeQueue() {
  graphics_queue_ = vk::raii::Queue{device_, gp_queue_family_index_.first, 0};
  present_queue_ = vk::raii::Queue{device_, gp_queue_family_index_.second, 0};
}

void Rendering::MakeSwapchain() {
  std::vector<vk::SurfaceFormatKHR> surface_format{
      physical_device_.getSurfaceFormatsKHR(*surface_)};
  assert(!surface_format.empty());
  vk::SurfaceFormatKHR picked_format{surface_format[0]};
  if (surface_format.size() == 1) {
    if (surface_format[0].format == vk::Format::eUndefined) {
      picked_format.format = vk::Format::eB8G8R8A8Unorm;
      picked_format.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
    }
  } else {
    vk::Format requested_formats[]{
        vk::Format::eB8G8R8A8Unorm, vk::Format::eR8G8B8A8Unorm,
        vk::Format::eB8G8R8Unorm, vk::Format::eR8G8B8Unorm};
    vk::ColorSpaceKHR requested_color_space{vk::ColorSpaceKHR::eSrgbNonlinear};
    for (size_t i = 0;
         i < sizeof(requested_formats) / sizeof(requested_formats[0]); i++) {
      vk::Format requestedFormat{requested_formats[i]};
      auto it{std::find_if(surface_format.begin(), surface_format.end(),
                           [requestedFormat, requested_color_space](
                               vk::SurfaceFormatKHR const& f) {
                             return (f.format == requestedFormat) &&
                                    (f.colorSpace == requested_color_space);
                           })};
      if (it != surface_format.end()) {
        picked_format = *it;
        break;
      }
    }
  }
  assert(picked_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear);

  color_format_ = picked_format.format;

  vk::SurfaceCapabilitiesKHR surface_capabilities{
      physical_device_.getSurfaceCapabilitiesKHR(*surface_)};
  vk::Extent2D swapchain_extent;
  if (surface_capabilities.currentExtent.width ==
      std::numeric_limits<uint32_t>::max()) {
    swapchain_extent.width =
        Clamp(extent_.width, surface_capabilities.minImageExtent.width,
              surface_capabilities.maxImageExtent.width);
    swapchain_extent.height =
        Clamp(extent_.height, surface_capabilities.minImageExtent.height,
              surface_capabilities.maxImageExtent.height);
  } else {
    swapchain_extent = surface_capabilities.currentExtent;
  }

  vk::SurfaceTransformFlagBitsKHR pre_transform{
      (surface_capabilities.supportedTransforms &
       vk::SurfaceTransformFlagBitsKHR::eIdentity)
          ? vk::SurfaceTransformFlagBitsKHR::eIdentity
          : surface_capabilities.currentTransform};

  vk::CompositeAlphaFlagBitsKHR composite_alpha{
      (surface_capabilities.supportedCompositeAlpha &
       vk::CompositeAlphaFlagBitsKHR::ePreMultiplied)
          ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
      : (surface_capabilities.supportedCompositeAlpha &
         vk::CompositeAlphaFlagBitsKHR::ePostMultiplied)
          ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
      : (surface_capabilities.supportedCompositeAlpha &
         vk::CompositeAlphaFlagBitsKHR::eInherit)
          ? vk::CompositeAlphaFlagBitsKHR::eInherit
          : vk::CompositeAlphaFlagBitsKHR::eOpaque};

  std::vector<vk::PresentModeKHR> present_modes{
      physical_device_.getSurfacePresentModesKHR(*surface_)};

  vk::PresentModeKHR picked_mode = vk::PresentModeKHR::eFifo;
  for (const auto& present_mode : present_modes) {
    if (present_mode == vk::PresentModeKHR::eMailbox) {
      picked_mode = present_mode;
      break;
    }

    if (present_mode == vk::PresentModeKHR::eImmediate) {
      picked_mode = present_mode;
    }
  }

  vk::SwapchainCreateInfoKHR swapchain_create_info{
      {},
      *surface_,
      surface_capabilities.minImageCount,
      color_format_,
      picked_format.colorSpace,
      swapchain_extent,
      1,
      vk::ImageUsageFlagBits::eColorAttachment |
          vk::ImageUsageFlagBits::eTransferSrc,
      vk::SharingMode::eExclusive,
      {},
      pre_transform,
      composite_alpha,
      picked_mode,
      true,
      {}};

  if (gp_queue_family_index_.first != gp_queue_family_index_.second) {
    uint32_t queue_family_indices[2]{gp_queue_family_index_.first,
                                     gp_queue_family_index_.second};
    swapchain_create_info.imageSharingMode = vk::SharingMode::eConcurrent;
    swapchain_create_info.queueFamilyIndexCount = 2;
    swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
  }

  swapchain_ = vk::raii::SwapchainKHR{device_, swapchain_create_info};

  images_ = swapchain_.getImages();

  image_views_.reserve(images_.size());
  vk::ImageViewCreateInfo image_view_create_info{
      {},
      {},
      vk::ImageViewType::e2D,
      color_format_,
      {},
      {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};
  for (auto image : images_) {
    image_view_create_info.image = image;
    image_views_.emplace_back(device_, image_view_create_info);
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL Rendering::DebugUtilsMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData,
    void* /*pUserData*/) {
#ifndef NDEBUG
  if (pCallbackData->messageIdNumber == 648835635) {
    // UNASSIGNED-khronos-Validation-debug-build-warning-message
    return VK_FALSE;
  }
  if (pCallbackData->messageIdNumber == 767975156) {
    // UNASSIGNED-BestPractices-vkCreateInstance-specialuse-extension
    return VK_FALSE;
  }
#endif

  std::cout << vk::to_string(
                   static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(
                       message_severity))
            << ": "
            << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(
                   message_types))
            << ":\n";
  std::cout << std::string("\t") << "message id name   = <"
            << pCallbackData->pMessageIdName << ">\n";
  std::cout << std::string("\t")
            << "message id number = " << pCallbackData->messageIdNumber << "\n";
  std::cout << std::string("\t") << "message         = <"
            << pCallbackData->pMessage << ">\n";
  if (0 < pCallbackData->queueLabelCount) {
    std::cout << std::string("\t") << "queue labels:\n";
    for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++) {
      std::cout << std::string("\t\t") << "label_name = <"
                << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
    }
  }
  if (0 < pCallbackData->cmdBufLabelCount) {
    std::cout << std::string("\t") << "command buffer labels:\n";
    for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
      std::cout << std::string("\t\t") << "label name = <"
                << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
    }
  }
  if (0 < pCallbackData->objectCount) {
    std::cout << std::string("\t") << "objects:\n";
    for (uint32_t i = 0; i < pCallbackData->objectCount; i++) {
      std::cout << std::string("\t\t") << "object " << i << "\n";
      std::cout << std::string("\t\t\t") << "object type   = "
                << vk::to_string(static_cast<vk::ObjectType>(
                       pCallbackData->pObjects[i].objectType))
                << "\n";
      std::cout << std::string("\t\t\t")
                << "object handle = " << pCallbackData->pObjects[i].objectHandle
                << "\n";
      if (pCallbackData->pObjects[i].pObjectName) {
        std::cout << std::string("\t\t\t") << "object name   = <"
                  << pCallbackData->pObjects[i].pObjectName << ">\n";
      }
    }
  }
  return VK_TRUE;
}

}  // namespace luka
