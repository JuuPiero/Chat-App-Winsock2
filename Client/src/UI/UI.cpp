#include "PCH.h"
#include "UI/UI.h"
#include "ViewModel/UserViewModel.h"
namespace Piero {


void MyMessageElement(std::string message) {
    // Calculate the height needed for the name text
    const float maxWidth = 400.0f;
    ImGui::SetWindowFontScale(1.3f);
    ImVec2 messageTextSize = ImGui::CalcTextSize(message.c_str(), nullptr, true, maxWidth);
    ImGui::SetWindowFontScale(1.0f);
    // Calculate the total height needed for the outer child
    float totalHeight = messageTextSize.y + ImGui::GetStyle().FramePadding.y * 8; // Adding padding for both texts and child window borders
    float totalWidth = messageTextSize.x + ImGui::GetStyle().FramePadding.x * 8;

    float textWidth = ImGui::CalcTextSize(message.c_str()).x > maxWidth ? maxWidth : (ImGui::CalcTextSize(message.c_str()).x + 40.0f);

    ImGui::SetWindowFontScale(1.3f);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - textWidth - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.x );
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
    std::string messId = message + std::to_string(rand());
    ImGui::BeginChild(messId.c_str(), ImVec2(totalWidth, totalHeight), true, ImGuiWindowFlags_NoScrollbar);
    {
        ImGui::TextWrapped(message.c_str());
    }
    ImGui::SetWindowFontScale(1.0f);
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::Dummy(ImVec2(0, 10));
}


void ModalLoginForm(const char* id)  {
    ImGui::OpenPopup("Login");

    if (ImGui::BeginPopupModal("Login", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar)) {
        ImGui::SetWindowFontScale(2.0f);
        ImGui::Text("Login Form");
        ImGui::SetWindowFontScale(1.3f);
        ImGui::Separator();
        // user name
        if(ImGui::InputText("UserName", UserViewModel::GetInstance()->usernameBufferInput, IM_ARRAYSIZE(UserViewModel::GetInstance()->usernameBufferInput))) {}
        
        if(ImGui::InputText("Password", UserViewModel::GetInstance()->passwordBufferInput, IM_ARRAYSIZE(UserViewModel::GetInstance()->passwordBufferInput), ImGuiInputTextFlags_Password)) {
            std::cout << UserViewModel::GetInstance()->passwordBufferInput << std::endl;
        }

        if (ImGui::Button("Login", ImVec2(120, 0))) {
            //TODO: Connect server
            UserViewModel::GetInstance()->OnUserLogin(std::string(UserViewModel::GetInstance()->usernameBufferInput), std::string(UserViewModel::GetInstance()->passwordBufferInput));
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        
        if (ImGui::Button("Register", ImVec2(120, 0))) {
            UserViewModel::GetInstance()->OnSignup(std::string(UserViewModel::GetInstance()->usernameBufferInput), std::string(UserViewModel::GetInstance()->passwordBufferInput));
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}


void MessageElement(Message message) {
    const float maxWidth = 400.0f;
    // ImGui::Dummy(ImVec2(0, 15));
    // Calculate the height needed for the name text
    ImGui::SetWindowFontScale(1.5f);
    ImVec2 nameTextSize = ImGui::CalcTextSize(message.username.c_str());
    ImGui::SetWindowFontScale(1.0f);
    // Calculate the height needed for the message text
    ImGui::SetWindowFontScale(1.3f);
    ImVec2 messageTextSize = ImGui::CalcTextSize(message.content.c_str(), nullptr, true, maxWidth);
    ImGui::SetWindowFontScale(1.0f);

    ImVec2 messageCreatedAtSize = ImGui::CalcTextSize(message.createdAt.c_str(), nullptr, true, maxWidth);

    // Calculate the total height needed for the outer child
    float totalHeight = nameTextSize.y + messageTextSize.y + messageCreatedAtSize.y + ImGui::GetStyle().FramePadding.y * 8; // Adding padding for both texts and child window borders
    float totalWidth = messageTextSize.x + ImGui::GetStyle().FramePadding.x * 8;

    std::string id = message.content + std::to_string(rand());

    float widthBonus = (nameTextSize.x > messageCreatedAtSize.x) ? nameTextSize.x : messageCreatedAtSize.x;

    // Begin the outer child window with calculated height
    ImGui::BeginChild(id.c_str(), ImVec2(totalWidth + widthBonus, totalHeight), 0, ImGuiWindowFlags_NoScrollbar);
    {
        // Name
        ImGui::SetWindowFontScale(1.5f);
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), message.username.c_str()); // Chữ màu đỏ
        ImGui::SetWindowFontScale(1.0f);
        // ImGui::SameLine();
        ImGui::Text(("at " +  message.createdAt).c_str());

        // Message
        if(!message.isQuitMessage) {
            ImGui::SetWindowFontScale(1.3f);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
            std::string messId = message.content + std::to_string(rand());
            ImGui::BeginChild(messId.c_str(), ImVec2(totalWidth, messageTextSize.y + ImGui::GetStyle().FramePadding.y + 10), true, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::TextWrapped(message.content.c_str());
            }
            ImGui::EndChild();
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleColor();
        }
        else {
            ImGui::SetWindowFontScale(1.3f);
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            std::string messId = message.content + std::to_string(rand());
            ImGui::BeginChild(messId.c_str(), ImVec2(totalWidth, messageTextSize.y + ImGui::GetStyle().FramePadding.y + 10), true, ImGuiWindowFlags_NoScrollbar);
            {
                ImGui::TextWrapped(message.content.c_str());
            }
            ImGui::EndChild();
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopStyleColor();
        }
    }
    ImGui::EndChild();
}

}