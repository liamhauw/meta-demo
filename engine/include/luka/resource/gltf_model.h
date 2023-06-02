#pragma once

#include <string>
#include <vulkan/vulkan_raii.hpp>

namespace luka {

enum FileLoadFlag {
  None = 0x00000000,
  PreTransformVertex = 0x00000001,
  PreMultiplyVertexColor = 0x00000002,
  FlipY = 0x00000004,
  DontLoadImage = 0x00000008
};

class GltfModel {
 public:
  void LoadFromFile(const std::string& file_name, const vk::raii::Device& device,
                    const vk::raii::Queue& queue, uint32_t file_load_flags = FileLoadFlag::None,
                    float scale = 1.0f);

 private:
  //  void LoadImage();
  //  void LoadMaterial();
  //  void LoadNode();
  //  void LoadAnimation();
  //  void LoadSkin();

 private:
  std::string path_;
};
}  // namespace luka