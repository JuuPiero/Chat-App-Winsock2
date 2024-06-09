#pragma once

namespace Piero {
struct User {
    int id;
    std::string username;
    std::string password;
    User(int id, std::string username, std::string password): id(id), username(username), password(password) {}
    User(int id, std::string username): id(id), username(username), password("") {}
    User(): id(0), username(""), password("") {}
};
}