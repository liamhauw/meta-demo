#include "luka/function/rendering.h"

namespace luka {
const TBuiltInResource kDefaultTBuiltInResource{
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .maxMeshOutputVerticesNV = */ 256,
    /* .maxMeshOutputPrimitivesNV = */ 512,
    /* .maxMeshWorkGroupSizeX_NV = */ 32,
    /* .maxMeshWorkGroupSizeY_NV = */ 1,
    /* .maxMeshWorkGroupSizeZ_NV = */ 1,
    /* .maxTaskWorkGroupSizeX_NV = */ 32,
    /* .maxTaskWorkGroupSizeY_NV = */ 1,
    /* .maxTaskWorkGroupSizeZ_NV = */ 1,
    /* .maxMeshViewCountNV = */ 4,
    /* .maxMeshOutputVerticesEXT = */ 256,
    /* .maxMeshOutputPrimitivesEXT = */ 256,
    /* .maxMeshWorkGroupSizeX_EXT = */ 128,
    /* .maxMeshWorkGroupSizeY_EXT = */ 128,
    /* .maxMeshWorkGroupSizeZ_EXT = */ 128,
    /* .maxTaskWorkGroupSizeX_EXT = */ 128,
    /* .maxTaskWorkGroupSizeY_EXT = */ 128,
    /* .maxTaskWorkGroupSizeZ_EXT = */ 128,
    /* .maxMeshViewCountEXT = */ 4,
    /* .maxDualSourceDrawBuffersEXT = */ 1,

    /* .limits = */
    {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }};

struct VertexPC {
  float x, y, z, w;  // Position
  float r, g, b, a;  // Color
};

const VertexPC kColoredCubeData[]{
    // red face
    {-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
    // green face
    {-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
    // blue face
    {-1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    {-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f},
    // yellow face
    {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},
    // magenta face
    {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
    // cyan face
    {1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    {1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    {1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
    {-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
};

Rendering::Rendering() {
  MakeInstance();
  MakeSurface();
  MakePhysicalDevice();
  MakeDevice();
  MakeQueue();
  MakeSyncObjects();
  MakeCommandPool();
  MakeCommandBuffer();
  MakeSwapchain();
  MakeDepthImage();
  MakeRenderPass();
  MakeFramebuffer();
  
  MakeShaderModule();
  MakeDescriptorSetLayout();
  MakeDescriptorPool();
  MakeDescriptorSet();
  MakePipelineLayout();
  MakePipelineCache();
  MakePipeline();

  MakeVertexBuffer();
  MakeUniformBuffer();
  UpdateDescriptorSets();
}

void Rendering::Tick() {
  glfwPollEvents();

  // device_.waitForFences(*fence_[current_frame_], VK_TRUE, UINT64_MAX);
  // vk::Result result;
  // uint32_t image_index;
  // std::tie(result, image_index) = swapchain_data_.swapchain.acquireNextImage(
  //     UINT64_MAX, *image_available_semaphores_[current_frame_]);

  // -----
  vk::raii::Semaphore image_acquired_semaphore{device_, vk::SemaphoreCreateInfo{}};

  vk::Result result;
  uint32_t image_index;
  std::tie(result, image_index) =
      swapchain_data_.swapchain.acquireNextImage(100000000, *image_acquired_semaphore);
  assert(result == vk::Result::eSuccess);
  assert(image_index < swapchain_data_.images.size());

  command_buffer_.begin({});

  std::array<vk::ClearValue, 2> clear_values;
  clear_values[0].color = vk::ClearColorValue{0.2f, 0.2f, 0.2f, 0.2f};
  clear_values[1].depthStencil = vk::ClearDepthStencilValue{1.0f, 0};

  vk::RenderPassBeginInfo render_pass_begin_info{
      *render_pass_, *framebuffers_[image_index],
      vk::Rect2D{vk::Offset2D{0, 0}, surface_data_.extent}, clear_values};

  command_buffer_.beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
  command_buffer_.bindPipeline(vk::PipelineBindPoint::eGraphics, *graphics_pipeline_);
  command_buffer_.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, *pipeline_layout_, 0,
                                     {*descriptor_set_}, nullptr);
  command_buffer_.bindVertexBuffers(0, {*vertex_buffer_data_.buffer}, {0});
  command_buffer_.setViewport(
      0, vk::Viewport{0.0f, 0.0f, static_cast<float>(kWidth),
                      static_cast<float>(kHeight), 0.0f, 1.0f});
  command_buffer_.setScissor(0, vk::Rect2D{vk::Offset2D{0, 0}, surface_data_.extent});

  command_buffer_.draw(12 * 3, 1, 0, 0);
  command_buffer_.endRenderPass();
  command_buffer_.end();

  vk::raii::Fence draw_fence{device_, vk::FenceCreateInfo{}};

  vk::PipelineStageFlags wait_destination_stage_mask{
      vk::PipelineStageFlagBits::eColorAttachmentOutput};
  vk::SubmitInfo submit_info{*image_acquired_semaphore, wait_destination_stage_mask,
                             *command_buffer_};
  graphics_queue_.submit(submit_info, *draw_fence);

  while (vk::Result::eTimeout == device_.waitForFences({*draw_fence}, 1, 100000000)) {
    ;
  }

  vk::PresentInfoKHR present_info_khr{nullptr, *(swapchain_data_.swapchain), image_index};

  result = present_queue_.presentKHR(present_info_khr);

  switch (result) {
    case vk::Result::eSuccess:
      break;
    case vk::Result::eSuboptimalKHR:
      std::cout << "vk::Queue::presentKHR returned vk::Result::eSuboptimalKHR !\n";
      break;
    default:
      assert(false);  // an unexpected result is returned !
  }
}

bool Rendering::ShouldClose() { return glfwWindowShouldClose(surface_data_.glfw_window); }

void Rendering::MakeInstance() {
  vk::raii::Context context;

  vk::ApplicationInfo application_info{"luka", VK_MAKE_VERSION(1, 0, 0), "luka",
                                       VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_0};

  std::vector<const char*> required_layers;
  std::vector<const char*> required_extensions;

  static auto glfw_context{GlfwContext{}};
  uint32_t glfw_extension_count{0};
  const char** glfw_extensions{glfwGetRequiredInstanceExtensions(&glfw_extension_count)};
  required_extensions = {glfw_extensions, glfw_extensions + glfw_extension_count};

#ifdef __APPLE__
  required_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
  required_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
#endif

#ifndef NDEBUG
  const std::vector<vk::LayerProperties> layer_properties{
      context.enumerateInstanceLayerProperties()};
  const std::vector<vk::ExtensionProperties> extension_properties{
      context.enumerateInstanceExtensionProperties()};
#endif

  std::vector<const char*> enabled_layers;
  std::vector<const char*> enabled_extensions;

  enabled_layers.reserve(required_layers.size());
  for (const char* layer : required_layers) {
    assert(std::find_if(layer_properties.begin(), layer_properties.end(),
                        [layer](const vk::LayerProperties& lp) {
                          return (strcmp(layer, lp.layerName) == 0);
                        }) != layer_properties.end());
    enabled_layers.push_back(layer);
  }

  enabled_extensions.reserve(required_extensions.size());
  for (const char* extension : required_extensions) {
    assert(std::find_if(extension_properties.begin(), extension_properties.end(),
                        [extension](const vk::ExtensionProperties& ep) {
                          return (strcmp(extension, ep.extensionName) == 0);
                        }) != extension_properties.end());
    enabled_extensions.push_back(extension);
  }

#ifndef NDEBUG
  if (std::find(required_layers.begin(), required_layers.end(), "VK_LAYER_KHRONOS_validation") ==
          required_layers.end() &&
      std::find_if(layer_properties.begin(), layer_properties.end(),
                   [](const vk::LayerProperties& lp) {
                     return (strcmp("VK_LAYER_KHRONOS_validation", lp.layerName) == 0);
                   }) != layer_properties.end()) {
    enabled_layers.push_back("VK_LAYER_KHRONOS_validation");
  }

  if (std::find(required_extensions.begin(), required_extensions.end(),
                VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == required_extensions.end() &&
      std::find_if(extension_properties.begin(), extension_properties.end(),
                   [](vk::ExtensionProperties const& ep) {
                     return (strcmp(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, ep.extensionName) == 0);
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

  vk::StructureChain<vk::InstanceCreateInfo, vk::DebugUtilsMessengerCreateInfoEXT> info_chain{
      {flags, &application_info, enabled_layers, enabled_extensions},
      {{}, message_severity_flags, message_type_flags, &DebugUtilsMessengerCallback}};
#else
  vk::StructureChain<vk::InstanceCreateInfo> info_chain{
      {flags, &application_info, enabled_layers, enabled_extensions}};
#endif

  instance_ = vk::raii::Instance{context, info_chain.get<vk::InstanceCreateInfo>()};

#ifndef NDEBUG
  vk::DebugUtilsMessengerCreateInfoEXT debug_utils_messenger_create_info{
      {},
      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
          vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
      vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
          vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
          vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
      &DebugUtilsMessengerCallback};

  vk::raii::DebugUtilsMessengerEXT debug_utils_messneger{instance_,
                                                         debug_utils_messenger_create_info};
#endif
}

void Rendering::MakeSurface() {
  surface_data_.extent = vk::Extent2D{kWidth, kHeight};

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  surface_data_.glfw_window = glfwCreateWindow(
      kWidth, kHeight, "luka", nullptr, nullptr);
  glfwSetWindowUserPointer(surface_data_.glfw_window, this);
  glfwSetFramebufferSizeCallback(surface_data_.glfw_window, FramebufferSizeCallback);

  VkSurfaceKHR surface;
  glfwCreateWindowSurface(static_cast<VkInstance>(*instance_), surface_data_.glfw_window, nullptr,
                          &surface);

  surface_data_.surface = vk::raii::SurfaceKHR{instance_, surface};
}

void Rendering::MakePhysicalDevice() {
  physical_device_ = vk::raii::PhysicalDevices{instance_}.front();

  std::vector<vk::QueueFamilyProperties> queue_family_properties{
      physical_device_.getQueueFamilyProperties()};
  assert(queue_family_properties.size() < std::numeric_limits<uint32_t>::max());

  std::vector<vk::QueueFamilyProperties>::const_iterator graphics_queue_family_property{
      std::find_if(queue_family_properties.begin(), queue_family_properties.end(),
                   [](vk::QueueFamilyProperties const& qfp) {
                     return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
                   })};
  assert(graphics_queue_family_property != queue_family_properties.end());
  uint32_t graphics_queue_family_index{static_cast<uint32_t>(
      std::distance(queue_family_properties.cbegin(), graphics_queue_family_property))};

  if (physical_device_.getSurfaceSupportKHR(graphics_queue_family_index, *surface_data_.surface)) {
    gp_queue_family_index_ =
        std::make_pair(graphics_queue_family_index, graphics_queue_family_index);
    return;
  }

  for (size_t i = 0; i < queue_family_properties.size(); ++i) {
    if ((queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
        physical_device_.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface_data_.surface)) {
      gp_queue_family_index_ = std::make_pair(static_cast<uint32_t>(i), static_cast<uint32_t>(i));

      return;
    }
  }

  for (size_t i = 0; i < queue_family_properties.size(); i++) {
    if (physical_device_.getSurfaceSupportKHR(static_cast<uint32_t>(i), *surface_data_.surface)) {
      gp_queue_family_index_ =
          std::make_pair(graphics_queue_family_index, static_cast<uint32_t>(i));
      return;
    }
  }

  throw std::runtime_error{"fail to find queues for both graphics or present"};
}

void Rendering::MakeDevice() {
  std::vector<const char*> required_extensions;

  required_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

#ifdef __APPLE__
  required_extensions.push_back("VK_KHR_portability_subset");
#endif

  std::vector<const char*> enabled_extensions;
  enabled_extensions.reserve(required_extensions.size());
  for (const char* extension : required_extensions) {
    enabled_extensions.push_back(extension);
  }

  float queue_priority{0.0f};
  vk::DeviceQueueCreateInfo device_queue_create_info{
      {}, gp_queue_family_index_.first, 1, &queue_priority};
  vk::DeviceCreateInfo device_create_info{{}, device_queue_create_info, {}, enabled_extensions};

  device_ = vk::raii::Device{physical_device_, device_create_info};
}

void Rendering::MakeQueue() {
  graphics_queue_ = vk::raii::Queue{device_, gp_queue_family_index_.first, 0};
  present_queue_ = vk::raii::Queue{device_, gp_queue_family_index_.second, 0};
}

void Rendering::MakeSyncObjects(){
    // fence_.reserve(kFrameInFlight);
    // image_available_semaphores_.reserve(kFrameInFlight);
    // render_finished_semaphores_.reserve(kFrameInFlight);

    // vk::FenceCreateInfo
    // fence_create_info{vk::FenceCreateFlagBits::eSignaled};
    // vk::SemaphoreCreateInfo semaphore_create_info{};

    // for (size_t i = 0; i < kFrameInFlight; ++i) {
    //   fence_[i] = vk::raii::Fence{device_, fence_create_info};
    //   image_available_semaphores_[i] =
    //       vk::raii::Semaphore{device_, semaphore_create_info};
    //   render_finished_semaphores_[i] =
    //       vk::raii::Semaphore{device_, semaphore_create_info};
    // };
};

void Rendering::MakeCommandPool() {
  command_pool_ = vk::raii::CommandPool{
      device_, {vk::CommandPoolCreateFlagBits::eResetCommandBuffer, gp_queue_family_index_.first}};
}

void Rendering::MakeCommandBuffer() {
  vk::CommandBufferAllocateInfo command_buffer_allocate_info{*command_pool_,
                                                             vk::CommandBufferLevel::ePrimary, 1};
  command_buffer_ =
      std::move(vk::raii::CommandBuffers{device_, command_buffer_allocate_info}.front());
}

void Rendering::MakeSwapchain() {
  vk::SurfaceFormatKHR picked_format{PickSurfaceFormat()};

  swapchain_data_.format = picked_format.format;

  vk::SurfaceCapabilitiesKHR surface_capabilities{
      physical_device_.getSurfaceCapabilitiesKHR(*surface_data_.surface)};
  vk::Extent2D swapchain_extent;
  if (surface_capabilities.currentExtent.width == std::numeric_limits<uint32_t>::max()) {
    swapchain_extent.width =
        Clamp(kWidth, surface_capabilities.minImageExtent.width,
              surface_capabilities.maxImageExtent.width);
    swapchain_extent.height =
        Clamp(kHeight, surface_capabilities.minImageExtent.height,
              surface_capabilities.maxImageExtent.height);
  } else {
    swapchain_extent = surface_capabilities.currentExtent;
  }

  vk::SurfaceTransformFlagBitsKHR pre_transform{
      (surface_capabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity)
          ? vk::SurfaceTransformFlagBitsKHR::eIdentity
          : surface_capabilities.currentTransform};

  vk::CompositeAlphaFlagBitsKHR composite_alpha{
      (surface_capabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied)
          ? vk::CompositeAlphaFlagBitsKHR::ePreMultiplied
      : (surface_capabilities.supportedCompositeAlpha &
         vk::CompositeAlphaFlagBitsKHR::ePostMultiplied)
          ? vk::CompositeAlphaFlagBitsKHR::ePostMultiplied
      : (surface_capabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit)
          ? vk::CompositeAlphaFlagBitsKHR::eInherit
          : vk::CompositeAlphaFlagBitsKHR::eOpaque};

  std::vector<vk::PresentModeKHR> present_modes{
      physical_device_.getSurfacePresentModesKHR(*surface_data_.surface)};

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
      *surface_data_.surface,
      surface_capabilities.minImageCount,
      swapchain_data_.format,
      picked_format.colorSpace,
      swapchain_extent,
      1,
      vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransferSrc,
      vk::SharingMode::eExclusive,
      {},
      pre_transform,
      composite_alpha,
      picked_mode,
      true,
      {}};

  if (gp_queue_family_index_.first != gp_queue_family_index_.second) {
    uint32_t queue_family_indices[2]{gp_queue_family_index_.first, gp_queue_family_index_.second};
    swapchain_create_info.imageSharingMode = vk::SharingMode::eConcurrent;
    swapchain_create_info.queueFamilyIndexCount = 2;
    swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
  }

  swapchain_data_.swapchain = vk::raii::SwapchainKHR{device_, swapchain_create_info};

  swapchain_data_.images = swapchain_data_.swapchain.getImages();

  swapchain_data_.image_views.reserve(swapchain_data_.images.size());
  vk::ImageViewCreateInfo image_view_create_info{{},
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

void Rendering::MakeDepthImage() {
  depth_image_data_.format = vk::Format::eD16Unorm;

  vk::ImageCreateInfo image_create_info{
      {},
      vk::ImageType::e2D,
      depth_image_data_.format,
      vk::Extent3D{surface_data_.extent, 1},
      1,
      1,
      vk::SampleCountFlagBits::e1,
      vk::ImageTiling::eOptimal,
      vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eSampled,
      vk::SharingMode::eExclusive,
      {},
      vk::ImageLayout::eUndefined};
  depth_image_data_.image = vk::raii::Image{device_, image_create_info};

  depth_image_data_.device_memory = AllocateDeviceMemory(
      depth_image_data_.image.getMemoryRequirements(), vk::MemoryPropertyFlagBits::eDeviceLocal);

  depth_image_data_.image.bindMemory(*(depth_image_data_.device_memory), 0);
  depth_image_data_.image_view = vk::raii::ImageView{
      device_, vk::ImageViewCreateInfo{{},
                                       *(depth_image_data_.image),
                                       vk::ImageViewType::e2D,
                                       depth_image_data_.format,
                                       {},
                                       {vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1}}};
}

void Rendering::MakeRenderPass() {
  std::vector<vk::AttachmentDescription> attachment_descriptions;

  assert(swapchain_data_.format != vk::Format::eUndefined);
  attachment_descriptions.emplace_back(
      vk::AttachmentDescriptionFlags{}, swapchain_data_.format, vk::SampleCountFlagBits::e1,
      vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore, vk::AttachmentLoadOp::eDontCare,
      vk::AttachmentStoreOp::eDontCare, vk::ImageLayout::eUndefined,
      vk::ImageLayout::ePresentSrcKHR);

  if (depth_image_data_.format != vk::Format::eUndefined) {
    attachment_descriptions.emplace_back(
        vk::AttachmentDescriptionFlags{}, depth_image_data_.format, vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eDontCare,
        vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined, vk::ImageLayout::eDepthStencilAttachmentOptimal);
  }

  vk::AttachmentReference color_attachment{0, vk::ImageLayout::eColorAttachmentOptimal};
  vk::AttachmentReference depth_attachment{1, vk::ImageLayout::eDepthStencilAttachmentOptimal};

  vk::SubpassDescription subpass_description{
      {}, vk::PipelineBindPoint::eGraphics, {}, color_attachment, {}, &depth_attachment};

  vk::RenderPassCreateInfo render_pass_create_info{
      {}, attachment_descriptions, subpass_description};

  render_pass_ = vk::raii::RenderPass{device_, render_pass_create_info};
}

void Rendering::MakeFramebuffer() {
  vk::ImageView attachements[2];
  attachements[1] =
      &(depth_image_data_.image_view) ? *(depth_image_data_.image_view) : vk::ImageView{};

  vk::FramebufferCreateInfo framebuffer_create_info{
      {},
      *render_pass_,
      static_cast<uint32_t>(&(depth_image_data_.image_view) ? 2 : 1),
      attachements,
      kWidth,
      kHeight,
      1};

  framebuffers_.reserve(swapchain_data_.image_views.size());
  for (const auto& image_view : swapchain_data_.image_views) {
    attachements[0] = *image_view;
    framebuffers_.push_back(vk::raii::Framebuffer{device_, framebuffer_create_info});
  }
}

void Rendering::MakeShaderModule() {
  const std::string vertex_glsl_shader = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (std140, binding = 0) uniform buffer
{
  mat4 mvp;
} uniformBuffer;

layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 inColor;

layout (location = 0) out vec4 outColor;

void main()
{
  outColor = inColor;
  gl_Position = uniformBuffer.mvp * pos;
}
)";

  const std::string fragment_glsl_shader = R"(
#version 400

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 color;

layout (location = 0) out vec4 outColor;

void main()
{
  outColor = color;
}
)";

  glslang::InitializeProcess();
  vertex_shader_module_ = MakeShaderModule(vk::ShaderStageFlagBits::eVertex, vertex_glsl_shader);
  fragment_shader_module_ =
      MakeShaderModule(vk::ShaderStageFlagBits::eFragment, fragment_glsl_shader);
  glslang::FinalizeProcess();
}

void Rendering::MakeDescriptorSetLayout() {
  std::vector<std::tuple<vk::DescriptorType, uint32_t, vk::ShaderStageFlags>> bingding_data{
      {vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex}};

  std::vector<vk::DescriptorSetLayoutBinding> bindings(bingding_data.size());

  for (size_t i = 0; i < bingding_data.size(); ++i) {
    bindings[i] = vk::DescriptorSetLayoutBinding{
        static_cast<uint32_t>(i), std::get<0>(bingding_data[i]), std::get<1>(bingding_data[i]),
        std::get<2>(bingding_data[i])};
  }

  vk::DescriptorSetLayoutCreateInfo descriptor_set_layout_create_info{{}, bindings};

  descriptor_set_layout_ =
      vk::raii::DescriptorSetLayout{device_, descriptor_set_layout_create_info};
}

void Rendering::MakeDescriptorPool() {
  std::vector<vk::DescriptorPoolSize> pool_sizes{{vk::DescriptorType::eUniformBuffer, 1}};
  assert(!pool_sizes.empty());
  uint32_t max_sets{std::accumulate(
      pool_sizes.begin(), pool_sizes.end(), static_cast<uint32_t>(0),
      [](uint32_t sum, const vk::DescriptorPoolSize& dps) { return sum + dps.descriptorCount; })};
  assert(0 < max_sets);

  vk::DescriptorPoolCreateInfo descriptor_pool_create_info{
      vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet, max_sets, pool_sizes};
  descriptor_pool_ = vk::raii::DescriptorPool{device_, descriptor_pool_create_info};
}

void Rendering::MakeDescriptorSet() {
  descriptor_set_ = std::move(
      vk::raii::DescriptorSets{device_, {*descriptor_pool_, *descriptor_set_layout_}}.front());
}

void Rendering::MakePipelineLayout() {
  pipeline_layout_ = vk::raii::PipelineLayout{device_, {{}, *descriptor_set_layout_}};
}

void Rendering::MakePipelineCache() {
  pipeline_cache_ = vk::raii::PipelineCache{device_, {}};
}

void Rendering::MakePipeline() {
  std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_create_infos{
      {{}, vk::ShaderStageFlagBits::eVertex, *vertex_shader_module_, "main", nullptr},
      {{}, vk::ShaderStageFlagBits::eFragment, *fragment_shader_module_, "main", nullptr}};

  vk::PipelineVertexInputStateCreateInfo vertex_input_state_create_info;

  uint32_t vertex_stride{static_cast<uint32_t>(sizeof(kColoredCubeData[0]))};
  vk::VertexInputBindingDescription vertex_input_binding_description{0, vertex_stride};
  std::vector<vk::VertexInputAttributeDescription> vertex_input_attribute_descriptions;
  if (vertex_stride > 0) {
    std::vector<std::pair<vk::Format, uint32_t>> vertex_input_attribute_format_offset{
        {vk::Format::eR32G32B32A32Sfloat, 0}, {vk::Format::eR32G32B32A32Sfloat, 16}};
    vertex_input_attribute_descriptions.reserve(vertex_input_attribute_format_offset.size());
    for (uint32_t i = 0; i < vertex_input_attribute_format_offset.size(); i++) {
      vertex_input_attribute_descriptions.emplace_back(
          i, 0, vertex_input_attribute_format_offset[i].first,
          vertex_input_attribute_format_offset[i].second);
    }
    vertex_input_state_create_info.setVertexBindingDescriptions(vertex_input_binding_description);
    vertex_input_state_create_info.setVertexAttributeDescriptions(
        vertex_input_attribute_descriptions);
  }

  vk::PipelineInputAssemblyStateCreateInfo InputAssemblyStateCreateInfo{
      {}, vk::PrimitiveTopology::eTriangleList};

  vk::PipelineViewportStateCreateInfo viewport_state_create_info{{}, 1, nullptr, 1, nullptr};

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

  vk::PipelineMultisampleStateCreateInfo multisample_state_create_info{{},
                                                                       vk::SampleCountFlagBits::e1};

  vk::StencilOpState stencil_op_state{vk::StencilOp::eKeep, vk::StencilOp::eKeep,
                                      vk::StencilOp::eKeep, vk::CompareOp::eAlways};
  vk::PipelineDepthStencilStateCreateInfo depth_stencil_state_create_info{
      {},       VK_TRUE,  VK_TRUE,          vk::CompareOp::eLessOrEqual,
      VK_FALSE, VK_FALSE, stencil_op_state, stencil_op_state};

  vk::ColorComponentFlags color_component_flags{
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
      vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};
  vk::PipelineColorBlendAttachmentState color_blend_attachment_state{
      VK_FALSE,          vk::BlendFactor::eZero, vk::BlendFactor::eZero,
      vk::BlendOp::eAdd, vk::BlendFactor::eZero, vk::BlendFactor::eZero,
      vk::BlendOp::eAdd, color_component_flags};

  vk::PipelineColorBlendStateCreateInfo color_blend_state_create_info{
      {}, VK_FALSE, vk::LogicOp::eNoOp, color_blend_attachment_state, {{1.0f, 1.0f, 1.0f, 1.0f}}};

  std::array<vk::DynamicState, 2> dynamic_states{vk::DynamicState::eViewport,
                                                 vk::DynamicState::eScissor};
  vk::PipelineDynamicStateCreateInfo dynamic_state_create_info{{}, dynamic_states};

  vk::GraphicsPipelineCreateInfo graphics_pipeline_create_info{{},
                                                               shader_stage_create_infos,
                                                               &vertex_input_state_create_info,
                                                               &InputAssemblyStateCreateInfo,
                                                               nullptr,
                                                               &viewport_state_create_info,
                                                               &rasterization_state_create_info,
                                                               &multisample_state_create_info,
                                                               &depth_stencil_state_create_info,
                                                               &color_blend_state_create_info,
                                                               &dynamic_state_create_info,
                                                               *pipeline_layout_,
                                                               *render_pass_};

  graphics_pipeline_ = vk::raii::Pipeline{device_, pipeline_cache_, graphics_pipeline_create_info};
}

void Rendering::MakeVertexBuffer() {
  vertex_buffer_data_.buffer = vk::raii::Buffer{
      device_,
      vk::BufferCreateInfo{{}, sizeof(kColoredCubeData), vk::BufferUsageFlagBits::eVertexBuffer}};

  vertex_buffer_data_.device_memory = AllocateDeviceMemory(
      vertex_buffer_data_.buffer.getMemoryRequirements(),
      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

  vertex_buffer_data_.buffer.bindMemory(*(vertex_buffer_data_.device_memory), 0);

  CopyToDevice(vertex_buffer_data_.device_memory, kColoredCubeData,
               sizeof(kColoredCubeData) / sizeof(kColoredCubeData[0]));
}

void Rendering::MakeUniformBuffer() {
  float fov{glm::radians(45.0f)};

  if (kWidth > kHeight) {
    fov *= static_cast<float>(kHeight) /
           static_cast<float>(kWidth);
  }

  glm::mat4x4 model{glm::mat4x4{1.0f}};
  glm::mat4x4 view{glm::lookAt(glm::vec3{-5.0f, 3.0f, -10.0f}, glm::vec3{0.0f, 0.0f, 0.0f},
                               glm::vec3{0.0f, -1.0f, 0.0f})};
  glm::mat4x4 projection{glm::perspective(fov, 1.0f, 0.1f, 100.0f)};
  glm::mat4x4 clip{glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f,   // row 0
                               0.0f, -1.0f, 0.0f, 0.0f,  // row 1
                               0.0f, 0.0f, 0.5f, 0.0f,   // row 2
                               0.0f, 0.0f, 0.5f, 1.0f    // row 3
                               )};
  glm::mat4x4 mvpc_matrix{clip * projection * view * model};

  uniform_buffer_data_.buffer = vk::raii::Buffer{
      device_,
      vk::BufferCreateInfo{{}, sizeof(mvpc_matrix), vk::BufferUsageFlagBits::eUniformBuffer}};

  uniform_buffer_data_.device_memory = AllocateDeviceMemory(
      uniform_buffer_data_.buffer.getMemoryRequirements(),
      vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

  uniform_buffer_data_.buffer.bindMemory(*(uniform_buffer_data_.device_memory), 0);

  CopyToDevice(uniform_buffer_data_.device_memory, &mvpc_matrix);
}

void Rendering::UpdateDescriptorSets() {
  std::vector<std::tuple<vk::DescriptorType, vk::raii::Buffer const&, vk::DeviceSize,
                         vk::raii::BufferView const*>>
      buffer_data{{vk::DescriptorType::eUniformBuffer, uniform_buffer_data_.buffer, VK_WHOLE_SIZE,
                   nullptr}};

  std::vector<vk::DescriptorBufferInfo> buffer_infos;
  buffer_infos.reserve(buffer_data.size());

  std::vector<vk::WriteDescriptorSet> write_descriptor_sets;
  write_descriptor_sets.reserve(buffer_data.size());
  uint32_t dst_binding{0};

  for (auto const& bd : buffer_data) {
    buffer_infos.emplace_back(*std::get<1>(bd), 0, std::get<2>(bd));
    vk::BufferView buffer_view;
    if (std::get<3>(bd)) {
      buffer_view = **std::get<3>(bd);
    }
    write_descriptor_sets.emplace_back(*descriptor_set_, dst_binding++, 0, 1, std::get<0>(bd),
                                       nullptr, &buffer_infos.back(),
                                       std::get<3>(bd) ? &buffer_view : nullptr);
  }
  device_.updateDescriptorSets(write_descriptor_sets, nullptr);
}

// ------------------------------
vk::SurfaceFormatKHR Rendering::PickSurfaceFormat() {
  std::vector<vk::SurfaceFormatKHR> formats{
      physical_device_.getSurfaceFormatsKHR(*(surface_data_.surface))};

  assert(!formats.empty());
  vk::SurfaceFormatKHR picked_format{formats[0]};
  if (formats.size() == 1) {
    if (formats[0].format == vk::Format::eUndefined) {
      picked_format.format = vk::Format::eB8G8R8A8Unorm;
      picked_format.colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear;
    }
  } else {
    // request several formats, the first found will be used
    vk::Format requestedFormats[]{vk::Format::eB8G8R8A8Unorm, vk::Format::eR8G8B8A8Unorm,
                                  vk::Format::eB8G8R8Unorm, vk::Format::eR8G8B8Unorm};
    vk::ColorSpaceKHR requestedColorSpace{vk::ColorSpaceKHR::eSrgbNonlinear};
    for (size_t i = 0; i < sizeof(requestedFormats) / sizeof(requestedFormats[0]); i++) {
      vk::Format requestedFormat{requestedFormats[i]};
      auto it{std::find_if(formats.begin(), formats.end(),
                           [requestedFormat, requestedColorSpace](vk::SurfaceFormatKHR const& f) {
                             return (f.format == requestedFormat) &&
                                    (f.colorSpace == requestedColorSpace);
                           })};
      if (it != formats.end()) {
        picked_format = *it;
        break;
      }
    }
  }
  assert(picked_format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear);

  return picked_format;
}

vk::raii::DeviceMemory Rendering::AllocateDeviceMemory(
    const vk::MemoryRequirements& memory_requirements,
    vk::MemoryPropertyFlags memory_properties_flags) {
  vk::PhysicalDeviceMemoryProperties memory_properties{physical_device_.getMemoryProperties()};

  uint32_t memory_type_bits{memory_requirements.memoryTypeBits};

  uint32_t type_index{static_cast<uint32_t>(~0)};
  for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++) {
    if ((memory_type_bits & 1) && ((memory_properties.memoryTypes[i].propertyFlags &
                                    memory_properties_flags) == memory_properties_flags)) {
      type_index = i;
      break;
    }
    memory_type_bits >>= 1;
  }
  assert(type_index != static_cast<uint32_t>(~0));

  vk::MemoryAllocateInfo memory_allocate_info{memory_requirements.size, type_index};

  return vk::raii::DeviceMemory{device_, memory_allocate_info};
}

vk::raii::ShaderModule Rendering::MakeShaderModule(vk::ShaderStageFlagBits shader_stage,
                                                   const std::string& glsl_shader) {
  EShLanguage stage;

  switch (shader_stage) {
    case vk::ShaderStageFlagBits::eVertex:
      stage = EShLangVertex;
      break;
    case vk::ShaderStageFlagBits::eTessellationControl:
      stage = EShLangTessControl;
      break;
    case vk::ShaderStageFlagBits::eTessellationEvaluation:
      stage = EShLangTessEvaluation;
      break;
    case vk::ShaderStageFlagBits::eGeometry:
      stage = EShLangGeometry;
      break;
    case vk::ShaderStageFlagBits::eFragment:
      stage = EShLangFragment;
      break;
    case vk::ShaderStageFlagBits::eCompute:
      stage = EShLangCompute;
      break;
    case vk::ShaderStageFlagBits::eRaygenNV:
      stage = EShLangRayGenNV;
      break;
    case vk::ShaderStageFlagBits::eAnyHitNV:
      stage = EShLangAnyHitNV;
      break;
    case vk::ShaderStageFlagBits::eClosestHitNV:
      stage = EShLangClosestHitNV;
      break;
    case vk::ShaderStageFlagBits::eMissNV:
      stage = EShLangMissNV;
      break;
    case vk::ShaderStageFlagBits::eIntersectionNV:
      stage = EShLangIntersectNV;
      break;
    case vk::ShaderStageFlagBits::eCallableNV:
      stage = EShLangCallableNV;
      break;
    case vk::ShaderStageFlagBits::eTaskNV:
      stage = EShLangTaskNV;
      break;
    case vk::ShaderStageFlagBits::eMeshNV:
      stage = EShLangMeshNV;
      break;
    default:
      assert(false && "Unknown shader stage");
      stage = EShLangVertex;
      break;
  }

  const char* shader_strings[1];
  shader_strings[0] = glsl_shader.data();

  glslang::TShader shader(stage);
  shader.setStrings(shader_strings, 1);

  EShMessages messages{EShMsgSpvRules | EShMsgVulkanRules};

  if (!shader.parse(&kDefaultTBuiltInResource, 100, false, messages)) {
    puts(shader.getInfoLog());
    puts(shader.getInfoDebugLog());
    throw std::runtime_error{"fail to parse shader"};
  }

  glslang::TProgram program;
  program.addShader(&shader);

  if (!program.link(messages)) {
    puts(shader.getInfoLog());
    puts(shader.getInfoDebugLog());
    fflush(stdout);
    throw std::runtime_error{"fail to link program"};
  }

  std::vector<unsigned> spv_shader;
  glslang::GlslangToSpv(*program.getIntermediate(stage), spv_shader);

  return vk::raii::ShaderModule{device_, vk::ShaderModuleCreateInfo{{}, spv_shader}};
}

void Rendering::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
  auto rendering{reinterpret_cast<Rendering*>(glfwGetWindowUserPointer(window))};
  rendering->framebuffer_resized_ = true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL Rendering::DebugUtilsMessengerCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    VkDebugUtilsMessengerCallbackDataEXT const* pCallbackData, void* /*pUserData*/) {
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
                   static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(message_severity))
            << ": " << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(message_types))
            << ":\n";
  std::cout << std::string("\t") << "message id name   = <" << pCallbackData->pMessageIdName
            << ">\n";
  std::cout << std::string("\t") << "message id number = " << pCallbackData->messageIdNumber
            << "\n";
  std::cout << std::string("\t") << "message         = <" << pCallbackData->pMessage << ">\n";
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
                << vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType))
                << "\n";
      std::cout << std::string("\t\t\t")
                << "object handle = " << pCallbackData->pObjects[i].objectHandle << "\n";
      if (pCallbackData->pObjects[i].pObjectName) {
        std::cout << std::string("\t\t\t") << "object name   = <"
                  << pCallbackData->pObjects[i].pObjectName << ">\n";
      }
    }
  }
  return VK_TRUE;
}

}  // namespace luka
