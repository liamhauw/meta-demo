#pragma once

#include <memory>
#include <string>

#include "resource/config/config.h"
#include "resource/asset/asset.h"

namespace luka {

class World {
 public:
  World(std::shared_ptr<Config> config, std::shared_ptr<Asset> asset);

  void Tick(double delta_time);

 private:
  void LoadWorldAsset();

  std::shared_ptr<Config> config_;
  std::shared_ptr<Asset> asset_;

  bool is_world_loaded_{false};
  std::string world_file_path_;
};

}  // namespace luka