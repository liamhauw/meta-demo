#pragma once

#include <stb_image.h>

#include <string>
#include <vector>

#include "luka/core/core.h"

namespace luka {

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 tex_coord;

  bool operator==(const Vertex& other) const {
    return pos == other.pos && color == other.color &&
           tex_coord == other.tex_coord;
  }
};

struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

void LoadModel(const std::string& model, std::vector<Vertex>& vertices,
               std::vector<uint32_t>& indices);

class Resource {};

}  // namespace luka

namespace std {
template <>
struct hash<luka::Vertex> {
  size_t operator()(const luka::Vertex& vertex) const {
    return ((hash<glm::vec3>()(vertex.pos) ^
             (hash<glm::vec3>()(vertex.color) << 1)) >>
            1) ^
           (hash<glm::vec2>()(vertex.tex_coord) << 1);
  }
};
}  // namespace std