#pragma once

#include <memory>

#include "luka/function/window/window.h"

namespace luka {

enum class GameCommand : unsigned {
  FORWARD = 1 << 0,                         // W
  BACKWARD = 1 << 1,                        // S
  LEFT = 1 << 2,                            // A
  RIGHT = 1 << 3,                           // D
  JUMP = 1 << 4,                            // SPACE
  SQUAT = 1 << 5,                           // LEFT CONTROL
  SPRINT = 1 << 6,                          // LEFT SHIFT
  INVALID = static_cast<unsigned>(1 << 31)  // lost focus
};

extern unsigned g_control_command;

class Input {
 public:
  explicit Input(const std::shared_ptr<Window>& window);

  void OnKey(int key, int scancode, int action, int mod);
  void OnCursorPos(double xpos, double ypos);

 private:
  void OnKeyInEditorMode(int key, int scancode, int action, int mod);
  void OnKeyInFuncMode(int key, int scancode, int action, int mod);

  std::shared_ptr<Window> window_;

  unsigned game_command_{0};

  double cursor_last_xpos_{0.0};
  double cursor_last_ypos_{0.0};

  double cursor_delta_xpos_{0.0};
  double cursor_delta_ypos_{0.0};
};

}  // namespace luka