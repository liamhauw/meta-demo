#include "luka/function/input/input.h"

#include <memory>

#include "luka/core/context.h"
#include "luka/core/log.h"
#include "luka/function/context/context.h"
#include "luka/function/window/window.h"

namespace luka {

unsigned g_control_command{0xFFFFFFFF};

Input::Input() {
  std::shared_ptr<Window> window{g_function_context.window};

  window->RegisterOnKeyFunc([this](auto&& ph1, auto&& ph2, auto&& ph3, auto&& ph4) {
    OnKey(std::forward<decltype(ph1)>(ph1), std::forward<decltype(ph2)>(ph2),
          std::forward<decltype(ph3)>(ph3), std::forward<decltype(ph4)>(ph4));
  });

  window->RegisterOnCursorPosFunc([this](auto&& ph1, auto&& ph2) {
    OnCursorPos(std::forward<decltype(ph1)>(ph1), std::forward<decltype(ph2)>(ph2));
  });
}

void Input::OnKey(int key, int scancode, int action, int mod) {
  if (g_is_editor_mode) {
    OnKeyInEditorMode(key, scancode, action, mod);
  } else {
    OnKeyInFuncMode(key, scancode, action, mod);
  }
}
void Input::OnCursorPos(double xpos, double ypos) {
  if (g_function_context.window->FocusMode()) {
    cursor_delta_xpos = cursor_last_xpos - xpos;
    cursor_delta_ypos = cursor_last_ypos - ypos;
  }
  cursor_last_xpos = xpos;
  cursor_last_ypos = ypos;
}
void Input::OnKeyInEditorMode(int key, int scancode, int action, int mod) {
  switch (key) {
    case GLFW_KEY_ESCAPE:
      g_function_context.window->SetShouldClose();
      break;
    case GLFW_KEY_F:
      g_is_editor_mode = false;
    default:
      break;
  }
}
void Input::OnKeyInFuncMode(int key, int /*scancode*/, int action, int /*mod*/) {
  game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::kJump));
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_E:
        g_is_editor_mode = true;
      case GLFW_KEY_LEFT_ALT: {
        std::shared_ptr<Window> window{g_function_context.window};
        window->SetFocusMode(!window->FocusMode());
      } break;
      case GLFW_KEY_W:
        game_command_ |= static_cast<unsigned>(GameCommand::kForward);
        break;
      case GLFW_KEY_S:
        game_command_ |= static_cast<unsigned>(GameCommand::kBackward);
        break;
      case GLFW_KEY_A:
        game_command_ |= static_cast<unsigned>(GameCommand::kLeft);
        break;
      case GLFW_KEY_D:
        game_command_ |= static_cast<unsigned>(GameCommand::kRight);
        break;
      case GLFW_KEY_SPACE:
        game_command_ |= static_cast<unsigned>(GameCommand::kJump);
        break;
      case GLFW_KEY_LEFT_CONTROL:
        game_command_ |= static_cast<unsigned>(GameCommand::kSquat);
        break;
      case GLFW_KEY_LEFT_SHIFT:
        game_command_ |= static_cast<unsigned>(GameCommand::kSprint);
        break;
      default:
        break;
    }
  } else if (action == GLFW_RELEASE) {
    switch (key) {
      case GLFW_KEY_W:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::kForward));
        break;
      case GLFW_KEY_S:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::kBackward));
        break;
      case GLFW_KEY_A:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::kLeft));
        break;
      case GLFW_KEY_D:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::kRight));
        break;
      case GLFW_KEY_LEFT_CONTROL:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::kSquat));
        break;
      case GLFW_KEY_LEFT_SHIFT:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::kSprint));
        break;
      default:
        break;
    }
  }
}

}  // namespace luka
