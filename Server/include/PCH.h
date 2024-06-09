#pragma once
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"
// #include "glad/GL.h"
// #include <glfw3.h>
#include <cstdlib>
#include <stdlib.h>
#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <iostream>
#include <time.h>
#include <thread>
#include <algorithm>
#include <functional>
#include <ctime>
#include <iomanip>
#include <random>
#include "spdlog/spdlog.h"

#include "Helper/Helper.h"
#include "Core/Connection/Connection.h"
#include "Core/Server/Server.h"
#include "Core/Server/StatusCode.h"
#include "Core/Application/Application.h"
#include "Core/Model/Conversation.h"
#include "Core/Model/User.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

inline uint32_t GetId() {
    static uint32_t clientIdCounter = 0;
    return ++clientIdCounter;
}

inline std::string GenerateUUID() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);

    std::stringstream ss;
    ss << std::hex;

    for (int i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4"; // 4 cố định cho UUID phiên bản 4
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen); // y là một trong các giá trị 8, 9, a, b
    for (int i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (int i = 0; i < 12; i++) {
        ss << dis(gen);
    }

    return ss.str();
}