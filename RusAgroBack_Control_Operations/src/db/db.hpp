#pragma once

#include <future>
#include <string>
#include <memory>

// Forward declaration для уменьшения зависимости от libpqxx в заголовке
namespace pqxx 
{
    class connection;
}

class Database 
{
public:
    Database();
    ~Database();

    // Асинхронный метод подключения к БД
    std::future<void> connect(
        const std::string& host = "localhost",
        int port = 5432,
        const std::string& dbname = "agro_system",
        const std::string& user = "postgres",
        const std::string& password = "1111"
    );

    // Проверка, установлено ли соединение
    bool is_connected() const;

    // Явное закрытие соединения
    void disconnect();

private:
    std::unique_ptr<class pqxx::connection> conn_; // Используем forward declaration
};