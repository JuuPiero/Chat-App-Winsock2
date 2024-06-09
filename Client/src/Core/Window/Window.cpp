#include "Core/Window/Window.h"

namespace Piero {

Window::Window(const WindowProps& props): m_Props(props) {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        ShutDown();
        return;
    }

    m_Window = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), NULL, NULL);
    if (m_Window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        ShutDown();
        return;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1); // Enable vsync
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        ShutDown();
        return;
    }

    // Thiết lập ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Thiết lập style của ImGui
    ImGui::StyleColorsDark();

    // Thiết lập backends
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}   

Window::Window(): m_Props(WindowProps()) {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return;
    }

    m_Window = glfwCreateWindow(m_Props.Width, m_Props.Height, m_Props.Title.c_str(), NULL, NULL);
    if (m_Window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        ShutDown();
        return;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1); // Enable vsync
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        ShutDown();
        return;
    }

     // Thiết lập ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Thiết lập style của ImGui
    ImGui::StyleColorsDark();

    std::cout << "Created a Window" << std::endl;
}

void Window::Update() {
    PollEvents();
    
}

void Window::ShutDown() {
    std::cout << "Free Window" << std::endl;
    
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}
Window::~Window() {}
}