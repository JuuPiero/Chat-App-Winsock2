#pragma once
#include <mysql.h>
#include <iostream>
namespace Piero {

class Connection {
public:
    ~Connection();
    static Connection* GetInstance();
    void ShutDown();
    void Query(const char* query);
    inline MYSQL_RES* GetResult() const { return m_Result; }
    inline MYSQL* GetConnection() const { return m_Connection; }
    

    void ShowResults();
private:
    Connection();
private:
    const char* m_Server;
    const char* m_User;
    const char* m_Password;
    const char* m_Database;
    const uint32_t m_Port; // Port number

    static Connection* s_Instance;
    MYSQL* m_Connection;
    MYSQL_RES* m_Result;
};

}