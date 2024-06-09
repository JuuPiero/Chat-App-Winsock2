#include "View/CreateGroupChatForm.h"
#include "ViewModel/UserViewModel.h"
#include "PCH.h"

namespace Piero {
bool CreateGroupChatForm::isShowing = false;
char CreateGroupChatForm::groupNameInputBuffer[100] = "";

void CreateGroupChatForm::Render() {

    if (ImGui::Button("Create new group chat", ImVec2(0, 30))) {
        isShowing = true;
    }
    if(isShowing) {
        ImGui::OpenPopup("Create conversation");

        if (ImGui::BeginPopupModal("Create conversation", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar)) {
            ImGui::SetWindowFontScale(2.0f);
            ImGui::Text("Create group chat");
            ImGui::SetWindowFontScale(1.3f);
            ImGui::Separator();
            // user name

            if(ImGui::InputText("Name", groupNameInputBuffer, IM_ARRAYSIZE(groupNameInputBuffer))) {}

            if (ImGui::Button("Create", ImVec2(120, 0))) {
                //TODO: Connect server
                // OnCreateConversation(const std::string& name);
                UserViewModel::GetInstance()->OnCreateConversation(std::string(groupNameInputBuffer));
                ImGui::CloseCurrentPopup();
                isShowing = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
                isShowing = false;
            }
            
            ImGui::EndPopup();
        }
    }

}
}