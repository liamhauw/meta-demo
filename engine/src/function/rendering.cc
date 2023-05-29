#include "luka/function/rendering.h"

namespace luka {

Rendering::Rendering() {
  MakeInstance();
  MakeSurface();
  MakePhysicalDevice();
  MakeDevice();
  MakeCommandObject();
  MakeSyncObject();

  MakeSwapchain();
  MakeColorImage();
  MakeDepthImage();
  MakeRenderPass();
  MakeFramebuffer();
  MakeVertexBuffer();
  MakeIndexBuffer();
  MakeUniformBuffer();
  MakeTextureImage();
  MakeTextureSampler();

  MakeDescriptorSetLayout();
  MakeDescriptorPool();
  MakeDescriptorSet();
  MakePipeline();
}

Rendering::~Rendering() { device_.waitIdle(); }

void Rendering::Tick() {
  glfwPollEvents();

  vk::Result res{
      device_.waitForFences(*(fences_[current_frame_]), VK_TRUE, UINT64_MAX)};
  if (res != vk::Result::eSuccess) {
    throw std::runtime_error{"fail to wait for fences"};
  }

  vk::Result result;
  uint32_t image_index;
  std::tie(result, image_index) = AcquireNextImageWrapper(
      swapchain_data_.swapchain, UINT64_MAX,
      *image_available_semaphores_[current_frame_], nullptr);

  if (result == vk::Result::eErrorOutOfDateKHR) {
    RecreateSwapchin();
    return;
  } else if (result != vk::Result::eSuccess &&
             result != vk::Result::eSuboptimalKHR) {
    throw std::runtime_error{"fail to acquire next image"};
  }

  // update uniform buffer
  static auto start_time{std::chrono::high_resolution_clock::now()};

  auto current_time{std::chrono::high_resolution_clock::now()};
  float time{std::chrono::duration<float, std::chrono::seconds::period>(
                 current_time - start_time)
                 .count()};

  UniformBufferObject ubo{};
  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(10.0f),
                          glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.view =
      glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj =
      glm::perspective(glm::radians(45.0f),
                       static_cast<float>(swapchain_data_.extent.width) /
                           static_cast<float>(swapchain_data_.extent.height),
                       0.1f, 10.0f);
  ubo.proj[1][1] *= -1;
  memcpy(uniform_buffer_mapped_dates_[current_frame_], &ubo, sizeof(ubo));

  // record command
  device_.resetFences(*(fences_[current_frame_]));

  auto& cur_command_buffer{command_buffers_[current_frame_]};

  cur_command_buffer.reset({});
  cur_command_buffer.begin({});

  std::array<vk::ClearValue, 2> clear_values;
  clear_values[0].color = vk::ClearColorValue{0.0f, 0.0f, 0.0f, 1.0f};
  clear_values[1].depthStencil = vk::ClearDepthStencilValue{1.0f, 0};

  vk::RenderPassBeginInfo render_pass_begin_info{
      *render_pass_, *framebuffers_[image_index],
      vk::Rect2D{vk::Offset2D{0, 0}, swapchain_data_.extent}, clear_values};

  cur_command_buffer.beginRenderPass(render_pass_begin_info,
                                     vk::SubpassContents::eInline);

  cur_command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline_);

  cur_command_buffer.setViewport(
      0, vk::Viewport{
             0.0f, 0.0f, static_cast<float>(swapchain_data_.extent.width),
             static_cast<float>(swapchain_data_.extent.height), 0.0f, 1.0f});
  cur_command_buffer.setScissor(
      0, vk::Rect2D{vk::Offset2D{0, 0}, swapchain_data_.extent});

  cur_command_buffer.bindVertexBuffers(0, *(vertex_buffer_data_.buffer), {0});
  cur_command_buffer.bindIndexBuffer(*(index_buffer_data_.buffer), 0,
                                     vk::IndexType::eUint32);
  cur_command_buffer.bindDescriptorSets(
      vk::PipelineBindPoint::eGraphics, *pipeline_layout_, 0,
      *(descriptor_sets_[current_frame_]), nullptr);

  cur_command_buffer.drawIndexed(static_cast<uint32_t>(indices_.size()), 1, 0,
                                 0, 0);

  cur_command_buffer.endRenderPass();

  cur_command_buffer.end();

  vk::PipelineStageFlags wait_stage{
      vk::PipelineStageFlagBits::eColorAttachmentOutput};

  vk::SubmitInfo submit_info{*(image_available_semaphores_[current_frame_]),
                             wait_stage, *cur_command_buffer,
                             *(render_finished_semaphores_[current_frame_])};

  graphics_queue_.submit(submit_info, *(fences_[current_frame_]));

  vk::PresentInfoKHR presten_info_khr{
      *(render_finished_semaphores_[current_frame_]),
      *(swapchain_data_.swapchain), image_index};

  result = QueuePresentWrapper(present_queue_, presten_info_khr);

  if (result == vk::Result::eErrorOutOfDateKHR ||
      result == vk::Result::eSuboptimalKHR || framebuffer_resized_) {
    framebuffer_resized_ = false;
    RecreateSwapchin();
  } else if (result != vk::Result::eSuccess) {
    throw std::runtime_error{"fail to present swap chain image"};
  }
  current_frame_ = (current_frame_ + 1) % kFramesInFlight;
}

bool Rendering::ShouldClose() const {
  return glfwWindowShouldClose(surface_data_.glfw_window);
}

