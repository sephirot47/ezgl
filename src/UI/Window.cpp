#include <ez/Window.h>
#include <ez/InputListener.h>
#include <ez/Macros.h>
#include <GL/glew.h>
#include <algorithm>
#include <thread>
#include <type_traits>

namespace ez
{

template <typename TInputEvent>
void CallInputEventCallback(GLFWwindow* inGLFWWindow, const TInputEvent& inInputEvent);
void GLFWMouseButtonCallback(GLFWwindow* inGLFWWindow, int inButton, int inAction, int inModifiers);
void GLFWMouseMoveCallback(GLFWwindow* inGLFWWindow, double inPosX, double inPosY);
void GLFWMouseEnterExitCallback(GLFWwindow* inGLFWWindow, int inMouseEntered);
void GLFWMouseScrollCallback(GLFWwindow* inGLFWWindow, double inDeltaScrollX, double inDeltaScrollY);
void GLFWKeyCallback(GLFWwindow* inGLFWWindow, int inKey, int inScancode, int inAction, int inModifiers);

Window::Window(const Window::CreateOptions& inCreateOptions)
{
  if (!glfwInit())
    THROW_EXCEPTION("Error initiating GLFW");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, inCreateOptions.mGLVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, inCreateOptions.mGLVersionMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, false);

  glfwWindowHint(GLFW_RESIZABLE, inCreateOptions.mResizable);
  glfwWindowHint(GLFW_SAMPLES, (inCreateOptions.mUseAntialiasing ? 4 : 1));
  glfwWindowHint(GLFW_DOUBLEBUFFER, inCreateOptions.mDoubleBuffer);

  mHandle = glfwCreateWindow(inCreateOptions.mWidth,
      inCreateOptions.mHeight,
      inCreateOptions.mTitle.data(),
      nullptr,
      nullptr);
  if (!mHandle)
    THROW_EXCEPTION("Error creating GLFW window");

  glfwMakeContextCurrent(mHandle);

  glewExperimental = GL_TRUE;
  const auto glew_init_error = glewInit();
  if (glew_init_error != GLEW_OK)
    THROW_EXCEPTION("Error initiating GLEW");

