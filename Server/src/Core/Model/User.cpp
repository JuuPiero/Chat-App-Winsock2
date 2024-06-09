#include "Core/Model/User.h"
#include <string>
#include <spdlog/spdlog.h>
#include "Core/Model/Conversation.h"

namespace Piero {
std::vector<User> User::All() {
    MYSQL_ROW row;
    Connection::GetInstance()->Query("SELECT * FROM users");
    std::vector<User> users;
    while ((row = mysql_fetch_row(Connection::GetInstance()->GetResult())) != NULL) {
        users.push_back(User(atoi(row[0]), row[1], row[2], row[3]));
    }
    return users;
}
std::shared_ptr<User> User::GetUserById(const uint32_t id) {
    std::shared_ptr<User> user;
    std::string query = "SELECT * FROM users WHERE id = " + std::to_string(id);
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(Connection::GetInstance()->GetResult())) != NULL) {
        user = std::make_shared<User>(atoi(row[0]), row[1], row[2], row[3]);
    }
    else {
        std::cout << "Row is empty" << std::endl;
    }
   
    return user;
}
std::shared_ptr<User> User::GetUserByUserName(const std::string& username) {
    std::shared_ptr<User> user;
    std::string query = "SELECT * FROM users WHERE username = '" + username + "'";
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(Connection::GetInstance()->GetResult())) != NULL) {
        user = std::make_shared<User>(atoi(row[0]), row[1], row[2], row[3]);
    }
   
    return user;
}
std::vector<std::shared_ptr<User>> User::GetAllUserOnline(int id) {
    std::vector<std::shared_ptr<User>> users;
    std::string query = "SELECT * FROM users WHERE status = 1 AND id != " + std::to_string(id);
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(Connection::GetInstance()->GetResult())) != NULL) {
        users.push_back(std::make_shared<User>(atoi(row[0]), row[1], row[2], row[3]));
    }
    return users;
}

void User::SetOnline() { 
    std::string query = "UPDATE users SET status = '" + std::to_string(1) +  "' WHERE id = " + std::to_string(m_Id);
    Connection::GetInstance()->Query(query.c_str());
}
void User::SetOffline() {
    std::string query = "UPDATE users SET status = '" + std::to_string(0) +  "' WHERE id = " + std::to_string(m_Id);
    Connection::GetInstance()->Query(query.c_str());
}
void User::Create(const std::string& username, const std::string& password) {
    std::string query = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "')";
}
std::string User::GetUsernameById(const int id) {
    std::string query = "SELECT username FROM users WHERE id = " + std::to_string(id);
    Connection::GetInstance()->Query(query.c_str());
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(Connection::GetInstance()->GetResult()))) {
        return std::string(row[0]);
    }
    return "";
}

bool User::Attempt(const std::string& username, const std::string& password) {
    if(GetUserByUserName(username)->GetPassword() == password) {
        return true;
    }
    return false;
}

}