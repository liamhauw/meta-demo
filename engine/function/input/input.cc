#include "function/input/input.h"

#include "core/context/context.h"

namespace luka {

unsigned g_control_command{0xFFFFFFFF};

Input::Input(const std::shared_ptr<Window>& window) : window_{window} {
  window_->RegisterOnKeyFunc([this](auto&& ph1, auto&& ph2, auto&& ph3, auto&& ph4) {
    OnKey(std::forward<decltype(ph1)>(ph1), std::forward<decltype(ph2)>(ph2),
          std::forward<decltype(ph3)>(ph3), std::forward<decltype(ph4)>(ph4));
  });

  window_->RegisterOnCursorPosFunc([this](auto&& ph1, auto&& ph2) {
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
  if (window_->FocusMode()) {
    cursor_delta_xpos_ = cursor_last_xpos_ - xpos;
    cursor_delta_ypos_ = cursor_last_ypos_ - ypos;
  }
  cursor_last_xpos_ = xpos;
  cursor_last_ypos_ = ypos;
}
void Input::OnKeyInEditorMode(int key, int scancode, int action, int mod) {
  switch (key) {
    case GLFW_KEY_ESCAPE:
      window_->SetShouldClose();
      break;
    case GLFW_KEY_F:
      g_is_editor_mode = false;
    default:
      break;
  }
}
void Input::OnKeyInFuncMode(int key, int /*scancode*/, int action, int /*mod*/) {
  game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::JUMP));
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_E:
        g_is_editor_mode = true;
      case GLFW_KEY_LEFT_ALT: {
        window_->SetFocusMode(!window_->FocusMode());
      } break;
      case GLFW_KEY_W:
        game_command_ |= static_cast<unsigned>(GameCommand::FORWARD);
        break;
      case GLFW_KEY_S:
        game_command_ |= static_cast<unsigned>(GameCommand::BACKWARD);
        break;
      case GLFW_KEY_A:
        game_command_ |= static_cast<unsigned>(GameCommand::LEFT);
        break;
      case GLFW_KEY_D:
        game_command_ |= static_cast<unsigned>(GameCommand::RIGHT);
        break;
      case GLFW_KEY_SPACE:
        game_command_ |= static_cast<unsigned>(GameCommand::JUMP);
        break;
      case GLFW_KEY_LEFT_CONTROL:
        game_command_ |= static_cast<unsigned>(GameCommand::SQUAT);
        break;
      case GLFW_KEY_LEFT_SHIFT:
        game_command_ |= static_cast<unsigned>(GameCommand::SPRINT);
        break;
      default:
        break;
    }
  } else if (action == GLFW_RELEASE) {
    switch (key) {
      case GLFW_KEY_W:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::FORWARD));
        break;
      case GLFW_KEY_S:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::BACKWARD));
        break;
      case GLFW_KEY_A:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::LEFT));
        break;
      case GLFW_KEY_D:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::RIGHT));
        break;
      case GLFW_KEY_LEFT_CONTROL:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::SQUAT));
        break;
      case GLFW_KEY_LEFT_SHIFT:
        game_command_ &= (g_control_command ^ static_cast<unsigned>(GameCommand::SPRINT));
        break;
      default:
        break;
    }
  }
}

}  // namespace luka