void Rendering::MakeInstance() {
  vk::ApplicationInfo application_info{"luka", VK_MAKE_VERSION(1, 0, 0), "luka",
                                       VK_MAKE_VERSION(1, 0, 0),
                                       VK_API_VERSION_1_0};

  std::vector<const char*> required_layers;
  std::vector<const char*> required_extensions;

  static auto glfw_context{GlfwContext{}};
  uint32_t glfw_extension_count{0};
  const char** glfw_extensions{
      glfwGetRequiredInstanceExtensions(&glfw_extension_count)};
  required_extensions = {glfw_extensions,
                         glfw_extensions + glfw_extension_count};

#ifdef __APPLE__
  required_extensions.push_back(
      VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
  required_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

  const std::vector<vk::LayerProperties> layer_properties{
      context_.enumerateInstanceLayerProperties()};
  const std::vector<vk::ExtensionProperties> extension_properties{
      context_.enumerateInstanceExtensionProperties()};

  std::vector<const char*> enabled_layers;
  std::vector<const char*> enabled_extensions;

  enabled_layers.reserve(required_layers.size());
  for (const char* layer : required_layers) {
    if (std::find_if(layer_properties.begin(), layer_properties.end(),
                     [layer](const vk::LayerProperties& lp) {
                       return (strcmp(layer, lp.layerName) == 0);
                     }) == layer_properties.end()) {
      throw std::runtime_error{std::string{"fail to find "} + layer};
    }
    enabled_layers.push_back(layer);
  }

  enabled_extensions.reserve(required_extensions.size());
  for (const char* extension : required_extensions) {
    if (std::find_if(extension_properties.begin(), extension_properties.end(),
                     [extension](const vk::ExtensionProperties& ep) {
                       return (strcmp(extension, ep.extensionName) == 0);
                     }) == extension_properties.end()) {
      throw std::runtime_error{std::string{"fail to find "} + extension};
    }
    enabled_extensions.push_back(extension);
  }

#ifndef NDEBUG
  if (std::find(required_layers.begin(), required_layers.end(),
                "VK_LAYER_KHRONOS_validation") == required_layers.end() &&
      std::find_if(layer_properties.begin(), layer_properties.end(),
                   [](const vk::LayerProperties& lp) {
                     return (strcmp("VK_LAYER_KHRONOS_validation",
                                    lp.layerName) == 0);
                   }) != layer_properties.end()) {
    enabled_layers.push_back("VK_LAYER_KHRONOS_validation");
  }

  if (std::find(required_extensions.begin(), required_extensions.end(),
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME) ==
          required_extensions.end() &&
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
  vk::DebugUtilsMessageSeverityFlagsEXT message_severity_flags{
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError};

  vk::DebugUtilsMessageTypeFlagsEXT message_type_flags{
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
      vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
      vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation};

  vk::StructureChain<vk::InstanceCreateInfo,
                     vk::DebugUtilsMessengerCreateInfoEXT>
      info_chain{{flags, &application_info, enabled_layers, enabled_extensions},
                 {{},
                  message_severity_flags,
                  message_type_flags,
                  &DebugUtilsMessengerCallback}};
#else
  vk::StructureChain<vk::InstanceCreateInfo> info_chain{
      {flags, &application_info, enabled_layers, enabled_extensions}};
#endif

  instance_ =
      vk::raii::Instance{context_, info_chain.get<vk::InstanceCreateInfo>()};

#ifndef NDEBUG
  vk::DebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info{
      {},
      message_severity_flags,
      message_type_flags,
      &DebugUtilsMessengerCallback};

  debug_utils_messenger_ = vk::raii::DebugUtilsMessengerEXT{
      instance_, debug_utils_messenger_create_info};
#endif
}

void Rendering::MakeSurface() {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  surface_data_.glfw_window =
      glfwCreateWindow(static_cast<int>(kWidth), static_cast<int>(kHeight),
                       "luka", nullptr, nullptr);
  glfwSetWindowUserPointer(surface_data_.glfw_window, this);
  glfwSetFramebufferSizeCallback(surface_data_.glfw_window,
                                 FramebufferSizeCallback);

  VkSurfaceKHR surface;
  glfwCreateWindowSurface(static_cast<VkInstance>(*instance_),
                          surface_data_.glfw_window, nullptr, &surface);

  surface_data_.surface = vk::raii::SurfaceKHR{instance_, surface};
}

void Rendering::MakePhysicalDevice() {
  vk::raii::PhysicalDevices physical_devices{instance_};

  for (auto& physical_device : physical_devices) {
    // queue famliy
    QueueFamliy queue_family;

    std::vector<vk::QueueFamilyProperties> queue_family_properties{
        physical_device.getQueueFamilyProperties()};
    uint32_t i{0};
    for (const auto& queue_famliy_propertie : queue_family_properties) {
      if ((queue_famliy_propertie.queueFlags & vk::QueueFlagBits::eGraphics) &&
          physical_device.getSurfaceSupportKHR(i, *surface_data_.surface)) {
        queue_family.graphics_index = i;
        queue_family.present_index = i;
        break;
      }
      ++i;
    }
    if (!queue_family.IsComplete()) {
      i = 0;
      for (const auto& queue_famliy_propertie : queue_family_properties) {
        if (queue_famliy_propertie.queueFlags & vk::QueueFlagBits::eGraphics) {
          queue_family.graphics_index = i;
        }
        if (physical_device.getSurfaceSupportKHR(i, *surface_data_.surface)) {
          queue_family.present_index = i;
        }
        if (queue_family.IsComplete()) {
          break;
        }
        ++i;
      }
    }

    // device extension
    bool has_device_extension{true};
    std::vector<vk::ExtensionProperties> extension_properties{
        physical_device.enumerateDeviceExtensionProperties()};
    for (const char* device_extension : kDeviceExtensions) {
      if (std::find_if(extension_properties.begin(), extension_properties.end(),
                       [device_extension](const vk::ExtensionProperties& ep) {
                         return (strcmp(device_extension, ep.extensionName) ==
                                 0);
                       }) == extension_properties.end()) {
        has_device_extension = false;
        break;
      }
    }

    // device feature
    vk::PhysicalDeviceFeatures physical_device_features{
        physical_device.getFeatures()};
    bool has_device_feature{
        static_cast<bool>(physical_device_features.samplerAnisotropy &
                          physical_device_features.fillModeNonSolid)};

    if (queue_family.IsComplete() && has_device_extension &&
        has_device_feature) {
      queue_family_ = queue_family;
      physical_device_ = std::move(physical_device);
      break;
    }
  }

  if (!(*physical_device_)) {
    throw std::runtime_error{"fail to find physical device"};
  }

  vk::PhysicalDeviceProperties physical_device_properties{
      physical_device_.getProperties()};

  vk::SampleCountFlags sample_count{
      physical_device_properties.limits.framebufferColorSampleCounts &
      physical_device_properties.limits.framebufferDepthSampleCounts};
  if (sample_count & vk::SampleCountFlagBits::e64) {
    sample_count_ = vk::SampleCountFlagBits::e64;
  } else if (sample_count & vk::SampleCountFlagBits::e32) {
    sample_count_ = vk::SampleCountFlagBits::e32;
  } else if (sample_count & vk::SampleCountFlagBits::e16) {
    sample_count_ = vk::SampleCountFlagBits::e16;
  } else if (sample_count & vk::SampleCountFlagBits::e8) {
    sample_count_ = vk::SampleCountFlagBits::e8;
  } else if (sample_count & vk::SampleCountFlagBits::e4) {
    sample_count_ = vk::SampleCountFlagBits::e4;
  } else if (sample_count & vk::SampleCountFlagBits::e2) {
    sample_count_ = vk::SampleCountFlagBits::e2;
  } else {
    sample_count_ = vk::SampleCountFlagBits::e1;
  }

  max_anisotropy_ = physical_device_properties.limits.maxSamplerAnisotropy;
}

void Rendering::MakeDevice() {
  std::vector<const char*> enabled_extensions{kDeviceExtensions};
#ifdef __APPLE__
  enabled_extensions.push_back("VK_KHR_portability_subset");
#endif

  float queue_priority{0.0f};
  vk::DeviceQueueCreateInfo device_queue_create_info{
      {}, queue_family_.graphics_index.value(), 1, &queue_priority};

  vk::PhysicalDeviceFeatures device_features{};
  device_features.samplerAnisotropy = VK_TRUE;

  vk::DeviceCreateInfo device_create_info{
      {}, device_queue_create_info, {}, enabled_extensions, &device_features};

  device_ = vk::raii::Device{physical_device_, device_create_info};
}

void Rendering::MakeCommandObject() {
  graphics_queue_ =
      vk::raii::Queue{device_, queue_family_.graphics_index.value(), 0};
  present_queue_ =
      vk::raii::Queue{device_, queue_family_.present_index.value(), 0};
  command_pool_ =
      vk::raii::CommandPool{device_,
                            {vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
                             queue_family_.graphics_index.value()}};
  vk::CommandBufferAllocateInfo command_buffer_allocate_info{
      *command_pool_, vk::CommandBufferLevel::ePrimary, kFramesInFlight};
  command_buffers_ = std::move(
      vk::raii::CommandBuffers{device_, command_buffer_allocate_info});
}

void Rendering::MakeSyncObject() {
  fences_.reserve(kFramesInFlight);
  image_available_semaphores_.reserve(kFramesInFlight);
  render_finished_semaphores_.reserve(kFramesInFlight);

  vk::FenceCreateInfo fence_create_info{vk::FenceCreateFlagBits::eSignaled};
  vk::SemaphoreCreateInfo semaphore_create_info{};

  for (uint32_t i = 0; i < kFramesInFlight; ++i) {
    fences_.emplace_back(device_, fence_create_info);
    image_available_semaphores_.emplace_back(device_, semaphore_create_info);
    render_finished_semaphores_.emplace_back(device_, semaphore_create_info);
  }
}

void Rendering::MakeSwapchain() {
  // extent
  vk::SurfaceCapabilitiesKHR surface_capabilities{
      physical_device_.getSurfaceCapabilitiesKHR(*surface_data_.surface)};

  if (surface_capabilities.currentExtent.width ==
      std::numeric_limits<uint32_t>::max()) {
    int width, height;
    glfwGetFramebufferSize(surface_data_.glfw_window, &width, &height);

    swapchain_data_.extent.width = glm::clamp(
        static_cast<uint32_t>(width), surface_capabilities.minImageExtent.width,
        surface_capabilities.maxImageExtent.width);
    swapchain_data_.extent.height =
        glm::clamp(static_cast<uint32_t>(height),
                   surface_capabilities.minImageExtent.height,
                   surface_capabilities.maxImageExtent.height);
  } else {
    swapchain_data_.extent = surface_capabilities.currentExtent;
  }

  // image count
  uint32_t image_count{surface_capabilities.minImageCount + 1};
  if (surface_capabilities.maxImageCount > 0 &&
      image_count > surface_capabilities.maxImageCount) {
    image_count = surface_capabilities.maxImageCount;
  }

  // formats
  std::vector<vk::SurfaceFormatKHR> surface_formats{
      physical_device_.getSurfaceFormatsKHR(*(surface_data_.surface))};

  vk::SurfaceFormatKHR picked_format{surface_formats[0]};
  if (surface_formats.size() == 1) {
    if (surface_formats[0].format == vk::Format::eUndefined) {
      picked_format.format = vk::Format::eB8G8R8A8Srgb;
      picked_format.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
    }
  } else {
    std::vector<vk::Format> requested_formats{
        vk::Format::eB8G8R8A8Srgb, vk::Format::eR8G8B8A8Srgb,
        vk::Format::eB8G8R8Srgb, vk::Format::eR8G8B8Srgb};
    vk::ColorSpaceKHR requested_color_space{vk::ColorSpaceKHR::eSrgbNonlinear};
    for (const auto& requested_format : requested_formats) {
      auto it{std::find_if(surface_formats.begin(), surface_formats.end(),
                           [requested_format, requested_color_space](
                               const vk::SurfaceFormatKHR& f) {
                             return (f.format == requested_format) &&
                                    (f.colorSpace == requested_color_space);
                           })};
      if (it != surface_formats.end()) {
        picked_format = *it;
        break;
      }
    }
  }

  swapchain_data_.format = picked_format.format;

  // present modes
  std::vector<vk::PresentModeKHR> present_modes{
      physical_device_.getSurfacePresentModesKHR(*surface_data_.surface)};

  vk::PresentModeKHR picked_mode{vk::PresentModeKHR::eFifo};
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
      *surface_data_.surface,
      image_count,
      swapchain_data_.format,
      picked_format.colorSpace,
      swapchain_data_.extent,
      1,
      vk::ImageUsageFlagBits::eColorAttachment,
      vk::SharingMode::eExclusive,
      {},
      surface_capabilities.currentTransform,
      vk::CompositeAlphaFlagBitsKHR::eOpaque,
      picked_mode,
      VK_TRUE,
      {}};

  if (queue_family_.graphics_index.value() !=
      queue_family_.present_index.value()) {
    uint32_t queue_family_indices[2]{queue_family_.graphics_index.value(),
                                     queue_family_.present_index.value()};
    swapchain_create_info.imageSharingMode = vk::SharingMode::eConcurrent;
    swapchain_create_info.queueFamilyIndexCount = 2;
    swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
  }

  swapchain_data_.swapchain =
      vk::raii::SwapchainKHR{device_, swapchain_create_info};

  swapchain_data_.images = swapchain_data_.swapchain.getImages();

  swapchain_data_.image_views.reserve(swapchain_data_.images.size());
  vk::ImageViewCreateInfo image_view_create_info{
      {},
      {},
      vk::ImageViewType::e2D,
      swapchain_data_.format,
      {},
      {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};
  for (auto image : swapchain_data_.images) {
    image_view_create_info.image = image;
    swapchain_data_.image_views.emplace_back(device_, image_view_create_info);
  }
}

void Rendering::MakeColorImage() {
  color_image_data_.format = swapchain_data_.format;

  vk::ImageCreateInfo image_create_info{
      {},
      vk::ImageType::e2D,
      swapchain_data_.format,
      vk::Extent3D{swapchain_data_.extent, 1},
      1,
      1,
      sample_count_,
      vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eTransientAttachment |
          vk::ImageUsageFlagBits::eColorAttachment,
      vk::SharingMode::eExclusive,
      {},
      vk::ImageLayout::eUndefined};

  color_image_data_.image = vk::raii::Image{device_, image_create_info};

  color_image_data_.device_memory =
      AllocateDeviceMemory(color_image_data_.image.getMemoryRequirements(),
                           vk::MemoryPropertyFlagBits::eDeviceLocal);

  color_image_data_.image.bindMemory(*(color_image_data_.device_memory), 0);

  color_image_data_.image_view = vk::raii::ImageView{
      device_,
      vk::ImageViewCreateInfo{{},
                              *(color_image_data_.image),
                              vk::ImageViewType::e2D,
                              color_image_data_.format,
                              {},
                              {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}}};
}

void Rendering::MakeDepthImage() {
  depth_image_data_.format = vk::Format::eD32Sfloat;

  vk::ImageCreateInfo image_create_info{
      {},
      vk::ImageType::e2D,
      depth_image_data_.format,
      vk::Extent3D{swapchain_data_.extent, 1},
      1,
      1,
      sample_count_,
      vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eDepthStencilAttachment,
      vk::SharingMode::eExclusive,
      {},
      vk::ImageLayout::eUndefined};
  depth_image_data_.image = vk::raii::Image{device_, image_create_info};

  depth_image_data_.device_memory =
      AllocateDeviceMemory(depth_image_data_.image.getMemoryRequirements(),
                           vk::MemoryPropertyFlagBits::eDeviceLocal);

  depth_image_data_.image.bindMemory(*(depth_image_data_.device_memory), 0);
  depth_image_data_.image_view = vk::raii::ImageView{
      device_,
      vk::ImageViewCreateInfo{{},
                              *(depth_image_data_.image),
                              vk::ImageViewType::e2D,
                              depth_image_data_.format,
                              {},
                              {vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1}}};
}

void Rendering::MakeRenderPass() {
  std::vector<vk::AttachmentDescription> attachment_descriptions;

  attachment_descriptions.emplace_back(
      vk::AttachmentDescriptionFlags{}, color_image_data_.format, sample_count_,
      vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
      vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
      vk::ImageLayout::eUndefined, vk::ImageLayout::eColorAttachmentOptimal);

  attachment_descriptions.emplace_back(
      vk::AttachmentDescriptionFlags{}, depth_image_data_.format, sample_count_,
      vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eDontCare,
      vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
      vk::ImageLayout::eUndefined,
      vk::ImageLayout::eDepthStencilAttachmentOptimal);

  attachment_descriptions.emplace_back(
      vk::AttachmentDescriptionFlags{}, swapchain_data_.format,
      vk::SampleCountFlagBits::e1, vk::AttachmentLoadOp::eDontCare,
      vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare,
      vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined,
      vk::ImageLayout::ePresentSrcKHR);

  vk::AttachmentReference color_attachment{
      0, vk::ImageLayout::eColorAttachmentOptimal};
  vk::AttachmentReference depth_attachment{
      1, vk::ImageLayout::eDepthStencilAttachmentOptimal};
  vk::AttachmentReference resolve_attachment{
      2, vk::ImageLayout::eColorAttachmentOptimal};

  vk::SubpassDescription subpass_description{{},
                                             vk::PipelineBindPoint::eGraphics,
                                             {},
                                             color_attachment,
                                             resolve_attachment,
                                             &depth_attachment};

  vk::SubpassDependency subpass_dependency{
      VK_SUBPASS_EXTERNAL,
      {},
      vk::PipelineStageFlagBits::eColorAttachmentOutput |
          vk::PipelineStageFlagBits::eEarlyFragmentTests,
      vk::PipelineStageFlagBits::eColorAttachmentOutput |
          vk::PipelineStageFlagBits::eEarlyFragmentTests,
      {},
      vk::AccessFlagBits::eColorAttachmentWrite |
          vk::AccessFlagBits::eDepthStencilAttachmentWrite};

  vk::RenderPassCreateInfo render_pass_create_info{
      {}, attachment_descriptions, subpass_description, subpass_dependency};

  render_pass_ = vk::raii::RenderPass{device_, render_pass_create_info};
}

void Rendering::MakeFramebuffer() {
  std::vector<vk::ImageView> attachements(3);
  attachements[0] = *(color_image_data_.image_view);
  attachements[1] = *(depth_image_data_.image_view);

  vk::FramebufferCreateInfo framebuffer_create_info{
      {},
      *render_pass_,
      static_cast<uint32_t>(attachements.size()),
      attachements.data(),
      swapchain_data_.extent.width,
      swapchain_data_.extent.height,
      1};

  framebuffers_.reserve(swapchain_data_.image_views.size());
  for (const auto& image_view : swapchain_data_.image_views) {
    attachements[2] = *image_view;
    framebuffers_.emplace_back(device_, framebuffer_create_info);
  }
}

void Rendering::MakeVertexBuffer() {
  LoadModel("/resource/model/viking_room.obj", vertices_, indices_);
  VkDeviceSize buffer_size{sizeof(vertices_[0]) * vertices_.size()};

  vk::raii::Buffer staging_buffer{
      device_, {{}, buffer_size, vk::BufferUsageFlagBits::eTransferSrc}};
  vk::raii::DeviceMemory staging_device_memory{
      AllocateDeviceMemory(staging_buffer.getMemoryRequirements(),
                           vk::MemoryPropertyFlagBits::eHostVisible |
                               vk::MemoryPropertyFlagBits::eHostCoherent)};
  staging_buffer.bindMemory(*staging_device_memory, 0);
  CopyToDevice(staging_device_memory, vertices_.data(), buffer_size);

  vertex_buffer_data_.buffer =
      vk::raii::Buffer{device_,
                       {{},
                        buffer_size,
                        vk::BufferUsageFlagBits::eTransferDst |
                            vk::BufferUsageFlagBits::eVertexBuffer}};
  vertex_buffer_data_.device_memory = vk::raii::DeviceMemory{
      AllocateDeviceMemory(vertex_buffer_data_.buffer.getMemoryRequirements(),
                           vk::MemoryPropertyFlagBits::eDeviceLocal)};
  vertex_buffer_data_.buffer.bindMemory(*(vertex_buffer_data_.device_memory),
                                        0);

  CopyBuffer(staging_buffer, vertex_buffer_data_.buffer, buffer_size);
}

void Rendering::MakeIndexBuffer() {
  VkDeviceSize buffer_size{sizeof(indices_[0]) * indices_.size()};

  vk::raii::Buffer staging_buffer{
      device_, {{}, buffer_size, vk::BufferUsageFlagBits::eTransferSrc}};
  vk::raii::DeviceMemory staging_device_memory{
      AllocateDeviceMemory(staging_buffer.getMemoryRequirements(),
                           vk::MemoryPropertyFlagBits::eHostVisible |
                               vk::MemoryPropertyFlagBits::eHostCoherent)};
  staging_buffer.bindMemory(*staging_device_memory, 0);
  CopyToDevice(staging_device_memory, indices_.data(), buffer_size);

  index_buffer_data_.buffer =
      vk::raii::Buffer{device_,
                       {{},
                        buffer_size,
                        vk::BufferUsageFlagBits::eTransferDst |
                            vk::BufferUsageFlagBits::eIndexBuffer}};
  index_buffer_data_.device_memory = vk::raii::DeviceMemory{
      AllocateDeviceMemory(index_buffer_data_.buffer.getMemoryRequirements(),
                           vk::MemoryPropertyFlagBits::eDeviceLocal)};
  index_buffer_data_.buffer.bindMemory(*(index_buffer_data_.device_memory), 0);

  CopyBuffer(staging_buffer, index_buffer_data_.buffer, buffer_size);
}

void Rendering::MakeUniformBuffer() {
  VkDeviceSize buffer_size{sizeof(UniformBufferObject)};

  uniform_buffer_datas_.resize(kFramesInFlight);
  uniform_buffer_mapped_dates_.resize(kFramesInFlight);

  vk::BufferCreateInfo buffer_create_info{
      {}, buffer_size, vk::BufferUsageFlagBits::eUniformBuffer};

  for (uint32_t i = 0; i < kFramesInFlight; ++i) {
    uniform_buffer_datas_[i].buffer =
        vk::raii::Buffer{device_, buffer_create_info};
    uniform_buffer_datas_[i].device_memory =
        vk::raii::DeviceMemory{AllocateDeviceMemory(
            uniform_buffer_datas_[i].buffer.getMemoryRequirements(),
            vk::MemoryPropertyFlagBits::eHostVisible |
                vk::MemoryPropertyFlagBits::eHostCoherent)};

    uniform_buffer_datas_[i].buffer.bindMemory(
        *(uniform_buffer_datas_[i].device_memory), 0);

    uniform_buffer_mapped_dates_[i] = static_cast<uint8_t*>(
        uniform_buffer_datas_[i].device_memory.mapMemory(0, buffer_size));
  }
}

void Rendering::MakeTextureImage() {
  // load texture
  int tex_width, tex_height, tex_channels;
  stbi_uc* piexls{
      stbi_load(std::string{root_directory + "/resource/texture/viking_room.png"}.c_str(),
                &tex_width, &tex_height, &tex_channels, STBI_rgb_alpha)};

  if (!piexls) {
    throw std::runtime_error{"fail to load texture image"};
  }
  mip_level_count_ = static_cast<uint32_t>(std::floor(
                         std::log2(std::max(tex_width, tex_height)))) +
                     1;
  vk::DeviceSize image_size{
      static_cast<vk::DeviceSize>(tex_width * tex_height * 4)};

  // copy to staging buffer
  vk::raii::Buffer staging_buffer{
      device_, {{}, image_size, vk::BufferUsageFlagBits::eTransferSrc}};
  vk::raii::DeviceMemory staging_device_memory{
      AllocateDeviceMemory(staging_buffer.getMemoryRequirements(),
                           vk::MemoryPropertyFlagBits::eHostVisible |
                               vk::MemoryPropertyFlagBits::eHostCoherent)};
  staging_buffer.bindMemory(*staging_device_memory, 0);
  CopyToDevice(staging_device_memory, piexls, image_size);
  stbi_image_free(piexls);

  // create texture image data
  texture_image_data_.format = vk::Format::eR8G8B8A8Srgb;
  vk::ImageCreateInfo image_create_info{
      {},
      vk::ImageType::e2D,
      texture_image_data_.format,
      vk::Extent3D{static_cast<uint32_t>(tex_width),
                   static_cast<uint32_t>(tex_height), 1},
      mip_level_count_,
      1,
      vk::SampleCountFlagBits::e1,
      vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eTransferSrc |
          vk::ImageUsageFlagBits::eTransferDst |
          vk::ImageUsageFlagBits::eSampled,
      vk::SharingMode::eExclusive,
      {},
      vk::ImageLayout::eUndefined};
  texture_image_data_.image = vk::raii::Image{device_, image_create_info};
  texture_image_data_.device_memory =
      AllocateDeviceMemory(texture_image_data_.image.getMemoryRequirements(),
                           vk::MemoryPropertyFlagBits::eDeviceLocal);
  texture_image_data_.image.bindMemory(*(texture_image_data_.device_memory), 0);

  // copy buffer to image
  {
    vk::raii::CommandBuffer command_buffer{BeginSingleTimeCommand()};
    vk::ImageMemoryBarrier barrier{
        {},
        vk::AccessFlagBits::eTransferWrite,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eTransferDstOptimal,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        *(texture_image_data_.image),
        {vk::ImageAspectFlagBits::eColor, 0, mip_level_count_, 0, 1}};
    command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                   vk::PipelineStageFlagBits::eTransfer, {}, {},
                                   {}, barrier);
    EndSingleTimeCommand(command_buffer);
  }

  {
    vk::raii::CommandBuffer command_buffer{BeginSingleTimeCommand()};

    vk::BufferImageCopy buffer_image_copy{
        {},
        {},
        {},
        {vk::ImageAspectFlagBits::eColor, 0, 0, 1},
        {0, 0, 0},
        {static_cast<uint32_t>(tex_width), static_cast<uint32_t>(tex_height),
         1}};

    command_buffer.copyBufferToImage(
        *staging_buffer, *(texture_image_data_.image),
        vk::ImageLayout::eTransferDstOptimal, buffer_image_copy);

    EndSingleTimeCommand(command_buffer);
  }

  // generate mipmaps
  vk::FormatProperties format_properties{
      physical_device_.getFormatProperties(texture_image_data_.format)};
  if (!(format_properties.optimalTilingFeatures &
        vk::FormatFeatureFlagBits::eSampledImageFilterLinear)) {
    throw std::runtime_error{
        "texture image format does not support linear blitting"};
  }

  {
    vk::raii::CommandBuffer command_buffer{BeginSingleTimeCommand()};

    vk::ImageMemoryBarrier barrier{
        {},
        vk::AccessFlagBits::eTransferWrite,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::eTransferDstOptimal,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        *(texture_image_data_.image),
        {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

    int32_t mip_width{tex_width};
    int32_t mip_height{tex_height};

    for (uint32_t i = 1; i < mip_level_count_; ++i) {
      barrier.subresourceRange.baseMipLevel = i - 1;
      barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
      barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
      barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
      barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

      command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                     vk::PipelineStageFlagBits::eTransfer, {},
                                     nullptr, nullptr, barrier);

      vk::ImageSubresourceLayers src_subresource{
          vk::ImageAspectFlagBits::eColor, i - 1, 0, 1};
      std::array<vk::Offset3D, 2> src_offsets{
          vk::Offset3D{0, 0, 0}, vk::Offset3D{mip_width, mip_height, 1}};
      vk::ImageSubresourceLayers dst_subresource{
          vk::ImageAspectFlagBits::eColor, i, 0, 1};
      std::array<vk::Offset3D, 2> dst_offsets{
          vk::Offset3D{0, 0, 0},
          vk::Offset3D{mip_width > 1 ? mip_width / 2 : 1,
                       mip_height > 1 ? mip_height / 2 : 1, 1}};

      vk::ImageBlit blit{src_subresource, src_offsets, dst_subresource,
                         dst_offsets};

      command_buffer.blitImage(
          *(texture_image_data_.image), vk::ImageLayout::eTransferSrcOptimal,
          *(texture_image_data_.image), vk::ImageLayout::eTransferDstOptimal,
          blit, vk::Filter::eLinear);

      barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
      barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
      barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
      barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

      command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                     vk::PipelineStageFlagBits::eFragmentShader,
                                     {}, nullptr, nullptr, barrier);

      if (mip_width > 1) {
        mip_width /= 2;
      }
      if (mip_height > 1) {
        mip_height /= 2;
      }
    }

    barrier.subresourceRange.baseMipLevel = mip_level_count_ - 1;
    barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
    barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
    barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

    command_buffer.pipelineBarrier(vk::PipelineStageFlagBits::eTransfer,
                                   vk::PipelineStageFlagBits::eFragmentShader,
                                   {}, nullptr, nullptr, barrier);

    EndSingleTimeCommand(command_buffer);
  }

  texture_image_data_.image_view = vk::raii::ImageView{
      device_, vk::ImageViewCreateInfo{{},
                                       *(texture_image_data_.image),
                                       vk::ImageViewType::e2D,
                                       texture_image_data_.format,
                                       {},
                                       {vk::ImageAspectFlagBits::eColor, 0,
                                        mip_level_count_, 0, 1}}};
}

void Rendering::MakeTextureSampler() {
  sampler_ = vk::raii::Sampler{
      device_, vk::SamplerCreateInfo{{},
                                     vk::Filter::eLinear,
                                     vk::Filter::eLinear,
                                     vk::SamplerMipmapMode::eLinear,
                                     vk::SamplerAddressMode::eRepeat,
                                     vk::SamplerAddressMode::eRepeat,
                                     vk::SamplerAddressMode::eRepeat,
                                     {},
                                     VK_TRUE,
                                     max_anisotropy_,
                                     VK_FALSE,
                                     vk::CompareOp::eAlways,
                                     0.0f,
                                     static_cast<float>(mip_level_count_),
                                     vk::BorderColor::eIntOpaqueBlack,
                                     VK_FALSE}};
}

void Rendering::MakeDescriptorSetLayout() {
  std::vector<std::tuple<vk::DescriptorType, uint32_t, vk::ShaderStageFlags>>
      bingding_data{{vk::DescriptorType::eUniformBuffer, 1,
                     vk::ShaderStageFlagBits::eVertex},
                    {vk::DescriptorType::eCombinedImageSampler, 1,
                     vk::ShaderStageFlagBits::eFragment}};

  std::vector<vk::DescriptorSetLayoutBinding> bindings(bingding_data.size());

  for (uint32_t i = 0; i < bingding_data.size(); ++i) {
    bindings[i] = vk::DescriptorSetLayoutBinding{
        static_cast<uint32_t>(i), std::get<0>(bingding_data[i]),
        std::get<1>(bingding_data[i]), std::get<2>(bingding_data[i])};
  }

  vk::DescriptorSetLayoutCreateInfo descriptor_set_layout_create_info{{},
                                                                      bindings};

  descriptor_set_layout_ =
      vk::raii::DescriptorSetLayout{device_, descriptor_set_layout_create_info};
}

void Rendering::MakeDescriptorPool() {
  std::vector<vk::DescriptorPoolSize> pool_sizes{
      {vk::DescriptorType::eUniformBuffer,
       static_cast<uint32_t>(kFramesInFlight)},
      {vk::DescriptorType::eCombinedImageSampler,
       static_cast<uint32_t>(kFramesInFlight)}};

  uint32_t max_sets{std::accumulate(
      pool_sizes.begin(), pool_sizes.end(), static_cast<uint32_t>(0),
      [](uint32_t sum, const vk::DescriptorPoolSize& dps) {
        return sum + dps.descriptorCount;
      })};

  vk::DescriptorPoolCreateInfo descriptor_pool_create_info{
      vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, max_sets,
      pool_sizes};
  descriptor_pool_ =
      vk::raii::DescriptorPool{device_, descriptor_pool_create_info};
}

void Rendering::MakeDescriptorSet() {
  std::vector<vk::DescriptorSetLayout> descriptor_set_layouts(
      kFramesInFlight, *descriptor_set_layout_);

  descriptor_sets_ = std::move(vk::raii::DescriptorSets{
      device_, {*descriptor_pool_, descriptor_set_layouts}});

  for (uint32_t i = 0; i < kFramesInFlight; ++i) {
    vk::DescriptorBufferInfo buffer_info{*(uniform_buffer_datas_[i].buffer), 0,
                                         sizeof(UniformBufferObject)};

    vk::DescriptorImageInfo image_info{*sampler_,
                                       *(texture_image_data_.image_view),
                                       vk::ImageLayout::eShaderReadOnlyOptimal};

    std::array<vk::WriteDescriptorSet, 2> descriptir_writes{
        vk::WriteDescriptorSet{
            *(descriptor_sets_[i]),
            0,
            0,
            1,
            vk::DescriptorType::eUniformBuffer,
            nullptr,
            &buffer_info,
        },
        vk::WriteDescriptorSet{
            *(descriptor_sets_[i]),
            1,
            0,
            1,
            vk::DescriptorType::eCombinedImageSampler,
            &image_info,
            nullptr,
        }};

    device_.updateDescriptorSets(descriptir_writes, nullptr);
  }
}

void Rendering::MakePipeline() {
  vk::raii::ShaderModule vertex_shader_module{
      MakeShaderModule("/resource/shader/generated/shader.vert.spirv")};
  vk::raii::ShaderModule fragment_shader_module{
      MakeShaderModule("/resource/shader/generated/shader.frag.spirv")};

  std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_create_infos{
      {{},
       vk::ShaderStageFlagBits::eVertex,
       *vertex_shader_module,
       "main",
       nullptr},
      {{},
       vk::ShaderStageFlagBits::eFragment,
       *fragment_shader_module,
       "main",
       nullptr}};

  vk::PipelineVertexInputStateCreateInfo vertex_input_state_create_info;

  uint32_t vertex_stride{static_cast<uint32_t>(sizeof(Vertex))};

  vk::VertexInputBindingDescription vertex_input_binding_description{
      0, vertex_stride, vk::VertexInputRate::eVertex};

  std::vector<vk::VertexInputAttributeDescription>
      vertex_input_attribute_descriptions;

  std::vector<std::pair<vk::Format, uint32_t>>
      vertex_input_attribute_format_offset{
          {vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos)},
          {vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color)},
          {vk::Format::eR32G32Sfloat, offsetof(Vertex, tex_coord)},
      };

  vertex_input_attribute_descriptions.reserve(
      vertex_input_attribute_format_offset.size());

  for (uint32_t i = 0; i < vertex_input_attribute_format_offset.size(); i++) {
    vertex_input_attribute_descriptions.emplace_back(
        i, 0, vertex_input_attribute_format_offset[i].first,
        vertex_input_attribute_format_offset[i].second);
  }
  vertex_input_state_create_info.setVertexBindingDescriptions(
      vertex_input_binding_description);
  vertex_input_state_create_info.setVertexAttributeDescriptions(
      vertex_input_attribute_descriptions);

  vk::PipelineInputAssemblyStateCreateInfo input_assembly_state_create_info{
      {}, vk::PrimitiveTopology::eTriangleList, VK_FALSE};

  vk::PipelineViewportStateCreateInfo viewport_state_create_info{
      {}, 1, nullptr, 1, nullptr};

  vk::PipelineRasterizationStateCreateInfo rasterization_state_create_info{
      {},
      VK_FALSE,
      VK_FALSE,
      vk::PolygonMode::eFill,
      vk::CullModeFlagBits::eBack,
      vk::FrontFace::eClockwise,
      VK_FALSE,
      0.0f,
      0.0f,
      0.0f,
      1.0f};

  vk::PipelineMultisampleStateCreateInfo multisample_state_create_info{
      {}, sample_count_};

  vk::PipelineDepthStencilStateCreateInfo depth_stencil_state_create_info{
      {}, VK_TRUE, VK_TRUE, vk::CompareOp::eLess, VK_FALSE, VK_FALSE,
  };

  vk::ColorComponentFlags color_component_flags{
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
      vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};
  vk::PipelineColorBlendAttachmentState color_blend_attachment_state{
      VK_FALSE,          vk::BlendFactor::eZero, vk::BlendFactor::eZero,
      vk::BlendOp::eAdd, vk::BlendFactor::eZero, vk::BlendFactor::eZero,
      vk::BlendOp::eAdd, color_component_flags};

  vk::PipelineColorBlendStateCreateInfo color_blend_state_create_info{
      {},
      VK_FALSE,
      vk::LogicOp::eCopy,
      color_blend_attachment_state,
      {{0.0f, 0.0f, 0.0f, 0.0f}}};

  std::array<vk::DynamicState, 2> dynamic_states{vk::DynamicState::eViewport,
                                                 vk::DynamicState::eScissor};
  vk::PipelineDynamicStateCreateInfo dynamic_state_create_info{{},
                                                               dynamic_states};

  pipeline_layout_ =
      vk::raii::PipelineLayout{device_, {{}, *descriptor_set_layout_}};

  vk::GraphicsPipelineCreateInfo graphics_pipeline_create_info{
      {},
      shader_stage_create_infos,
      &vertex_input_state_create_info,
      &input_assembly_state_create_info,
      nullptr,
      &viewport_state_create_info,
      &rasterization_state_create_info,
      &multisample_state_create_info,
      &depth_stencil_state_create_info,
      &color_blend_state_create_info,
      &dynamic_state_create_info,
      *pipeline_layout_,
      *render_pass_};

  pipeline_cache_ = vk::raii::PipelineCache{device_, {}};

  pipeline_ = vk::raii::Pipeline{device_, pipeline_cache_,
                                 graphics_pipeline_create_info};
}

