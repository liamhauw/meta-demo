#pragma once

#include <filesystem>

#include "luka/resource/config/generated/source_path.h"

namespace luka {

class Config {
 public:
  Config();

  std::filesystem::path model_path();
  std::filesystem::path shader_path();
  std::filesystem::path texture_path();

 private:
  std::filesystem::path source_path_{LUKA_SOURCE_PATH};
  std::filesystem::path resource_path_{source_path_ / "resource"};
  std::filesystem::path asset_path_{resource_path_ / "asset"};
  std::filesystem::path config_file_{resource_path_ / "config/config.ini"};

  std::filesystem::path model_path_;
  std::filesystem::path shader_path_;
  std::filesystem::path texture_path_;
};

}  // namespace luka