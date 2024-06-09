#pragma once

#include "PCH.h"

#include <functional>

namespace Piero {
class Window;    
class Application {
public:
    static Application* GetInstance();

    void Run(std::function<void()> callback = []() {});
    void ShutDown();

    int GetWindowWidth();
    int GetWindowHeight();
private:
    void LoadDotenv(const std::string& filepath);
    Application();
    ~Application();
private:
    std::shared_ptr<Window> m_Window;
    static Application* s_Instance;
};

}