#pragma once

#include <memory>

namespace luka {

class Window;
class Input;

struct FunctionContext {
  void Initialize();

  std::shared_ptr<Window> window;
  std::shared_ptr<Input> input;

};

extern FunctionContext g_function_context;

}  // namespace luka