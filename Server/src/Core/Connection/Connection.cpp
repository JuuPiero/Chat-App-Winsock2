#include "Core/Connection/Connection.h"
#include "spdlog/spdlog.h"
namespace Piero {

Connection* Connection::s_Instance = nullptr;

Connection::Connection(): m_Server(getenv("DB_HOST")), m_Port(atoi(getenv("DB_PORT"))), m_User(getenv("DB_USERNAME")), m_Password(getenv("DB_PASSWORD")), m_Database(getenv("DB_DATABASE")) {

    m_Connection = mysql_init(NULL);
    if (!mysql_real_connect(m_Connection, m_Server, m_User, m_Password, m_Database, m_Port, NULL, 0)) {
        spdlog::critical("Couldn't connect mysql: {}", mysql_error(m_Connection));
        mysql_close(m_Connection);
    }
    else {
        spdlog::info("Connected to Mysql");
    }
    m_Result = nullptr;
}

Connection::~Connection() {
    s_Instance = nullptr;
}

Connection* Connection::GetInstance() {
    if(s_Instance == nullptr) {
        s_Instance = new Connection;
    }
    return s_Instance;
}


void Connection::ShutDown() {
    mysql_free_result(m_Result);
    mysql_close(m_Connection);
    delete s_Instance;

    spdlog::info("Close Connection");

}  

void Connection::Query(const char* query) {
    mysql_free_result(m_Result);
    if (mysql_query(m_Connection, query)) {
        spdlog::error("Query failed: {}", mysql_error(m_Connection));
        mysql_close(m_Connection);
    }
    m_Result = mysql_use_result(m_Connection);
}

void Connection::ShowResults() {
    MYSQL_ROW row;
    if (m_Result) {
        int numFields = mysql_num_fields(m_Result);
        while ((row = mysql_fetch_row(m_Result))) {
            for (int i = 0; i < numFields; i++) {
                std::cout << (row[i] ? row[i] : "NULL") << " ";
            }
            std::cout << std::endl;
        }
    } 
    else {
        spdlog::warn("No Results");
    }
}
}