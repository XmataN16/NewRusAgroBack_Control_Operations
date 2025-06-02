#pragma once
#include <future>
#include <string>
#include <memory>
#include <vector>

#include "InitialData.hpp"
#include "SapData.hpp"
#include "SapDataAggregated.hpp"
#include "utilsBoostDate.hpp"

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

    std::future<InitialData> LoadInitialData();

    std::future<SapData> LoadSapData();

    std::future<SapDataAggregated> LoadSapDataAggregated();

private:
    std::unique_ptr<class pqxx::connection> conn_; // ���������� forward declaration
};