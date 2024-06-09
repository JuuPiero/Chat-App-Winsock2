#include "Core/Model/Message.h"
#include "Core/Model/User.h"
#include <string>
#include <iostream>

namespace Piero {

std::shared_ptr<Message> Message::Create(int conversationId, int userId, const std::string& content) {
    std::shared_ptr<Message> newMessage;
    std::string query = "INSERT INTO messages (conversation_id, user_id, content) VALUES (" + std::to_string(conversationId) + ", " + std::to_string(userId) + ", '" + content + "')";
    Connection::GetInstance()->Query(query.c_str());
    query = "SELECT * FROM messages WHERE user_id = " + std::to_string(userId) + " AND conversation_id = " + std::to_string(conversationId) + " ORDER BY created_at DESC LIMIT 1";
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(Connection::GetInstance()->GetResult())) != NULL) {
        int id = atoi(row[0]);
        int conversationId = atoi(row[1]);
        int userId = atoi(row[2]);
        std::string content(row[3]);
        std::string createdAt(row[4]);
        newMessage = std::make_shared<Message>(id, conversationId, userId, content, createdAt);
    }
    return newMessage;
}


}
