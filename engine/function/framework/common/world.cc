#include "function/framework/common/world.h"

#include <utility>

#include "resource/res_type/common/world.h"

namespace luka {

World::World(std::shared_ptr<Config> config, std::shared_ptr<Asset> asset)
    : config_{std::move(config)}, asset_{std::move(asset)} {
  world_file_path_ = config_->GetWorldFile().generic_string();
}

void World::Tick(double delta_time) {
  if (!is_world_loaded_) {
    LoadWorldAsset();
  }
}

void World::LoadWorldAsset() {
  WorldRes world_res;
  asset_->LoadAsset(world_file_path_, world_res);

  is_world_loaded_ = true;
}

}  // namespace luka