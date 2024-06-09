#pragma once
#define GLFW_INCLUDE_NONE
#define DEFAULT_BUFLEN 5012
#include "glad/gl.h"
#include <glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <sstream>
#include <memory>
#include <fstream>
#include <iostream>
#include <time.h>
#include <map>
#include <thread>
#include <utility>
#include <algorithm>
#include <winsock2.h>
#include <windows.h>
#include <cstring>
#include <sstream>

#include "Core/Application/Application.h"
#include "Core/Client/StatusCode.h"
#include "Core/Window/Window.h"
#include "Core/Client/Client.h"

#include "Model/Conversation.h"
#include "Model/Message.h"
#include "Model/User.h"

#include "View/ListUserOnilne.h"
#include "View/ChatBox.h"
#include "View/ListConservations.h"
#include "View/AcceptModal.h"


#include <nlohmann/json.hpp>
using json = nlohmann::json;

static bool s_ScrollToBottom = false;