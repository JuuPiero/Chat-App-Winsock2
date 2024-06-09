#pragma once
#include "PCH.h"

namespace Piero {
struct WindowProps {
    int Width = 1280, Height = 720;
    std::string Title = "Chat app";
    WindowProps(int width, int height, const char* title) : Width(width), Height(height), Title(title) {}
    WindowProps(): Width(1280), Height(720), Title("Chat app") {}

};

class Window {
public:
    Window();
    Window(const WindowProps& props);
    ~Window();
    void Update();
    void ShutDown();

    inline void PollEvents() const { glfwPollEvents(); }
    inline const int WindowShouldClose() { return glfwWindowShouldClose(m_Window); }
    inline const int& GetWidth() const { return m_Props.Width; }
    inline const int& GetHeight() const { return m_Props.Height; }
    inline GLFWwindow* GetWindow() const { return m_Window; }

    inline WindowProps& GetProps() { return m_Props; }
private:
    GLFWwindow* m_Window;
    WindowProps m_Props;
};
    
}