#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>

class ini_parser {
public:
    ini_parser(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Невозможно открыть файл: " + filename);
        }

        std::string line, current_section;
        while (std::getline(file, line)) {
            line = trim(line);
            if (line.empty() || line[0] == ';') {
                continue; 
            }

            if (line.front() == '[' && line.back() == ']') {
                current_section = trim(line.substr(1, line.size() - 2));
                continue;
            }

            auto pos = line.find('=');
            if (pos == std::string::npos) {
                throw std::runtime_error("Неверный формат: " + line);
            }

            std::string key = trim(line.substr(0, pos));
            std::string value = trim(line.substr(pos + 1));
            data_[current_section][key] = value;
        }
    }

    template <typename T>
    T get_value(const std::string& section, const std::string& key) const {
        auto sec_it = data_.find(section);
        if (sec_it == data_.end()) {
            throw std::runtime_error("Секция не найдена: " + section);
        }

        auto key_it = sec_it->second.find(key);
        if (key_it == sec_it->second.end()) {
            throw std::runtime_error("Ключ не найден: " + key);
        }

        return convert<T>(key_it->second);
    }

private:
    std::map<std::string, std::map<std::string, std::string>> data_;

    static std::string trim(const std::string& str) {
        const char* whitespace = " \t\n\r";
        auto start = str.find_first_not_of(whitespace);
        auto end = str.find_last_not_of(whitespace);
        return start == std::string::npos ? "" : str.substr(start, end - start + 1);
    }

    template <typename T>
    static T convert(const std::string& value);

    template <>
    static int convert<int>(const std::string& value) {
        return std::stoi(value);
    }

    template <>
    static double convert<double>(const std::string& value) {
        return std::stod(value);
    }

    template <>
    static std::string convert<std::string>(const std::string& value) {
        return value;
    }
};

int main() {
    std::setlocale(LC_ALL, "ru_RU");

    try {
        ini_parser parser("C:\\Users\\Кристина\\Desktop\\VS\\Parcer_Course_Project\\test.ini");
        int intValue = parser.get_value<int>("Section1", "var1");
        std::string stringValue = parser.get_value<std::string>("Section1", "var2");
        double doubleValue = parser.get_value<double>("Section1", "var1");

        std::cout << "Section1.var1 (int): " << intValue << std::endl;
        std::cout << "Section1.var2 (string): " << stringValue << std::endl;
        std::cout << "Section1.var1 (double): " << doubleValue << std::endl;
    }
    catch (const std::exception& ex) {
        std::cerr << "Ошибка: " << ex.what() << std::endl;
    }

    return 0;
}
