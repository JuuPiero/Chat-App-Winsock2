#include "View/ListConservations.h"
#include "View/CreateGroupChatForm.h"
#include "ViewModel/UserViewModel.h"
#include "ViewModel/MessageViewModel.h"

#include "PCH.h"
namespace Piero {
void ListConservations::Render() {
    ImGui::Dummy(ImVec2(0, 20));

    CreateGroupChatForm::Render();
    
    for (const auto& conversation : MessageViewModel::GetInstance()->Conversations) {
        if(conversation.id == MessageViewModel::GetInstance()->CurrentConversation.id) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.6f, 0.0f, 1.0f));
            if(ImGui::Button((conversation.name + " " + std::to_string(conversation.id)).c_str(), ImVec2(300, 80))) {
                MessageViewModel::GetInstance()->OnGetConversation(UserViewModel::GetInstance()->GetId(), conversation.id);
            }
            ImGui::PopStyleColor();
        }
        else {
            if(ImGui::Button((conversation.name + " " + std::to_string(conversation.id)).c_str(), ImVec2(300, 80))) {
                MessageViewModel::GetInstance()->OnGetConversation(UserViewModel::GetInstance()->GetId(), conversation.id);
            }
        }

    }
}

}