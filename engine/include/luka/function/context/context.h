#pragma once

#include <memory>

namespace luka {

class Config;
class Asset;
class World;
class Window;
class Input;
class Rendering;

struct FunctionContext {
  void Initialize();
  void Terminate();

  [[nodiscard]] bool Tick(double delta_time) const;

  std::shared_ptr<Config> config;
  std::shared_ptr<Asset> asset;
  std::shared_ptr<World> world;
  std::shared_ptr<Window> window;
  std::shared_ptr<Input> input;

  Rendering* rendering;
};

extern FunctionContext g_function_context;

}  // namespace luka