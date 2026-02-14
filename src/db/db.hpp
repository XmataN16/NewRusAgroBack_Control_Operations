#pragma once
#include <data_struct/SapData.hpp>
#include <utils/utilsBoostDate.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <optional>
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

    pqxx::result fetchInitialDataRaw();

    pqxx::result fetchSapControlOperationsRaw();

    pqxx::result fetchSapSugarBeetSawingDatesOpsRaw();

    pqxx::result fetchSapIDSSeedingRaw();

    pqxx::result fetchSapIDSReseedingRaw();

    void insertIntoControlOperationsAggregated(const YearSlices& uniqueSlices);

    void insertIntoSapSugarBeetSawingDates(const YearSlices& uniqueSlices);

    void truncateAndRestartIdentity(std::string table_name);

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