#include "Core/Model/Conversation.h"
#include "Core/Model/Message.h"
#include "Core/Model/User.h"
#include <string>

namespace Piero {
std::shared_ptr<Conversation> Conversation::GetConversationById(int id) {
    std::shared_ptr<Conversation> conversation;
    std::string query = "SELECT * FROM conversations c WHERE id = " + std::to_string(id);
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_ROW row;
    if((row = mysql_fetch_row(Connection::GetInstance()->GetResult())) != NULL) {
        int id = atoi(row[0]);
        std::string name = (row[1] != NULL) ? std::string(row[1]) : "";
        bool is_group = atoi(row[2]) == 1;
        conversation = std::make_shared<Conversation>(id, name, is_group);
    }
    return conversation;
}

std::vector<std::shared_ptr<Conversation>> Conversation::GetAllConversationsByUserId(const int userId) {
    std::vector<std::shared_ptr<Conversation>> conversations;
    std::string query = "SELECT c.id , c.name , c.is_group  FROM conversations c JOIN participants p ON c.id = p.conversation_id WHERE p.user_id = " + std::to_string(userId);
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(Connection::GetInstance()->GetResult())) != NULL) {
        int id = atoi(row[0]);
        std::string name = (row[1] != NULL) ? std::string(row[1]) : "";
        bool is_group = atoi(row[2]) == 1;
        
        conversations.push_back(std::make_shared<Conversation>(id, name, is_group));
    }
    return conversations;
}

std::string Conversation::GetReceiverUsername(int senderId, int conversationId) {
    int receiverId = 0;
    std::string query = "SELECT p.* FROM participants p JOIN conversations c ON p.conversation_id = c.id WHERE p.user_id != " + std::to_string(senderId) + " AND c.id = " + std::to_string(conversationId);
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(Connection::GetInstance()->GetResult())) != NULL) {
        receiverId = atoi(row[2]);
    }
    return User::GetUsernameById(receiverId);
}
int Conversation::GetReceiverId(int senderId, int conversationId) {
    int receiverId = 0;
    std::string query = "SELECT p.* FROM participants p JOIN conversations c ON p.conversation_id = c.id WHERE p.user_id != " + std::to_string(senderId) + " AND c.id = " + std::to_string(conversationId);
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(Connection::GetInstance()->GetResult())) != NULL) {
        receiverId = atoi(row[2]);
    }
    return receiverId;
}

std::vector<std::shared_ptr<Message>> Conversation::GetMessages(int id) {
    std::vector<std::shared_ptr<Message>> messages;
    std::string query = "SELECT * FROM messages WHERE conversation_id = " + std::to_string(id);
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_RES* result = Connection::GetInstance()->GetResult();
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        int id = atoi(row[0]);
        int conversationId = atoi(row[1]);
        int userId = atoi(row[2]);
        std::string message(row[3]);
        std::string createAt(row[4]);
        messages.push_back(std::make_shared<Message>(id, conversationId, userId, message, createAt));    
    }
    return messages;
}


std::vector<int> Conversation::GetMembersExcept(int userId, int conversationId) {
    std::vector<int> members;
    std::string query = "SELECT u.id FROM users u JOIN participants p ON u.id = p.user_id WHERE p.conversation_id = " + std::to_string(conversationId) + " AND u.id != " + std::to_string(userId);
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_RES* result = Connection::GetInstance()->GetResult();
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        members.push_back(atoi(row[0]));
    }

    if(members.size() < 1) members.push_back(0);
    return members;
}
std::shared_ptr<Conversation> Conversation::GetConversationOfTwoUsers(int userId1, int userId2) {
    std::shared_ptr<Conversation> conversation;
    std::string query = "SELECT c.id, c.name, c.is_group FROM conversations c JOIN participants p1 ON c.id = p1.conversation_id JOIN participants p2 ON c.id = p2.conversation_id WHERE p1.user_id = " + std::to_string(userId1) + " AND p2.user_id = " + std::to_string(userId2) + " AND c.is_group = 0 AND c.id IN ( SELECT conversation_id FROM participants GROUP BY conversation_id HAVING COUNT(user_id) = 2)";
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_RES* result = Connection::GetInstance()->GetResult();
    MYSQL_ROW row;
    if(row = mysql_fetch_row(result)) {
        conversation = std::make_shared<Conversation>(atoi(row[0]), (row[1] != NULL) ? row[1] : "", atoi(row[2]) == 1 ? true : false);
    }
    return conversation;
}

int Conversation::CreateConversation(int userId, const std::string& name, bool isGroup) {
    std::string query = "INSERT INTO conversations (name, is_group) VALUES ( '" + name + "', " + std::to_string(isGroup) + ")";
    Connection::GetInstance()->Query(query.c_str());

    query = "SELECT * FROM conversations ORDER BY created_at DESC LIMIT 1";
    Connection::GetInstance()->Query(query.c_str());

    int conversationId = 0;
    MYSQL_RES* result = Connection::GetInstance()->GetResult();
    MYSQL_ROW row;
    if(row = mysql_fetch_row(result)) {
        conversationId = atoi(row[0]);
    }
    query = "INSERT INTO participants (conversation_id, user_id) VALUES (" + std::to_string(conversationId) + ", " + std::to_string(userId) + ")";
    Connection::GetInstance()->Query(query.c_str());
    return conversationId;
}

int Conversation::CreateConversationOfTwoUsers(int userId1, int userId2) {
    std::string query = "INSERT INTO conversations (is_group) VALUES (0)";
    Connection::GetInstance()->Query(query.c_str());
    query = "SELECT id, name, is_group, created_at FROM conversations ORDER BY created_at DESC LIMIT 1";
    Connection::GetInstance()->Query(query.c_str());
    int conversationId = 0;
    MYSQL_RES* result = Connection::GetInstance()->GetResult();
    MYSQL_ROW row;
    if(row = mysql_fetch_row(result)) {
        conversationId = atoi(row[0]);
    }
    // create participants
    query = "INSERT INTO participants (conversation_id, user_id) VALUES (" + std::to_string(conversationId) + ", " + std::to_string(userId1) + ")";
    Connection::GetInstance()->Query(query.c_str());
    query = "INSERT INTO participants (conversation_id, user_id) VALUES (" + std::to_string(conversationId) + ", " + std::to_string(userId2) + ")";
    Connection::GetInstance()->Query(query.c_str());

    return conversationId;
}
std::vector<std::shared_ptr<User>> Conversation::GetUserCanJoin(int conversationId) {
    std::vector<std::shared_ptr<User>> users;
    std::string query = "SELECT u.* FROM users u WHERE u.id NOT IN (SELECT p.user_id FROM participants p WHERE p.conversation_id = " + std::to_string(conversationId) + ")";
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_RES* result = Connection::GetInstance()->GetResult();
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result))) {
        int id = atoi(row[0]);
        std::string username = std::string(row[1]);
        std::string password = std::string(row[2]);
        bool status = atoi(row[3]) == 1 ? true : false;
        users.push_back(std::make_shared<User>(id, username, password, status));
    }
    return users;
}
}