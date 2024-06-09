#pragma once

namespace Piero {
struct Message {
    int id;
    std::string username;
    std::string content;
    std::string createdAt;
    bool isQuitMessage;
    // Message(int id, std::string username, std::string content) : id(id), content(content), username(username) {}
    Message(int id, std::string username, std::string content, std::string createdAt = "") : id(id), content(content), username(username), createdAt(createdAt), isQuitMessage(false) {}
    Message(std::string username, std::string content) : id(0), content(content), username(username), createdAt(""), isQuitMessage(false) {}

    
    Message() : id(0), content(""), username(""), createdAt(""), isQuitMessage(false) {}
};

}