  if (inCreateOptions.mUseAntialiasing)
  {
    GL::Enable(GL::EEnablable::LINE_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  }

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(&GL::ErrorCallback, 0);

  GL::Enable(GL::EEnablable::DEPTH_TEST);
  GL::Enable(GL::EEnablable::BLEND);
  GL::Disable(GL::EEnablable::CULL_FACE);
  GL::BlendFunc(GL::EBlendFactor::SRC_ALPHA, GL::EBlendFactor::ONE_MINUS_SRC_ALPHA);

  glfwSetWindowUserPointer(mHandle, this);

  // Set input callbacks
  glfwSetMouseButtonCallback(mHandle, &GLFWMouseButtonCallback);
  glfwSetCursorPosCallback(mHandle, &GLFWMouseMoveCallback);
  glfwSetCursorEnterCallback(mHandle, &GLFWMouseEnterExitCallback);
  glfwSetScrollCallback(mHandle, &GLFWMouseScrollCallback);
  glfwSetKeyCallback(mHandle, &GLFWKeyCallback);
}

Window::~Window()
{
  glfwDestroyWindow(mHandle);
  glfwTerminate();
}

bool Window::ShouldClose() const { return glfwWindowShouldClose(mHandle); }
void Window::SwapBuffers() { glfwSwapBuffers(mHandle); }
void Window::PollEvents() { glfwPollEvents(); }
void Window::Maximize() { glfwMaximizeWindow(mHandle); }
void Window::Minimize() { glfwIconifyWindow(mHandle); }
void Window::SetCursorMode(const Window::ECursorMode inCursorMode)
{
  glfwSetInputMode(mHandle, GLFW_CURSOR, static_cast<int>(inCursorMode));
}
void Window::SetInterFrameRestTime(const Seconds& inInterFrameRestTime) { mInterFrameRestTime = inInterFrameRestTime; }

Vec2i Window::GetSize() const
{
  Vec2i size;
  glfwGetWindowSize(mHandle, &size[0], &size[1]);
  return size;
}

Vec2i Window::GetFramebufferSize() const
{
  Vec2i framebuffer_size;
  glfwGetFramebufferSize(mHandle, &framebuffer_size[0], &framebuffer_size[1]);
  return framebuffer_size;
}

float Window::GetFramebufferAspectRatio() const
{
  const auto framebuffer_size = GetFramebufferSize();
  return static_cast<float>(framebuffer_size[0]) / framebuffer_size[1];
}

Vec2i Window::GetPosition() const
{
  Vec2i position;
  glfwGetWindowPos(mHandle, &position[0], &position[1]);
  return position;
}

void Window::Loop(const Window::LoopCallback& inLoopCallback)
{
  auto previous_time = Now();
  while (!ShouldClose())
  {
    const auto delta_time = (Now() - previous_time);
    previous_time = Now();
    const auto loop_result = inLoopCallback(delta_time);
    if (loop_result == Window::ELoopResult::END_LOOP)
      break;

    SwapBuffers();
    PollEvents();

    std::this_thread::sleep_for(mInterFrameRestTime);
  }
}

void Window::AddInputEventCallback(const Window::InputEventCallback& inInputEventCallback)
{
  mInputEventCallbacks.push_back(inInputEventCallback);
}
void Window::AddKeyEventCallback(const Window::KeyEventCallback& inKeyEventCallback)
{
  mKeyEventCallbacks.push_back(inKeyEventCallback);
}
void Window::AddMouseButtonEventCallback(const Window::MouseButtonEventCallback& inMouseButtonEventCallback)
{
  mMouseButtonEventCallbacks.push_back(inMouseButtonEventCallback);
}
void Window::AddMouseEnterExitEventCallback(const Window::MouseEnterExitEventCallback& inMouseEnterExitEventCallback)
{
  mMouseEnterExitEventCallbacks.push_back(inMouseEnterExitEventCallback);
}
void Window::AddMouseMoveEventCallback(const Window::MouseMoveEventCallback& inMouseMoveEventCallback)
{
  mMouseMoveEventCallbacks.push_back(inMouseMoveEventCallback);
}
void Window::AddMouseScrollEventCallback(const Window::MouseScrollEventCallback& inMouseScrollEventCallback)
{
  mMouseScrollEventCallbacks.push_back(inMouseScrollEventCallback);
}
const std::vector<Window::InputEventCallback>& Window::GetInputEventCallbacks() const { return mInputEventCallbacks; }
const std::vector<Window::KeyEventCallback>& Window::GetKeyEventCallbacks() const { return mKeyEventCallbacks; }
const std::vector<Window::MouseButtonEventCallback>& Window::GetMouseButtonEventCallbacks() const
{
  return mMouseButtonEventCallbacks;
}
const std::vector<Window::MouseEnterExitEventCallback>& Window::GetMouseEnterExitEventCallbacks() const
{
  return mMouseEnterExitEventCallbacks;
}
const std::vector<Window::MouseMoveEventCallback>& Window::GetMouseMoveEventCallbacks() const
{
  return mMouseMoveEventCallbacks;
}
const std::vector<Window::MouseScrollEventCallback>& Window::GetMouseScrollEventCallbacks() const
{
  return mMouseScrollEventCallbacks;
}

bool Window::IsMouseButtonPressed(const EMouseButton& inMouseButton)
{
  return glfwGetMouseButton(mHandle, static_cast<int>(inMouseButton)) == GLFW_PRESS;
}

bool Window::IsKeyPressed(const EKey& inKey) { return glfwGetKey(mHandle, static_cast<int>(inKey)) == GLFW_PRESS; }

Vec2f Window::GetMousePosition() const
{
  double mouse_position_x = 0.0;
  double mouse_position_y = 0.0;
  glfwGetCursorPos(mHandle, &mouse_position_x, &mouse_position_y);
  const auto mouse_position = Vec2f(static_cast<float>(mouse_position_x), static_cast<float>(mouse_position_y));
  return mouse_position;
}

Vec2f Window::GetMousePositionViewport() const
{
  const auto normalized_mouse_pos = GetMousePosition() / Vec2f(GetFramebufferSize());
  const auto normalized_clamped_mouse_pos = Clamp(normalized_mouse_pos, Zero<Vec2f>(), One<Vec2f>());
  const auto viewport_mouse_pos = Vec2f(normalized_clamped_mouse_pos[0], 1.0f - normalized_clamped_mouse_pos[1]);
  return viewport_mouse_pos;
}

template <typename TInputEvent>
void CallInputEventCallback(GLFWwindow* inGLFWWindow, const TInputEvent& inInputEvent)
{
  const Window& window = *(reinterpret_cast<Window*>(glfwGetWindowUserPointer(inGLFWWindow)));
  // Call callbacks
  {
    for (const auto& event_callback : window.GetInputEventCallbacks()) event_callback({ inInputEvent });

    if constexpr (std::is_same_v<TInputEvent, KeyEvent>)
      for (const auto& event_callback : window.GetKeyEventCallbacks()) event_callback(inInputEvent);
    else if constexpr (std::is_same_v<TInputEvent, MouseButtonEvent>)
      for (const auto& event_callback : window.GetMouseButtonEventCallbacks()) event_callback(inInputEvent);
    else if constexpr (std::is_same_v<TInputEvent, MouseEnterExitEvent>)
      for (const auto& event_callback : window.GetMouseEnterExitEventCallbacks()) event_callback(inInputEvent);
    else if constexpr (std::is_same_v<TInputEvent, MouseMoveEvent>)
      for (const auto& event_callback : window.GetMouseMoveEventCallbacks()) event_callback(inInputEvent);
    else if constexpr (std::is_same_v<TInputEvent, MouseScrollEvent>)
      for (const auto& event_callback : window.GetMouseScrollEventCallbacks()) event_callback(inInputEvent);
  }

  // Call InputListeners
  for (auto& input_listener : window.GetInputListeners()) input_listener->OnInputEvent({ inInputEvent });
}

void Window::AddInputListener(InputListener* inInputListener) { mInputListeners.push_back(inInputListener); }
void Window::RemoveInputListener(InputListener* inInputListener)
{
  mInputListeners.erase(std::find(mInputListeners.cbegin(), mInputListeners.cend(), inInputListener));
}
const std::vector<InputListener*>& Window::GetInputListeners() const { return mInputListeners; }

void GLFWMouseButtonCallback(GLFWwindow* inGLFWWindow, int inButton, int inAction, int inModifiers)
{
  MouseButtonEvent mouse_button_event;
  mouse_button_event.mButton = static_cast<EMouseButton>(inButton);
  mouse_button_event.mAction = static_cast<EMouseAction>(inAction);
  mouse_button_event.mModifiers = static_cast<EModifierKey>(inModifiers);
  CallInputEventCallback(inGLFWWindow, mouse_button_event);
}

void GLFWMouseMoveCallback(GLFWwindow* inGLFWWindow, double inPosX, double inPosY)
{
  MouseMoveEvent mouse_move_event;
  mouse_move_event.mPosition = Vec2f(static_cast<float>(inPosX), static_cast<float>(inPosY));
  CallInputEventCallback(inGLFWWindow, mouse_move_event);
}

void GLFWMouseEnterExitCallback(GLFWwindow* inGLFWWindow, int inMouseEntered)
{
  MouseEnterExitEvent mouse_enter_exit_event;
  mouse_enter_exit_event.mEntered = (inMouseEntered == GLFW_TRUE);
  CallInputEventCallback(inGLFWWindow, mouse_enter_exit_event);
}

void GLFWMouseScrollCallback(GLFWwindow* inGLFWWindow, double inDeltaScrollX, double inDeltaScrollY)
{
  MouseScrollEvent mouse_scroll_event;
  mouse_scroll_event.mDeltaScroll = Vec2f(static_cast<float>(inDeltaScrollX), static_cast<float>(inDeltaScrollY));
  CallInputEventCallback(inGLFWWindow, mouse_scroll_event);
}

void GLFWKeyCallback(GLFWwindow* inGLFWWindow, int inKey, int inScancode, int inAction, int inModifiers)
{
  UNUSED(inScancode);
  KeyEvent key_event;
  key_event.mKey = static_cast<EKey>(inKey);
  key_event.mAction = static_cast<EKeyAction>(inAction);
  key_event.mModifiers = static_cast<EModifierKey>(inModifiers);
  CallInputEventCallback(inGLFWWindow, key_event);
};

}