#include "luka/function/world/world.h"

#include "luka/function/context/context.h"
#include "luka/resource/config/config.h"

namespace luka {

World::World() { world_file_ = g_function_context.config->GetWorldFile(); }

}  // namespace luka