#pragma once
#include <ctime>
#include <iostream>
#define PrintLn(mess) std::cout << mess << std::endl

inline std::string getCurrentTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return std::string(buf);
}
