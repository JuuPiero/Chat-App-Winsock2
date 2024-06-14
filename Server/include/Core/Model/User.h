#pragma once
#include <vector>
#include <memory>
#include "Core/Connection/Connection.h"

namespace Piero {
class Conversation;
class User {
public:
    User(): m_Id(-1), m_Username(""), m_Password(""), m_Status(false) {}
    User(int id, const std::string &username, const std::string &password, bool status) : m_Id(id), m_Username(username), m_Password(password), m_Status(status) {}
    ~User() = default;

    inline const int& GetId() const { return m_Id; }

    inline const bool& GetStatus() const { return m_Status; }
   
    inline const std::string GetUsername() const { return m_Username; }
    inline void SetUsername(const std::string& username) { m_Username = username; }

    inline const std::string& GetPassword() const { return m_Password; }
    inline void SetPassword(const std::string& password) { m_Password; }

    static std::string GetUsernameById(const int id);

    static std::vector<User> All();
    static std::shared_ptr<User> GetUserById(const uint32_t id);

    static std::shared_ptr<User> GetUserByUserName(const std::string& username);

    static void Create(std::string username, std::string password);

    static bool Attempt(const std::string& username, const std::string& password);
    static std::vector<std::shared_ptr<User>> GetAllUserOnline(int id);

    void SetOnline();
    void SetOffline();
    inline void SetStatus(bool status) {
        if(status) SetOnline();
        else SetOffline();
    }
public:
    // std::vector<std::shared_ptr<Piero::Conversation>> conversations;
    
protected:
    std::string table = "users";
    int m_Id;
    std::string m_Username;
    std::string m_Password;
    bool m_Status;
};
}
