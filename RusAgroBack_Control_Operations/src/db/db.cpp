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
        FROM static_initial_data_control_operations ORDER BY "order" ASC, culture_id ASC, region_id ASC
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
        FROM sap_control_operations_source ORDER BY calendar_day, id
    )";

	pqxx::result result = txn.exec(query);
	txn.commit();
	return result;
}

pqxx::result Database::fetchSapSugarBeetSawingDatesOpsRaw()
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
        FROM sap_sugar_beet_sawing_dates_ops_source ORDER BY calendar_day, id
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
       FROM sap_operations 
       WHERE (operation_name ILIKE '%Посев с%' OR operation_name ILIKE '%Посев без%')
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
       FROM sap_operations 
       WHERE (operation_name ILIKE '%Пересев с%' OR operation_name ILIKE '%Пересев без%')
    )";

	pqxx::result result = txn.exec(query);
	txn.commit();
	return result;
}

void Database::insertIntoControlOperationsAggregated(const YearSlices& uniqueSlices)
{
	pqxx::work txn(*conn_);
	try
	{
		// Подготовка SQL-запроса с параметрами
		const std::string query = R"(
            INSERT INTO control_operations_aggregated_new (
                culture_id, region_id, t_material_id, pu_id, higher_tm, season, calendar_day, year,
                actual_date, start_date, is_completed, is_started, sawing_date, resawing_date,
                status, is_actual, minimal_date, "order"
            ) VALUES (
                $1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18
            )
        )";

		// Обход всех записей в YearSlices
		for (const auto& [year, higherTmMap] : uniqueSlices) 
		{
			for (const auto& [higherTm, materialSlices] : higherTmMap) 
			{
				for (const auto& slice : materialSlices) 
				{
					for (const auto& frame : slice) 
					{
						pqxx::params params;

						// Добавление обязательных полей
						params.append(frame.culture_id);
						params.append(frame.region_id);
						params.append(frame.t_material_id);
						params.append(frame.pu_id);
						params.append(higherTm);
						params.append(frame.season);
						params.append(to_iso_string(frame.calendar_day));
						params.append(year);

						// Добавление опциональных полей
						if (frame.actual_date.has_value())
						{
							params.append(to_iso_string(*frame.actual_date));
						}
						else
						{
							params.append(std::nullopt);
						}
						if (frame.start_date.has_value())
						{
							params.append(to_iso_string(*frame.start_date));
						}
						else
						{
							params.append(std::nullopt);
						}

						params.append(frame.is_completed);
						params.append(frame.is_started);

						if (frame.sawing_date.has_value())
						{
							params.append(to_iso_string(*frame.sawing_date));
						}
						else
						{
							params.append(std::nullopt);
						}
						if (frame.resawing_date.has_value())
						{
							params.append(to_iso_string(*frame.resawing_date));
						}
						else
						{
							params.append(std::nullopt);
						}
						if (frame.status.has_value())
						{
							params.append(frame.status);
						}
						else
						{
							params.append(std::nullopt);
						}
						if (frame.is_actual.has_value())
						{
							params.append(frame.is_actual);
						}
						else
						{
							params.append(std::nullopt);
						}
						if (frame.minimal_date.has_value())
						{
							params.append(to_iso_string(*frame.minimal_date));
						}
						else
						{
							params.append(std::nullopt);
						}
						if (frame.order.has_value())
						{
							params.append(*frame.order);
						}
						else
						{
							params.append(std::nullopt);
						}

						// Выполнение вставки
						txn.exec(query, params);
					}
				}
			}
		}

		// Подтверждение транзакции
		txn.commit();
	}
	catch (const std::exception& e)
	{
		// Откат в случае ошибки
		txn.abort();
		throw; // Перебрасываем исключение для обработки на верхнем уровне
	}
}

void Database::insertIntoSapSugarBeetSawingDates(const YearSlices& uniqueSlices)
{
	pqxx::work txn(*conn_);
	try
	{
		const std::string query = R"(
            INSERT INTO sap_sugar_beet_sawing_dates
                (culture_id, pu_id, higher_tm, year, sawing_date)
            VALUES ($1, $2, $3, $4, $5)
        )";

		// Обход YearSlices -> map<year, map<higher_tm, vector<vector<Frame>>>>
		for (const auto& [year, higherTmMap] : uniqueSlices)
		{
			for (const auto& [higherTm, materialSlices] : higherTmMap)
			{
				for (const auto& slice : materialSlices)
				{
					for (const auto& frame : slice)
					{
						pqxx::params params;

						params.append(frame.culture_id);
						params.append(frame.pu_id);
						params.append(higherTm);  // строка
						params.append(year);

						if (frame.sawing_date.has_value())
						{
							params.append(to_iso_string(*frame.sawing_date));
						}
						else
						{
							params.append(std::nullopt);
						}

						txn.exec(query, params);
					}
				}
			}
		}

		txn.commit();
	}
	catch (const std::exception& e)
	{
		txn.abort();
		throw; // пробрасываем выше
	}
}


void Database::truncateAndRestartIdentity(std::string table_name)
{
	pqxx::work txn(*conn_);

	const std::string query = u8R"(TRUNCATE TABLE )" + table_name + u8R"( RESTART IDENTITY)";

	txn.exec(query);
	txn.commit();
}

