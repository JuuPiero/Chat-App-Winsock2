#pragma once
#include <memory>
#include <functional>

namespace Piero {

class Application {

public:
    ~Application();
    void Run(std::function<void()> callback = []() {});
    void ShutDown();
    static Application* GetInstance();
    inline bool IsRunning() { return m_IsRunning; }
private:
    void LoadDotenv(const std::string& filepath);
private:
    Application();
    static Application* s_Instance;
    bool m_IsRunning;
};



}