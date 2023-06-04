#pragma once

#include <memory>

namespace luka {

class Config;
class Asset;
class Window;
class Input;
class Rendering;

struct FunctionContext {
  void Initialize();
  void Terminate();

  std::shared_ptr<Config> config;
  std::shared_ptr<Asset> asset;
  std::shared_ptr<Window> window;
  std::shared_ptr<Input> input;
  // if shared_ptr<Rendering>
  // std::__1::system_error: mutex lock failed: Invalid argument (in vkDestory...)
  // solution: delete manually
  Rendering* rendering;
};

extern FunctionContext g_function_context;

}  // namespace luka