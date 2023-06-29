#include "luka/engine.h"

#include "luka/core/log.h"
#include "luka/core/time.h"

namespace luka {

Engine::Engine() {
  LOGI("engine initialize");

  config_ = std::make_shared<Config>();
  asset_ = std::make_shared<Asset>();

  world_ = std::make_shared<World>(config_);
  window_ = std::make_shared<Window>();
  input_ = std::make_shared<Input>(window_);
  rendering_ = std::make_shared<Rendering>(window_);
}

void Engine::Run() {
  LOGI("engine run");

  Time time;
  double delta_time;
  while (true) {
    delta_time = time.Tick().GetDeltaTime();
    if (!window_->Tick(delta_time)) {
      break;
    }
  }
}

Engine::~Engine() { LOGI("engine terminate"); }

}  // namespace luka
