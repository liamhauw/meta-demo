#include "luka/function/context/context.h"

#include "luka/function/window/input.h"
#include "luka/function/window/window.h"

namespace luka {

FunctionContext g_function_context;

void FunctionContext::Initialize() {
  window = std::make_shared<Window>();
  input = std::make_shared<Input>();
}

}  // namespace luka
