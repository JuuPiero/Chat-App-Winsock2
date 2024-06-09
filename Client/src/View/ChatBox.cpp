#include "View/ChatBox.h"
#include "PCH.h"
#include "UI/UI.h"
#include "ViewModel/MessageViewModel.h"
#include "ViewModel/UserViewModel.h"

namespace Piero {

void ChatBox::Render() {
    ImGui::Text(MessageViewModel::GetInstance()->CurrentConversation.isGroup ? MessageViewModel::GetInstance()->CurrentConversation.name.c_str() : "");
    ImGui::SameLine();
   
    ImGui::Separator();
    ImGui::BeginChild("Message area", ImVec2(0, Application::GetInstance()->GetWindowHeight() - 100));
    {
        if(s_ScrollToBottom) {
            ImGui::SetScrollY(ImGui::GetScrollMaxY() + 1000.0f);
            s_ScrollToBottom = false;
        }
        // VIEW MESSAGES FROM AN CONVERSATION
        for (auto &mess : MessageViewModel::GetInstance()->Messages) {
            if(mess.username == UserViewModel::GetInstance()->GetUsername()) {
                MyMessageElement(mess.content);
            }
            else {
                MessageElement(mess);
            }
        }
    }
    ImGui::EndChild();

    ImGui::SetWindowFontScale(1.3f);
    if(ImGui::InputTextMultiline(" ", MessageViewModel::GetInstance()->messInputBuffer, 100, ImVec2(0, 35))) {
        MessageViewModel::GetInstance()->SetMessageToSend(std::string(MessageViewModel::GetInstance()->messInputBuffer));
        MessageViewModel::GetInstance()->ClearBuffer();
    }
    ImGui::SetWindowFontScale(1.0f);

    ImGui::SameLine();
    if(ImGui::Button("Send Message", ImVec2(0, 30))) {
        UserViewModel::GetInstance()->OnSendMessage(MessageViewModel::GetInstance()->CurrentConversation.id, MessageViewModel::GetInstance()->GetMessageToSend());
        MessageViewModel::GetInstance()->Messages.push_back(Message(UserViewModel::GetInstance()->GetUsername(), MessageViewModel::GetInstance()->GetMessageToSend()));
        s_ScrollToBottom = true;
    }
    ImGui::SameLine();
    if(ImGui::Button("Clear Message", ImVec2(0, 30))) {
        s_ScrollToBottom = true;
    }
    ImGui::SameLine();
    ListUserOnilne::Render();
}

}