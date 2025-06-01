#pragma once

#include <future>
#include <string>
#include <memory>

// Forward declaration ��� ���������� ����������� �� libpqxx � ���������
namespace pqxx 
{
    class connection;
}

class Database 
{
public:
    Database();
    ~Database();

    // ����������� ����� ����������� � ��
    std::future<void> connect(
        const std::string& host = "localhost",
        int port = 5432,
        const std::string& dbname = "agro_system",
        const std::string& user = "postgres",
        const std::string& password = "1111"
    );

    // ��������, ����������� �� ����������
    bool is_connected() const;

    // ����� �������� ����������
    void disconnect();

private:
    std::unique_ptr<class pqxx::connection> conn_; // ���������� forward declaration
};