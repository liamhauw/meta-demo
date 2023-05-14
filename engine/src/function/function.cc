#include "luka/function/function.h"

namespace luka {
Function::Function() { rendering_ = std::make_shared<Rendering>(); }

void Function::Tick() { rendering_->Tick(); }

bool Function::ShouldClose() { return rendering_->ShouldClose(); }

}  // namespace luka
