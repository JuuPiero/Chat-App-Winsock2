#include "View/AcceptModal.h"
#include "ViewModel/UserViewModel.h"
#include "ViewModel/MessageViewModel.h"
#include "PCH.h"

namespace Piero {
bool AcceptModal::isShowing = false;
std::string AcceptModal::message = "";
int AcceptModal::conversationId = 0;

void AcceptModal::Render() {
    if(isShowing) {
        ImGui::OpenPopup(message.c_str());

        if (ImGui::BeginPopupModal(message.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar)) {
            ImGui::SetWindowFontScale(2.0f);
            ImGui::Text(message.c_str());
            ImGui::SetWindowFontScale(1.3f);
            ImGui::Separator();
            
            
            if (ImGui::Button("Accept", ImVec2(120, 0))) {
                // TODO
                UserViewModel::GetInstance()->OnAcceptInvite(conversationId);
                isShowing = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            
            if (ImGui::Button("Don't accept", ImVec2(120, 0))) {
                // TODO

                isShowing = false;
                ImGui::CloseCurrentPopup();
            }
                
            ImGui::EndPopup();
        }
    }


}
}
