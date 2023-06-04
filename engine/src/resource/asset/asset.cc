#include "luka/resource/asset/asset.h"

#include <fstream>

#include "luka/function/context/context.h"
#include "luka/resource/config/config.h"

namespace luka {

std::vector<char> Asset::ReadFile(const std::string &filename) {
  std::shared_ptr<Config> config{g_function_context.config};

  std::ifstream file{config->shader_path().generic_string() + filename,
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