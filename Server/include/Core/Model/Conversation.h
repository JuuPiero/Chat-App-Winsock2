#pragma once
#include "Core/Model/Message.h"
#include "Core/Model/User.h"
#include <memory>
#include <vector>
namespace Piero {
// class User;
class Message;

class Conversation {

public:
    Conversation(): m_Id(0), m_Name(""), m_isGroup(false) {}
    Conversation(int id, const std::string& name, bool isGroup): m_Id(id), m_Name(name), m_isGroup(isGroup) {}
    
    ~Conversation() = default;
    inline const int GetId() const { return m_Id; }
    inline const std::string GetName() const { return m_Name; }
    inline const bool IsGroup() const { return m_isGroup; }
    inline void SetName(const std::string& name) { m_Name = name; }

    static std::vector<std::shared_ptr<Conversation>> GetAllConversationsByUserId(const int userId);

    static std::vector<std::shared_ptr<Message>> GetMessages(int id);
    static std::shared_ptr<Conversation> GetConversationById(int id);

    
    static std::vector<int> GetMembersExcept(int userId, int conversationId); // Return id
    static std::shared_ptr<Conversation> GetConversationOfTwoUsers(int userId1, int userId2);

    static int CreateConversation(int userId, const std::string& name, bool isGroup = true);
    static int CreateConversationOfTwoUsers(int userId1, int userId2);

    static std::string GetReceiverUsername(int senderId, int conversationId);
    static int GetReceiverId(int senderId, int conversationId);
    
    static std::vector<std::shared_ptr<User>> GetUserCanJoin(int conversationId); 

public:
    std::vector<std::pair<std::string, std::string>> messages;
private:
    int m_Id;
    std::string m_Name;
    bool m_isGroup;
    

};


}