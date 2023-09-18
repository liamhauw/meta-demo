/*
  Copyright (C) 2023-present Liam Hauw.

  SPDX license identifier: MIT.   

  Engine source file.
*/

#include "engine.h"

#include "core/util/log.h"
#include "core/util/time.h"

namespace luka {

Engine::Engine() {
  LOGI("Engine initialize");

  config_ = std::make_shared<Config>();
  asset_ = std::make_shared<Asset>();
  world_ = std::make_shared<World>(config_, asset_);
  window_ = std::make_shared<Window>();
  input_ = std::make_shared<Input>(window_);
  rendering_ = std::make_shared<Rendering>(window_);
}

void Engine::Run() {
  LOGI("Engine run");

  Time time;
  double delta_time{0.0};
  while (true) {
    delta_time = time.Tick().GetDeltaTime();
    world_->Tick(delta_time);
    if (!window_->Tick(delta_time)) {
      break;
    }
  }
}

Engine::~Engine() { LOGI("Engine terminate"); }

}  // namespace luka