void Rendering::RecreateSwapchin() {
  int width{0};
  int height{0};
  glfwGetFramebufferSize(surface_data_.glfw_window, &width, &height);

  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(surface_data_.glfw_window, &width, &height);
    glfwWaitEvents();
  }

  device_.waitIdle();

  depth_image_data_.image_view.clear();
  depth_image_data_.image.clear();
  depth_image_data_.device_memory.clear();

  color_image_data_.image_view.clear();
  color_image_data_.image.clear();
  color_image_data_.device_memory.clear();

  for (auto& framebuffer : framebuffers_) {
    framebuffer.clear();
  }
  framebuffers_ = std::vector<vk::raii::Framebuffer>{};

  for (auto& image_view : swapchain_data_.image_views) {
    image_view.clear();
  }
  swapchain_data_.image_views = std::vector<vk::raii::ImageView>{};

  swapchain_data_.swapchain.clear();

  MakeSwapchain();
  MakeColorImage();
  MakeDepthImage();
  MakeFramebuffer();
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
  std::cout << std::string("\t") << "message          = <"
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

void Rendering::FramebufferSizeCallback(GLFWwindow* window, int width,
                                        int height) {
  auto rendering{
      reinterpret_cast<Rendering*>(glfwGetWindowUserPointer(window))};
  rendering->framebuffer_resized_ = true;
}

