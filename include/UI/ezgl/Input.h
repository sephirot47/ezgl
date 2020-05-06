#pragma once

#include "ezgl/Flags.h"
#include <GLFW/glfw3.h>
#include <variant>

namespace ez
{
enum class Key
{
  UNKNOWN = GLFW_KEY_UNKNOWN,
  SPACE = GLFW_KEY_SPACE,
  APOSTROPHE = GLFW_KEY_APOSTROPHE,
  COMMA = GLFW_KEY_COMMA,
  MINUS = GLFW_KEY_MINUS,
  PERIOD = GLFW_KEY_PERIOD,
  SLASH = GLFW_KEY_SLASH,
  NUM_0 = GLFW_KEY_0,
  NUM_1 = GLFW_KEY_1,
  NUM_2 = GLFW_KEY_2,
  NUM_3 = GLFW_KEY_3,
  NUM_4 = GLFW_KEY_4,
  NUM_5 = GLFW_KEY_5,
  NUM_6 = GLFW_KEY_6,
  NUM_7 = GLFW_KEY_7,
  NUM_8 = GLFW_KEY_8,
  NUM_9 = GLFW_KEY_9,
  SEMICOLON = GLFW_KEY_SEMICOLON,
  EQUAL = GLFW_KEY_EQUAL,
  A = GLFW_KEY_A,
  B = GLFW_KEY_B,
  C = GLFW_KEY_C,
  D = GLFW_KEY_D,
  E = GLFW_KEY_E,
  F = GLFW_KEY_F,
  G = GLFW_KEY_G,
  H = GLFW_KEY_H,
  I = GLFW_KEY_I,
  J = GLFW_KEY_J,
  K = GLFW_KEY_K,
  L = GLFW_KEY_L,
  M = GLFW_KEY_M,
  N = GLFW_KEY_N,
  O = GLFW_KEY_O,
  P = GLFW_KEY_P,
  Q = GLFW_KEY_Q,
  R = GLFW_KEY_R,
  S = GLFW_KEY_S,
  T = GLFW_KEY_T,
  U = GLFW_KEY_U,
  V = GLFW_KEY_V,
  W = GLFW_KEY_W,
  X = GLFW_KEY_X,
  Y = GLFW_KEY_Y,
  Z = GLFW_KEY_Z,
  LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
  BACKSLASH = GLFW_KEY_BACKSLASH,
  RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
  GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
  WORLD_1 = GLFW_KEY_WORLD_1,
  WORLD_2 = GLFW_KEY_WORLD_2,
  ESCAPE = GLFW_KEY_ESCAPE,
  ENTER = GLFW_KEY_ENTER,
  TAB = GLFW_KEY_TAB,
  BACKSPACE = GLFW_KEY_BACKSPACE,
  INSERT = GLFW_KEY_INSERT,
  DELETE = GLFW_KEY_DELETE,
  RIGHT = GLFW_KEY_RIGHT,
  LEFT = GLFW_KEY_LEFT,
  DOWN = GLFW_KEY_DOWN,
  UP = GLFW_KEY_UP,
  PAGE_UP = GLFW_KEY_PAGE_UP,
  PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
  HOME = GLFW_KEY_HOME,
  END = GLFW_KEY_END,
  CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
  SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
  NUM_LOCK = GLFW_KEY_NUM_LOCK,
  PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
  PAUSE = GLFW_KEY_PAUSE,
  F1 = GLFW_KEY_F1,
  F2 = GLFW_KEY_F2,
  F3 = GLFW_KEY_F3,
  F4 = GLFW_KEY_F4,
  F5 = GLFW_KEY_F5,
  F6 = GLFW_KEY_F6,
  F7 = GLFW_KEY_F7,
  F8 = GLFW_KEY_F8,
  F9 = GLFW_KEY_F9,
  F10 = GLFW_KEY_F10,
  F11 = GLFW_KEY_F11,
  F12 = GLFW_KEY_F12,
  F13 = GLFW_KEY_F13,
  F14 = GLFW_KEY_F14,
  F15 = GLFW_KEY_F15,
  F16 = GLFW_KEY_F16,
  F17 = GLFW_KEY_F17,
  F18 = GLFW_KEY_F18,
  F19 = GLFW_KEY_F19,
  F20 = GLFW_KEY_F20,
  F21 = GLFW_KEY_F21,
  F22 = GLFW_KEY_F22,
  F23 = GLFW_KEY_F23,
  F24 = GLFW_KEY_F24,
  F25 = GLFW_KEY_F25,
  KP_0 = GLFW_KEY_KP_0,
  KP_1 = GLFW_KEY_KP_1,
  KP_2 = GLFW_KEY_KP_2,
  KP_3 = GLFW_KEY_KP_3,
  KP_4 = GLFW_KEY_KP_4,
  KP_5 = GLFW_KEY_KP_5,
  KP_6 = GLFW_KEY_KP_6,
  KP_7 = GLFW_KEY_KP_7,
  KP_8 = GLFW_KEY_KP_8,
  KP_9 = GLFW_KEY_KP_9,
  KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
  KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
  KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
  KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
  KP_ADD = GLFW_KEY_KP_ADD,
  KP_ENTER = GLFW_KEY_KP_ENTER,
  KP_EQUAL = GLFW_KEY_KP_EQUAL,
  LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
  LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
  LEFT_ALT = GLFW_KEY_LEFT_ALT,
  LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
  RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
  RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
  RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
  RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
  MENU = GLFW_KEY_MENU,
  LAST = GLFW_KEY_LAST,
};

enum class KeyAction
{
  UNKNOWN = -1,
  PRESS = GLFW_PRESS,
  REPEAT = GLFW_REPEAT,
  RELEASE = GLFW_RELEASE,
};

enum class MouseAction
{
  UNKNOWN = -1,
  PRESS = GLFW_PRESS,
  RELEASE = GLFW_RELEASE,
};

enum class MouseButton
{
  UNKNOWN = -1,
  BUTTON_1 = GLFW_MOUSE_BUTTON_1,
  BUTTON_2 = GLFW_MOUSE_BUTTON_2,
  BUTTON_3 = GLFW_MOUSE_BUTTON_3,
  BUTTON_4 = GLFW_MOUSE_BUTTON_4,
  BUTTON_5 = GLFW_MOUSE_BUTTON_5,
  BUTTON_6 = GLFW_MOUSE_BUTTON_6,
  BUTTON_7 = GLFW_MOUSE_BUTTON_7,
  BUTTON_8 = GLFW_MOUSE_BUTTON_8,
  LAST = GLFW_MOUSE_BUTTON_LAST,
  LEFT = GLFW_MOUSE_BUTTON_LEFT,
  RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
  MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
};

enum class ModifierKey
{
  NONE = 0,
  SHIFT = GLFW_MOD_SHIFT,
  CONTROL = GLFW_MOD_CONTROL,
  ALT = GLFW_MOD_ALT,
  SUPER = GLFW_MOD_SUPER,
  CAPS_LOCK = GLFW_MOD_CAPS_LOCK,
  NUM_LOCK = GLFW_MOD_NUM_LOCK,
};
DECLARE_FLAGS(ModifierKey);

struct KeyOrButtonEvent
{
  ModifierKey mModifiers = ModifierKey::NONE;

