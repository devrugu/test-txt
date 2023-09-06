#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <any>
#include <regex>

struct Variable {
    std::string name;
    std::string type;
    std::string value;
};

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

int main() {
    std::ifstream file("config-txt.txt");
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::regex var_pattern(R"(\*(\w+):\s*TYPE\s*=\s*([\w\<\>\,\s]+)\s*VALUE\s*=\s*([^\*]+))");
    std::smatch m;

    std::vector<Variable> Variables;
    std::unordered_map<std::string, std::any> CastedVariables;

    while (std::regex_search(content, m, var_pattern)) {
        Variable var{trim(m[1].str()), trim(m[2].str()), trim(m[3].str())};
        Variables.push_back(var);

        std::cout << var.type << "\n";  //debug type

        // Type casting
        if (var.type == "int") {
            CastedVariables[var.name] = std::stoi(var.value);
        } else if (var.type == "double") {
            CastedVariables[var.name] = std::stod(var.value);
        } else if (var.type == "float") {
            CastedVariables[var.name] = std::stof(var.value);
        } else if (var.type == "string") {
            CastedVariables[var.name] = var.value;
        }
        content = m.suffix().str();
    }

    try {
        std::cout << "Distance: " << std::any_cast<int>(CastedVariables["Distance"]) << "\n";
        std::cout << "Path: " << std::any_cast<std::string>(CastedVariables["Path"]) << "\n";
        std::cout << "Range: " << std::any_cast<double>(CastedVariables["Range"]) << "\n";
        std::cout << "Test: " << std::any_cast<float>(CastedVariables["Test"]) << "\n";
    } catch (const std::bad_any_cast& e) {
        std::cout << "Failed to cast variable: " << e.what() << "\n";
    }

    return 0;
}