vk::raii::DeviceMemory Rendering::AllocateDeviceMemory(
    const vk::MemoryRequirements& memory_requirements,
    vk::MemoryPropertyFlags memory_properties_flags) {
  vk::PhysicalDeviceMemoryProperties memory_properties{
      physical_device_.getMemoryProperties()};

  uint32_t memory_type_bits{memory_requirements.memoryTypeBits};

  uint32_t type_index{static_cast<uint32_t>(~0)};
  for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
    if ((memory_type_bits & 1) &&
        ((memory_properties.memoryTypes[i].propertyFlags &
          memory_properties_flags) == memory_properties_flags)) {
      type_index = i;
      break;
    }
    memory_type_bits >>= 1;
  }
  if (type_index == static_cast<uint32_t>(~0)) {
    throw std::runtime_error{"fail to find required memory"};
  }

  vk::MemoryAllocateInfo memory_allocate_info{memory_requirements.size,
                                              type_index};

  return vk::raii::DeviceMemory{device_, memory_allocate_info};
}

void Rendering::CopyBuffer(const vk::raii::Buffer& src_buffer,
                           const vk::raii::Buffer& dst_buffer,
                           vk::DeviceSize size) {
  vk::raii::CommandBuffer command_buffer{BeginSingleTimeCommand()};

  vk::BufferCopy buffer_copy{0, 0, size};

  command_buffer.copyBuffer(*src_buffer, *dst_buffer, buffer_copy);

  EndSingleTimeCommand(command_buffer);
}

