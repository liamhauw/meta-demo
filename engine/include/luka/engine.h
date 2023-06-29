#pragma once

#include <memory>

#include "luka/function/input/input.h"
#include "luka/function/rendering/rendering.h"
#include "luka/function/window/window.h"
#include "luka/function/world/world.h"
#include "luka/resource/asset/asset.h"
#include "luka/resource/config/config.h"

namespace luka {

class Engine {
 public:
  Engine();
  ~Engine();

  void Run();

 private:
  std::shared_ptr<Config> config_;
  std::shared_ptr<Asset> asset_;

  std::shared_ptr<World> world_;
  std::shared_ptr<Window> window_;
  std::shared_ptr<Input> input_;
  std::shared_ptr<Rendering> rendering_;
};

}  // namespace luka