#pragma once
#include "PCH.h"
#include <string>
#include <cstring>
namespace Piero {

class UserViewModel {
public:
    void OnUserLogin(const std::string& username, const std::string& password);
    void OnUserRegister();
    void OnUserLogout();
    void OnGetAllUsersOnline();
    void OnSendMessage(int conversationId, std::string message, bool isQuitMessage = false);

    void OnCreateConversation(const std::string& name);

    void OnInviteUser(int conversationId, const int& userId);
    // void OnReceiveInvite(int userId, int conversationId, bool isAccept = false);
    void OnAcceptInvite(int conversationId);
    // void OnDeclineInvite(int userId, int conversationId);
    ~UserViewModel() = default;
    static UserViewModel* GetInstance();

    inline bool IsLoggedIn() const { return m_IsLoggedIn; };

    inline void SetLogin(bool b) { m_IsLoggedIn = b; }
    inline void SetPassword(const std::string& password) { 
        m_Password =  password; 
        CurrentUser.password = m_Password;
    }
    inline std::string GetPassword() const { return m_Password; }
    inline std::string GetUsername() const { return CurrentUser.username; }
    inline int GetId() const { return CurrentUser.id; }
    inline void Reset() {
        m_IsLoggedIn = false;
        m_Password = "";
        UsersOnline.clear();
        CurrentUser = User();
    }
    inline void ShutDown() {
        Reset();
        std::cout << "Free UserViewModel" << std::endl;
        delete s_Instance;
        s_Instance = nullptr;
    }
private:
    UserViewModel() {}
public:
    char usernameBufferInput[255] = "";
    char passwordBufferInput[255] = "";
   
    std::vector<User> UsersOnline;
    User CurrentUser;
private:
    static UserViewModel* s_Instance;
    bool m_IsLoggedIn = false;
    std::string m_Password;
};


}