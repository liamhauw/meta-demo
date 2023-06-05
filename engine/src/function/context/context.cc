#include "luka/function/context/context.h"

#include "luka/function/input/input.h"
#include "luka/function/rendering/rendering.h"
#include "luka/function/window/window.h"
#include "luka/function/world/world.h"
#include "luka/resource/asset/asset.h"
#include "luka/resource/config/config.h"

namespace luka {

FunctionContext g_function_context;

void FunctionContext::Initialize() {
  config = std::make_shared<Config>();
  asset = std::make_shared<Asset>();
  world = std::make_shared<World>();
  window = std::make_shared<Window>();
  input = std::make_shared<Input>();
  rendering = new Rendering();
}

bool FunctionContext::Tick(double delta_time) const {


  return window->Tick(delta_time);
}

void FunctionContext::Terminate() {
  delete rendering;
  rendering = nullptr;
}

}  // namespace luka
