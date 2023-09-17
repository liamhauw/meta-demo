/*
  SPDX license identifier: MIT

  Copyright (C) 2023 Liam Hauw.

  Config header file.
*/

#pragma once

#include <filesystem>

#include "resource/config/generated/source_path.h"

namespace luka {

class Config {
 public:
  Config();

 const std::filesystem::path& GetWorldFile() const;

 private:
  std::filesystem::path source_path_{LUKA_SOURCE_PATH};
  std::filesystem::path resource_path_{source_path_ / "resource"};
  std::filesystem::path asset_path_{resource_path_ / "asset"};
  std::filesystem::path config_file_path_{resource_path_ / "config/config.ini"};

  std::filesystem::path world_file_path_;
};

}  // namespace luka