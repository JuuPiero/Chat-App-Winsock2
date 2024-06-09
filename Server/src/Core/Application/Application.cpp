#include "Core/Application/Application.h"
#include "PCH.h"

namespace Piero {
Application* Application::s_Instance = nullptr;

Application* Application::GetInstance() {
    if(s_Instance == nullptr) {
        s_Instance = new Application();
    }
    return s_Instance;
}

void Application::Run(std::function<void()> callback) {
    m_IsRunning = true;

    while(m_IsRunning) {

        Server::GetInstance()->Run();
        
        callback();
    }
}

void Application::ShutDown() {
    Connection::GetInstance()->ShutDown();
    Server::GetInstance()->ShutDown();

    m_IsRunning = false;
}

void Application::LoadDotenv(const std::string& filepath)  {
    std::ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Xóa khoảng trắng ở đầu và cuối dòng
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        // Bỏ qua các dòng trống hoặc comment
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (!std::getline(iss, value)) {
                value = "";
            }

            // Xóa khoảng trắng ở đầu và cuối key và value
            key.erase(key.begin(), std::find_if(key.begin(), key.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
            key.erase(std::find_if(key.rbegin(), key.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), key.end());

            value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
            value.erase(std::find_if(value.rbegin(), value.rend(), [](unsigned char ch) {
                return !std::isspace(ch);
            }).base(), value.end());

            // Đặt biến môi trường ngay cả khi giá trị rỗng
            #if defined(_WIN32) || defined(_WIN64)
                if (_putenv_s(key.c_str(), value.c_str()) != 0) {
                    std::cerr << "Error setting environment variable: " << key << std::endl;
                }
            #else
                if (setenv(key.c_str(), value.c_str(), 1) != 0) {
                    std::cerr << "Error setting environment variable: " << key << std::endl;
                }
            #endif
        }
    }

    file.close();

    SPDLOG_INFO("Loading environment variables");
}


Application::Application() {
    spdlog::set_pattern("%^[%H:%M:%S] [thread %t] %v%$");
    LoadDotenv("../.env");
    SPDLOG_INFO("Created Application");
}

Application::~Application() {}

}