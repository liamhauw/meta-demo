#pragma once

#include <memory>
#include <string>

#include "resource/config/config.h"

namespace luka {

class World {
 public:
  explicit World(const std::shared_ptr<Config>& config);

  void Tick(double delta_time);

 private:
  bool LoadWorld();

  bool is_world_loaded_{false};
  std::string world_file_;
};

}  // namespace luka