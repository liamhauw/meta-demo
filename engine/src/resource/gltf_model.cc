#include "luka/resource/gltf_model.h"

#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <stdexcept>

#include "tiny_gltf.h"

namespace luka {

void GltfModel::LoadFromFile(const std::string& file_name, const vk::raii::Device& device,
                             const vk::raii::Queue& queue, uint32_t file_load_flag, float scale) {
  tinygltf::TinyGLTF tiny_gltf;
  if (file_load_flag & FileLoadFlag::DontLoadImage) {
    tiny_gltf.SetImageLoader([](tinygltf::Image*, const int, std::string*, std::string*, int, int,
                                const unsigned char*, int, void*) { return true; },
                             nullptr);
  } else {
    tiny_gltf.SetImageLoader(
        [](tinygltf::Image* image, const int image_idx, std::string* err, std::string* warn,
           int req_width, int req_height, const unsigned char* bytes, int size,
           void* user_data) -> bool {
          if (image->uri.find_last_of('.') != std::string::npos) {
            if (image->uri.substr(image->uri.find_last_of('.') + 1) == "ktx") {
              return true;
            }
          }
          return tinygltf::LoadImageData(image, image_idx, err, warn, req_width, req_height, bytes,
                                         size, user_data);
        },
        nullptr);
  }

  size_t pos{file_name.find_last_of('/')};
  path_ = file_name.substr(0, pos);

  tinygltf::Model model;
  std::string err, warn;
  if (!tiny_gltf.LoadASCIIFromFile(&model, &err, &warn, file_name)) {
    throw std::runtime_error{std::string("fail to load ") + file_name + "\n" + err};
  }

  //  // image
  //  if (!(file_load_flag & FileLoadFlag::DontLoadImage)) {
  //    LoadImage();
  //  }
  //
  //  // material
  //  LoadMaterial();
  //
  //  // node
  //  const tinygltf::Scene& scene{model.scenes[model.defaultScene > -1 ? model.defaultScene : 0]};
  //  for (const auto& scene_node : scene.nodes) {
  //    const tinygltf::Node model_node{model.nodes[scene_node]};
  //    LoadNode();
  //  }
  //
  //  // animation
  //  if (!model.animations.empty()) {
  //    LoadAnimation();
  //  }
  //
  //  // skin
  //  LoadSkin();
}
}  // namespace luka