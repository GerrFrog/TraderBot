#ifndef DOTENV_H
#define DOTENV_H

#include <string>

namespace dotenv {

std::string get(const std::string &name,
                const std::string &default_value = "");

void set(const std::string &name, const std::string &value);
void unset(const std::string &name);

void load(const std::string &filename = ".env");

} // namespace dotenv

#endif
