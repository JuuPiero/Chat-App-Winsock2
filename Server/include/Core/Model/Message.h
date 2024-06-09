#pragma once

#include <vector>
#include <memory>
namespace Piero {
class User;
class Message {
public:
    Message(int id, const std::string& content): m_Id(id), m_Content(content) {}
    Message(int id, int conversationId, int userId, const std::string& content, const std::string& createdAt = ""): m_Id(id), m_ConversationId(conversationId), m_UserId(userId), m_Content(content), m_CreatedAt(createdAt) {}
    Message(): m_Id(-1), m_Content("") {}
    ~Message() = default;
    inline int GetId() const { return m_Id; }
    inline std::string GetContent() { return m_Content; }
    inline int GetConversationId() const { return m_ConversationId; }
    inline int GetUserId() const { return m_UserId; }
    inline const std::string& CreatedAt() const { return m_CreatedAt; }

    static std::shared_ptr<Message> Create(int conversationId, int userId, const std::string& content);

public:
    std::shared_ptr<User> user;
private:
    int m_Id;
    int m_ConversationId;
    int m_UserId;
    std::string m_CreatedAt;
    std::string m_Content;
};
}