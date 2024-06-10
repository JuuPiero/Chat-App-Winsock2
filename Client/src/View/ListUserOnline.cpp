#include "View/ListUserOnilne.h"
#include "ViewModel/UserViewModel.h"
#include "ViewModel/MessageViewModel.h"

#include "PCH.h"
namespace Piero {
bool ListUserOnilne::isShowing = false;

void ListUserOnilne::Render() {
    if (ImGui::Button("Users Online", ImVec2(0, 30))) {
        UserViewModel::GetInstance()->OnGetAllUsersOnline();
        isShowing = true;
    }
    if(isShowing) {
        ImGui::OpenPopup("List User Onilne");
        if (ImGui::BeginPopupModal("List User Onilne", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar)) {
            if (ImGui::Button("X", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
                UserViewModel::GetInstance()->UsersOnline.clear();
                isShowing = false;
            }
            auto userOnline = UserViewModel::GetInstance()->UsersOnline;
            for (size_t i = 0; i < userOnline.size(); i++) {
                if (ImGui::Button(userOnline[i].username.c_str(), ImVec2(280, 45))) {
                    MessageViewModel::GetInstance()->OnGetOrCreateConversationOfTwoUser(UserViewModel::GetInstance()->GetId(), userOnline[i].id);
                }
                if(MessageViewModel::GetInstance()->CurrentConversation.isGroup) {
                    int idToCheck = userOnline[i].id;
                    auto it = std::find_if(MessageViewModel::GetInstance()->CurrentConversation.members.begin(), MessageViewModel::GetInstance()->CurrentConversation.members.end(), [idToCheck](const User& usercheck) {
                        return usercheck.id == idToCheck;
                    });
                    if (it == MessageViewModel::GetInstance()->CurrentConversation.members.end()) {
                        ImGui::SameLine();
                        std::string buttonLabel = "+##" + std::to_string(i);
                        if (ImGui::Button(buttonLabel.c_str(), ImVec2(35, 35))) {
                            UserViewModel::GetInstance()->OnInviteUser(MessageViewModel::GetInstance()->CurrentConversation.id, idToCheck);
                        }
                    } 
                }
            }
           
        }
        ImGui::EndPopup();

    }
}

}
