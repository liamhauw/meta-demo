#include "luka/resource/resource_path.h"

#include "luka/core/file.h"

namespace luka {
std::string GetResourcePath(const std::string& path) {
  return {root_directory + "/resource/" + path};
}
}  // namespace luka