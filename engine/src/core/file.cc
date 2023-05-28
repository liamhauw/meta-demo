#include "luka/core/file.h"

#include <fstream>

#include "luka/core/generated/config.h"

namespace luka {

std::string root_directory{LUKA_ROOT_DIRECTORY};

std::vector<char> ReadFile(const std::string& filename) {
  std::ifstream file{root_directory + filename,
                     std::ios::ate | std::ios::binary};
  if (!file.is_open()) {
    throw std::runtime_error{"fail to open " + filename};
  }

  uint32_t file_size{static_cast<uint32_t>(file.tellg())};
  std::vector<char> buffer(file_size);

  file.seekg(0);
  file.read(buffer.data(), file_size);

  file.close();

  return buffer;
}

}  // namespace luka