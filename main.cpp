#include "SceneRegistry.hpp"
#include "base/assert.hpp"

#include <chrono>
#include <GLFW/glfw3.h>

using namespace std;

class MainLoop
{
public:
  MainLoop(std::unique_ptr<IScene> scene, GLFWwindow * window)
    : m_scene(std::move(scene))
    , m_window(window)
  {
  }

  void OnSize(int w, int h)
  {
    m_scene->OnSize(w, h);
  }

  void Run()
  {
    using THighResClock = std::chrono::high_resolution_clock;
    using TTimePoint = THighResClock::time_point;
    using std::chrono::duration;
    using std::chrono::duration_cast;

    double elapsedSeconds = 0.0;
    while (!glfwWindowShouldClose(m_window))
    {
      TTimePoint startTime = THighResClock::now();
      m_scene->Update(elapsedSeconds);
      m_scene->Render();

      glfwSwapBuffers(m_window);
      glfwPollEvents();
      elapsedSeconds = duration_cast<duration<double>>(THighResClock::now() - startTime).count();
    }
  }

private:
  std::unique_ptr<IScene> m_scene;
  GLFWwindow * m_window;
};

std::unique_ptr<MainLoop> g_mainLoop;

void WindowSizeChanged(GLFWwindow * /*window*/, int w, int h)
{
  if (g_mainLoop)
    g_mainLoop->OnSize(w, h);
}

int main()
{
  std::string const defaultScene("LoadModelScene");

  if (!glfwInit())
  {
    return 1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 16);

  GLFWwindow * window = glfwCreateWindow(800, 600, defaultScene.c_str(), nullptr, nullptr);
  if (window == nullptr)
  {
    glfwTerminate();
    return 2;
  }

  glfwSetWindowSizeCallback(window, &WindowSizeChanged);

  glfwMakeContextCurrent(window);
  std::unique_ptr<IScene> scene;
  SceneRegistry::EnumerateScenes([&scene, &defaultScene](std::string const & title, SceneRegistry::TSceneCreator const & creator)
  {
    if (defaultScene == title)
    {
      ASSERT(scene == nullptr, "Test message");
      scene = creator();
    }
  });

  g_mainLoop.reset(new MainLoop(std::move(scene), window));

  int w, h;
  glfwGetWindowSize(window, &w, &h);
  WindowSizeChanged(window, w, h);

  g_mainLoop->Run();

  return 0;
}
