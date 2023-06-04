#include "luka/engine.h"

#include "luka/core/log.h"
#include "luka/function/context/context.h"
#include "luka/function/window/window.h"

namespace luka {

void Engine::Run() {
  LOGI("engine initialize");
  g_function_context.Initialize();

  LOGI("engine run");
  std::shared_ptr<Window> window{g_function_context.window};
  while (!window->ShouldClose()) {
    window->PollEvent();
  }

  g_function_context.Terminate();
  LOGI("engine terminate");
}

}  // namespace luka
