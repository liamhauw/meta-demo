#include "luka/engine.h"

#include <memory>

namespace luka {

Engine::Engine() {
  platform_ = std::make_shared<Platform>();
  core_ = std::make_shared<Core>();
  resource_ = std::make_shared<Resource>();
  function_ = std::make_shared<Function>();
  editor_ = std::make_shared<Editor>();
}

void Engine::Run() {
  while (!function_->ShouldClose()) {
    core_->Tick();
    function_->Tick();
  }
}

}  // namespace luka
