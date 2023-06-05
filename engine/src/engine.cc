#include "luka/engine.h"

#include "luka/core/log.h"
#include "luka/core/time.h"
#include "luka/function/context/context.h"
#include "luka/function/window/window.h"

namespace luka {

void Engine::Run() {
  LOGI("engine initialize");
  g_function_context.Initialize();

  LOGI("engine run");

  Time time;
  double delta_time;
  while(true) {
    delta_time = time.Tick().GetDeltaTime();
    if(!g_function_context.Tick(delta_time)) {
      break;
    }
  }

  g_function_context.Terminate();
  LOGI("engine terminate");
}

}  // namespace luka