  bool IsAltModifierPressed() const { return (mModifiers & ModifierKey::ALT) != ModifierKey::NONE; }
  bool IsShiftModifierPressed() const { return (mModifiers & ModifierKey::SHIFT) != ModifierKey::NONE; }
  bool IsControlModifierPressed() const { return (mModifiers & ModifierKey::CONTROL) != ModifierKey::NONE; }
  bool IsCapsLockModifierPressed() const { return (mModifiers & ModifierKey::CAPS_LOCK) != ModifierKey::NONE; }
  bool IsNumLockModifierPressed() const { return (mModifiers & ModifierKey::NUM_LOCK) != ModifierKey::NONE; }
  bool IsSuperModifierPressed() const { return (mModifiers & ModifierKey::SUPER) != ModifierKey::NONE; }
};

struct KeyEvent final : public KeyOrButtonEvent
{
  KeyAction mAction = KeyAction::UNKNOWN;
  Key mKey = Key::UNKNOWN;

  bool IsPress() const { return mAction == KeyAction::PRESS; }
  bool IsRepeat() const { return mAction == KeyAction::REPEAT; }
  bool IsPressOrRepeat() const { return IsPress() || IsRepeat(); }
  bool IsRelease() const { return mAction == KeyAction::RELEASE; }
};

struct MouseButtonEvent final : public KeyOrButtonEvent
{
  MouseAction mAction = MouseAction::UNKNOWN;
  MouseButton mButton = MouseButton::UNKNOWN;

  bool IsPress() const { return mAction == MouseAction::PRESS; }
  bool IsRelease() const { return mAction == MouseAction::RELEASE; }
};

struct MouseMoveEvent final
{
  Vec2f mPosition = Vec2f(0.0f, 0.0f);
};

struct MouseEnterExitEvent final
{
  bool mEntered = false; // If true mouse has entered. If false mouse has exited.
};

struct MouseScrollEvent final
{
  Vec2f mDeltaScroll = Vec2f(0.0f, 0.0f);
};

using VariantInputEvent
    = std::variant<KeyEvent, MouseButtonEvent, MouseMoveEvent, MouseEnterExitEvent, MouseScrollEvent>;
struct InputEvent final : public VariantInputEvent
{
  using VariantInputEvent::operator=;

  enum class Type // Must be in the same order as the variant template types
  {
    Key,
    MouseButton,
    MouseMove,
    MouseEnterExit,
    MouseScroll
  };

  InputEvent::Type GetType() const { return static_cast<Type>(index()); }

  template <InputEvent::Type InputEventType>
  auto& As()
  {
    assert(GetType() == InputEventType);
    return std::get<static_cast<int>(InputEventType)>(*this);
  }

  template <InputEvent::Type InputEventType>
  const auto& As() const
  {
    return const_cast<InputEvent&>(*this).As<InputEventType>();
  }
};

}