#pragma once

namespace luka {

enum class GameCommand : unsigned {
  kForward = 1 << 0,                         // W
  kBackward = 1 << 1,                        // S
  kLeft = 1 << 2,                            // A
  kRight = 1 << 3,                           // D
  kJump = 1 << 4,                            // SPACE
  kSquat = 1 << 5,                           // LEFT CONTROL
  kSprint = 1 << 6,                          // LEFT SHIFT
  kInvalid = static_cast<unsigned>(1 << 31)  // lost focus
};

extern unsigned g_control_command;

class Input {
 public:
  Input();

  void OnKey(int key, int scancode, int action, int mod);
  void OnCursorPos(double xpos, double ypos);

 private:
  static void OnKeyInEditorMode(int key, int scancode, int action, int mod);
  void OnKeyInFuncMode(int key, int scancode, int action, int mod);

  unsigned game_command_{0};

  double cursor_last_xpos{0};
  double cursor_last_ypos{0};

  double cursor_delta_xpos{0};
  double cursor_delta_ypos{0};
};

}  // namespace luka