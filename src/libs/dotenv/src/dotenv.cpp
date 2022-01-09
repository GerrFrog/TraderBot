#include <cstdlib>
#include <fstream>
#include <vector>
#include "../include/dotenv.hpp"

namespace dotenv {

std::string get(const std::string &name, const std::string &default_value)
{
#ifdef _WIN32
    size_t size = 0;
    getenv_s(&size, nullptr, 0, name.c_str());
    if (size > 0) {
        std::string value(size - 1, '\0');
        getenv_s(&size, &value[0], size, name.c_str());
        return value;
    } else {
        return default_value;
    }
#else
    const char *value = getenv(name.c_str());
    if (value != nullptr) {
        return std::string(value);
    } else {
        return default_value;
    }
#endif
}

void set(const std::string &name, const std::string &value)
{
#ifdef _WIN32
    auto var = name + '=' + value;
    (void)_putenv(var.c_str());
#else
    setenv(name.c_str(), value.c_str(), true);
#endif
}

void unset(const std::string &name)
{
#ifdef _WIN32
    auto var = name + '=';
    (void)_putenv(var.c_str());
#else
    unsetenv(name.c_str());
#endif
}

void load(const std::string &filename)
{
    std::ifstream stream(filename);
    std::string line;

    while (std::getline(stream, line)) {
        auto sep_pos = line.find('=');
        if (sep_pos == std::string::npos) {
            continue;
        }
        set(line.substr(0, sep_pos), line.substr(sep_pos + 1));
    }
}

} // namespace dotenv
