#include "db.hpp"

#include <iostream>     
#include <sstream>      
#include <stdexcept>    

Database::Database(const std::string& config_file)
    : port_(0)
{
    loadConfig(config_file);
}

Database::~Database() 
{
    if (conn_ && conn_->is_open()) 
    {
        try 
        {
            // Закрываем соединение через close()
            conn_->close();
        }
        catch (const std::exception& e) 
        {
            std::cerr << "[Database::~Database] Ошибка при закрытии соединения: "
                << e.what() << std::endl;
        }
    }
}

void Database::loadConfig(const std::string& config_file) 
{
    try 
    {
        YAML::Node config = YAML::LoadFile(config_file);

        // Проверяем обязательные поля
        if (!config["host"] || !config["port"] || !config["user"] ||
            !config["password"] || !config["dbname"])
        {
            throw std::runtime_error("В файле конфигурации отсутствуют обязательные поля.");
        }

        host_ = config["host"].as<std::string>();
        port_ = config["port"].as<int>();
        user_ = config["user"].as<std::string>();
        password_ = config["password"].as<std::string>();
        dbname_ = config["dbname"].as<std::string>();

        // Опциональное поле sslmode
        if (config["sslmode"]) 
        {
            sslmode_ = config["sslmode"].as<std::string>();
        }
        else 
        {
            sslmode_.clear();
        }

    }
    catch (const YAML::BadFile& e) 
    {
        throw std::runtime_error("Не удалось открыть YAML-файл: " + std::string(e.what()));
    }
    catch (const YAML::ParserException& e) 
    {
        throw std::runtime_error("Ошибка парсинга YAML-файла: " + std::string(e.what()));
    }
    // прочие исключения (например, отсутствует поле) просто пробрасываем
}

void Database::connect() 
{
    if (conn_ && conn_->is_open()) 
    {
        // Уже подключены, ничего не делаем
        return;
    }

    // Собираем строку подключения в формате libpq
    std::ostringstream conn_str;
    conn_str << "host=" << host_
        << " port=" << port_
        << " user=" << user_
        << " password=" << password_
        << " dbname=" << dbname_;
    if (!sslmode_.empty()) 
    {
        conn_str << " sslmode=" << sslmode_;
    }

    try 
    {
        // Создаём соединение
        conn_ = std::make_unique<pqxx::connection>(conn_str.str());

        if (!conn_->is_open()) 
        {
            throw std::runtime_error("Не удалось открыть соединение с базой данных.");
        }
    }
    catch (const pqxx::broken_connection& e) 
    {
        throw std::runtime_error("Ошибка соединения с PostgreSQL: " + std::string(e.what()));
    }
    catch (const std::exception& e) 
    {
        throw std::runtime_error("Ошибка при попытке connect(): " + std::string(e.what()));
    }
}

pqxx::connection& Database::getConnection() 
{
    if (!conn_ || !conn_->is_open()) 
    {
        throw std::runtime_error("Соединение с БД не установлено. Вызовите connect() перед использованием.");
    }
    return *conn_;
}

pqxx::result Database::FetchInitialDataRaw()
{
    pqxx::work txn(*conn_);

    const std::string query = R"(
        SELECT 
            culture_id,
            t_material_id,
            region_id,
            season,
            tech_operation,
            region_date,
            input_operation_order,
            alternative_operation_order,
            input_deadline,
            alternative_deadline,
            noinput_deadline,
            "order",
            year
        FROM static_initial_data
    )";

    pqxx::result result = txn.exec(query);
    txn.commit();
    return result;
}
