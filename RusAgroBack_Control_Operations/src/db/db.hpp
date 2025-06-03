#pragma once
#include <string>
#include <memory>
#include <pqxx/pqxx>
#include <yaml-cpp/yaml.h>

class Database 
{
public:
    
    explicit Database(const std::string& config_file);

    
    ~Database();

    
    void connect();

    
    pqxx::connection& getConnection();

    pqxx::result FetchInitialDataRaw();

private:
    std::string host_;
    int         port_;
    std::string user_;
    std::string password_;
    std::string dbname_;
    std::string sslmode_;  // опционально

    std::unique_ptr<pqxx::connection> conn_;

    void loadConfig(const std::string& config_file);
};