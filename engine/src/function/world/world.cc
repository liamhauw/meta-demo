#include "luka/function/world/world.h"

namespace luka {

World::World(const std::shared_ptr<Config>& config) { world_file_ = config->GetWorldFile(); }

}  // namespace luka