#pragma once
#include <cstring>
#include "PCH.h"
namespace Piero {

class MessageViewModel
{
private:
    MessageViewModel();
public:
    static MessageViewModel* GetInstance();
    ~MessageViewModel();
    inline std::string GetMessageToSend() const { return m_MessageTosend; }
    inline const uint32_t GetConversationId() const { return m_ConversationId; }
    inline void SetMessageToSend(std::string message) { m_MessageTosend = message; }
    inline void ClearBuffer() {
        memset(messInputBuffer, '\0', sizeof(messInputBuffer));
    }
    inline void ShutDown() {
        Reset();
        std::cout << "Free MessageViewModel" << std::endl;
        delete s_Instance;
        s_Instance = nullptr;
    }

    inline void Reset() {
        Conversations.clear();
        Messages.clear();
    }

    
    void OnGetOrCreateConversationOfTwoUser(int userId1, int userId2);
    void OnGetConversation(int userId, int conversationId);
    
public:
    char messInputBuffer[100] = "";
    
    std::vector<Conversation> Conversations;

    std::vector<Message> Messages;
    Conversation CurrentConversation;

private:
    static MessageViewModel* s_Instance;
    std::string m_MessageTosend;
    uint32_t m_ConversationId;
};

}