vk::raii::CommandBuffer Rendering::BeginSingleTimeCommand() {
  vk::CommandBufferAllocateInfo command_buffer_allocate_info{
      *command_pool_, vk::CommandBufferLevel::ePrimary, 1};

  vk::raii::CommandBuffer command_buffer = std::move(
      vk::raii::CommandBuffers{device_, command_buffer_allocate_info}.front());

  vk::CommandBufferBeginInfo command_begin_info{
      vk::CommandBufferUsageFlagBits::eOneTimeSubmit};

  command_buffer.begin(command_begin_info);

  return command_buffer;
}

void Rendering::EndSingleTimeCommand(
    const vk::raii::CommandBuffer& command_buffer) {
  command_buffer.end();

  vk::SubmitInfo submit_info{nullptr, nullptr, *command_buffer};
  graphics_queue_.submit(submit_info, nullptr);

  graphics_queue_.waitIdle();
}

vk::raii::ShaderModule Rendering::MakeShaderModule(
    const std::string& shader_file) {
  std::vector<char> buffer{ReadFile(shader_file)};

  vk::ShaderModuleCreateInfo shader_module_create_info{
      {}, buffer.size(), reinterpret_cast<const uint32_t*>(buffer.data())};

  return vk::raii::ShaderModule{device_, shader_module_create_info};
}
}  // namespace luka
