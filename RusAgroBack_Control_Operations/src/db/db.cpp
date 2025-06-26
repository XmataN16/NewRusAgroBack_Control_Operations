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
			// ��������� ���������� ����� close()
			conn_->close();
		}
		catch (const std::exception& e)
		{
			std::cerr << "[Database::~Database] ������ ��� �������� ����������: "
				<< e.what() << std::endl;
		}
	}
}

void Database::loadConfig(const std::string& config_file)
{
	try
	{
		YAML::Node config = YAML::LoadFile(config_file);

		// ��������� ������������ ����
		if (!config["host"] || !config["port"] || !config["user"] ||
			!config["password"] || !config["dbname"])
		{
			throw std::runtime_error("� ����� ������������ ����������� ������������ ����.");
		}

		host_ = config["host"].as<std::string>();
		port_ = config["port"].as<int>();
		user_ = config["user"].as<std::string>();
		password_ = config["password"].as<std::string>();
		dbname_ = config["dbname"].as<std::string>();

		// ������������ ���� sslmode
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
		throw std::runtime_error("�� ������� ������� YAML-����: " + std::string(e.what()));
	}
	catch (const YAML::ParserException& e)
	{
		throw std::runtime_error("������ �������� YAML-�����: " + std::string(e.what()));
	}
	// ������ ���������� (��������, ����������� ����) ������ ������������
}

void Database::connect()
{
	if (conn_ && conn_->is_open())
	{
		// ��� ����������, ������ �� ������
		return;
	}

	// �������� ������ ����������� � ������� libpq
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
		// ������ ����������
		conn_ = std::make_unique<pqxx::connection>(conn_str.str());

		if (!conn_->is_open())
		{
			throw std::runtime_error("�� ������� ������� ���������� � ����� ������.");
		}
	}
	catch (const pqxx::broken_connection& e)
	{
		throw std::runtime_error("������ ���������� � PostgreSQL: " + std::string(e.what()));
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error("������ ��� ������� connect(): " + std::string(e.what()));
	}
}

pqxx::connection& Database::getConnection()
{
	if (!conn_ || !conn_->is_open())
	{
		throw std::runtime_error("���������� � �� �� �����������. �������� connect() ����� ��������������.");
	}
	return *conn_;
}

pqxx::result Database::fetchInitialDataRaw()
{
	pqxx::work txn(*conn_);

	const std::string query = R"(
        SELECT 
            culture_id,
            t_material_id,
            region_id,
            season,
            region_date,
            input_operation_order,
            alternative_operation_order,
            input_deadline,
            alternative_deadline,
            noinput_deadline,
            "order",
            year
        FROM static_initial_data ORDER BY "order" ASC, culture_id ASC, region_id ASC
    )";

	pqxx::result result = txn.exec(query);
	txn.commit();
	return result;
}

pqxx::result Database::fetchSapControlOperationsRaw()
{
	pqxx::work txn(*conn_);

	const std::string query = R"(
        SELECT 
            culture_id,
            region_id,
            t_material_id,
            pu_id,
            higher_tm,
            season,
            calendar_day,
            planned_volume,
            actual_volume,
            year
        FROM sap_control_operations ORDER BY calendar_day, id
    )";

	pqxx::result result = txn.exec(query);
	txn.commit();
	return result;
}

pqxx::result Database::fetchSapIDSSeedingRaw()
{
	pqxx::work txn(*conn_);

	const std::string query = u8R"(
       SELECT 
       id 
       FROM sap_operations_manual 
       WHERE (operation_name = '����� � ��������� ��������� (��)' OR operation_name = '����� ��� �������� ��������� (��)')
    )";

	pqxx::result result = txn.exec(query);
	txn.commit();
	return result;
}

pqxx::result Database::fetchSapIDSReseedingRaw()
{
	pqxx::work txn(*conn_);

	const std::string query = u8R"(
       SELECT 
       id 
       FROM sap_operations_manual 
       WHERE (operation_name = '������� ��� �������� ��������� (��)' OR operation_name = '������� � ��������� ��������� (��)')
    )";

	pqxx::result result = txn.exec(query);
	txn.commit();
	return result;
}

