#include "function/world/world.h"
#include "resource/res_type/common/world.h"



namespace luka {

World::World(const std::shared_ptr<Config>& config) {
  world_file_ = config->GetWorldFile().generic_string();
}

void World::Tick(double delta_time) {
  if (!is_world_loaded_) {
    LoadWorld();
  }
}

bool World::LoadWorld() {
  WorldRes world_res;


  return false;
}

}  // namespace luka