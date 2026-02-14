# RusAgroBack_Control_Operations

**Кратко:** это C++-утилита для расчёта сроков и статусов технологических операций на больших объёмах (миллионы записей). Получает/читает данные из базы (Postgres), выполняет расчёты (модуль `SapControlTech` и прочее) и сохраняет результаты (в том числе массовыми вставками). Проект ориентирован на кроссплатформенную сборку (Windows / Linux) через CMake.

## Что делает программа
- Парсит входные данные из PostgreSQL (через libpqxx).
- Выполняет расчёты дат/статусов операций (модуль `SapControlTech` и др.).
- Записывает агрегированные результаты в БД быстрыми bulk-операциями.

## Ключевые возможности
- Быстрые bulk INSERT (pqxx::stream_to / COPY).
- Параллельные вычисления с per-thread DB connections.
- Конфигурация через `config/db_config.yml` (файл с секретами **не** хранится в репозитории).

## Структура репозитория
```
/RusAgroBack_Control_Operations
  /src                # исходники .cpp .hpp
  /include            # публичные заголовки (если есть)
  /config             # config/db_config.yml (локально), db_config.yml.example (в репо)
  /logs               # ротация логов (ignored)
  CMakeLists.txt
  README.md
```

## Требования
- CMake >= 3.18
- vcpkg (рекомендовано для Windows) или системные пакеты на Linux
- Visual Studio 2022 (MSVC) или GCC/Clang на Linux
- libpqxx (Postgres C++ client)
- Boost (component `date_time`)
- spdlog
- yaml-cpp (опционально, если используется)

## Конфигурация
В репозитории хранится только `config/db_config.yml.example`.  
Скопируйте пример и заполните реальные значения:
```bash
cp config/db_config.yml.example config/db_config.yml
# затем отредактируйте config/db_config.yml
```
Файл `config/db_config.yml` автоматически копируется в папку с бинарником при сборке (см. CMake).

## Сборка — Linux (пример)
```bash
sudo apt update
sudo apt install build-essential cmake libboost-date-time-dev libpqxx-dev libyaml-cpp-dev libssl-dev pkg-config

mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

## Сборка — Windows (MSVC + vcpkg)
1. Установите vcpkg и пакеты:
```powershell
git clone https://github.com/microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install spdlog libpqxx boost-date-time yaml-cpp
```
2. Сборка:
```powershell
mkdir build; cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake -A x64 -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```

## Запуск
Запустите из каталога с бинарником:
```bash
./RusAgroBack_Control_Operations -full
# или
./RusAgroBack_Control_Operations -sugar_beet_sawing
```

## Безопасность и развертывание
- Не храните пароли в репозитории — используйте CI secrets или KeyVault.
- На продакшене создавайте `config/db_config.yml` во время деплоя.
- Для больших загрузок используйте временные таблицы и индексацию после загрузки.

## Troubleshooting
- Если при запуске не хватает DLL: добавьте путь vcpkg `.../vcpkg_installed/x64-windows/bin` в PATH.
- Ошибка подключения к Postgres: проверьте `host/port/sslmode` в `config/db_config.yml`.
- libpqxx не найден при конфигурации: установите dev-пакет libpqxx.