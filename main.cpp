#include <iostream>
#include <fstream>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <any>
#include <regex>

struct Variable
{
    std::string name;
    std::string type;
    std::string value;
};

std::any variant_to_any(const std::variant<int, double, float, std::string, bool>& var) {
    return std::visit([](auto&& arg) -> std::any { return arg; }, var);
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::variant<int, double, float, std::string, bool> SpecifyDataType(Variable Var)
{
    // Type casting
    if (Var.type == "int") {
        return std::stoi(Var.value);
    } else if (Var.type == "double") {
        return std::stod(Var.value);
    } else if (Var.type == "float") {
        return std::stof(Var.value);
    } else if (Var.type == "string") {
        return Var.value;
    } else if (Var.type == "bool") {
        bool value;
        if(Var.value == "true" || Var.value == "1" || Var.value == "TRUE") {
            value = true;
        } else if (Var.value == "false" || Var.value == "0" || Var.value == "FALSE") {
            value = false;
        } else {
            std::cout << "bool type should be 'true' or 'false'.\n"
                         "correct usage --> 'true', '1' or 'TRUE' for true "
                         "and 'false', '0' or 'FALSE' for false\n";
            exit(1);
        }
        return value;
    } else {
        std::cerr << Var.name << ":" << Var.type << " datatype is unknown\n";
        exit(1);
    }
}

std::unordered_map<std::string, std::any> Interprete(std::vector<Variable> Variables)
{
    std::unordered_map<std::string, std::any> CastedVariables;

    std::regex ArrVecPattern("\\b(\\w+)<(\\w+)>");
    std::smatch MatchArrVec;

    for (const auto& Var : Variables) {
        if (Var.type.compare(0, 5, "array") == 0 || Var.type.compare(0, 6, "vector") == 0) {
            if (std::regex_search(Var.type, MatchArrVec, ArrVecPattern)) {
                std::string elementType = MatchArrVec[2];

                std::regex number_pattern(R"((\btrue\b|\bfalse\b|\bTRUE\b|\bFALSE\b|\b\d+\.\d+|\b\d+|"(?:[^"\\]|\\.)*"))");
                std::sregex_iterator begin(Var.value.begin(), Var.value.end(), number_pattern);
                std::sregex_iterator end;
                std::vector<std::string> values;
                for(std::sregex_iterator it = begin; it != end; ++it) {
                    values.push_back(it->str());
                }
                std::cout << "(";
                for (const auto& s : values) {
                    std::cout << s << ",";      // debug vector-array value
                }
                std::cout << ")\n";

                if (elementType == "int") {
                    std::vector<int> VectorInt;
                    for (const auto &element : values)
                        VectorInt.push_back(std::stoi(element));
                    CastedVariables[Var.name] = VectorInt;
                } else if (elementType == "double") {
                    std::vector<double> VectorDouble;
                    for (const auto &element : values)
                        VectorDouble.push_back(std::stod(element));
                    CastedVariables[Var.name] = VectorDouble;
                } else if (elementType == "float") {
                    std::vector<float> VectorFloat;
                    for (const auto &element : values)
                        VectorFloat.push_back(std::stof(element));
                    CastedVariables[Var.name] = VectorFloat;
                } else if (elementType == "string") {
                    std::vector<std::string> VectorString;
                    for (const auto &element : values) {
                        std::string StrValue = element;
                        if (StrValue.front() == '\"' && StrValue.back() == '\"') {
                                StrValue = StrValue.substr(1, StrValue.size() - 2);
                            }
                        VectorString.push_back(StrValue);
                    }
                    CastedVariables[Var.name] = VectorString;
                } else if (elementType == "bool") {
                    std::vector<bool> VectorBool;
                    for (const auto& element : values) {
                        bool BoolValue;
                        if(element == "true" || element == "1" || element == "TRUE") {
                            BoolValue = true;
                        } else if (element == "false" || element == "0" || element == "FALSE") {
                            BoolValue = false;
                        } else {
                            std::cout << "bool type should be 'true' or 'false'.\n"
                                         "correct usage --> 'true', '1' or 'TRUE' for true "
                                         "and 'false', '0' or 'FALSE' for false\n";
                            exit(1);
                        }
                        VectorBool.push_back(BoolValue);
                    }
                    CastedVariables[Var.name] = VectorBool;
                }
            } else {
                std::cerr << "array or vector format is incorrect\n";
                exit(1);
            }
        } else {
            CastedVariables[Var.name] = variant_to_any(SpecifyDataType(Var));
        }
    }
    return CastedVariables;
}

int
main()
{
    std::ifstream file("config-txt.txt");
    if (!file.is_open()) {
        throw std::runtime_error("File not found");
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::regex var_pattern(R"(\*(\w+):\s*TYPE\s*=\s*([\w\<\>\,\s]+)\s*VALUE\s*=\s*([^\*]+))"), ArrVecPattern("\\b(\\w+)<(\\w+)>");
    std::smatch MatchVar, MatchArrVec;

    std::vector<Variable> Variables;

    while (std::regex_search(content, MatchVar, var_pattern)) {
        Variable var{trim(MatchVar[1].str()), trim(MatchVar[2].str()), trim(MatchVar[3].str())};
        Variables.push_back(var);
        std::cout << var.type << "\n";  //debug type
        content = MatchVar.suffix().str();
    }

    std::unordered_map<std::string, std::any> CastedVariables;
    CastedVariables = Interprete(Variables);

//    for(const auto& [key, value] : CastedVariables) {
//        std::cout << "Key: " << key << ", Type: " << value.type().name() << '\n';
//    }

    try {
        std::cout << "Distance: " << std::any_cast<int>(CastedVariables["Distance"]) << "\n";
        std::cout << "Path: " << std::any_cast<std::string>(CastedVariables["Path"]) << "\n";
        std::cout << "Range: " << std::any_cast<double>(CastedVariables["Range"]) << "\n";
        std::cout << "Test: " << std::any_cast<float>(CastedVariables["Test"]) << "\n";
        std::cout << "isValid: " << std::any_cast<bool>(CastedVariables["isValid"]) << "\n";

        std::vector<std::string> VectorString = std::any_cast<std::vector<std::string>>(CastedVariables["VectorString"]);
        std::vector<bool> VectorBool = std::any_cast<std::vector<bool>>(CastedVariables["VectorBool"]);
        std::vector<double> VectorDouble = std::any_cast<std::vector<double>>(CastedVariables["VectorDouble"]);
        std::vector<float> VectorFloat = std::any_cast<std::vector<float>>(CastedVariables["VectorFloat"]);
        std::vector<int> VectorInt = std::any_cast<std::vector<int>>(CastedVariables["VectorInt"]);

        std::cout << "VectorString:\n";
        for (const auto& elem : VectorString) {
            std::cout << elem << "\n";      // debug vector-array value
        }
        std::cout << "\n";

        std::cout << "VectorBool:\n";
        for (const auto& elem : VectorBool) {
            std::cout << elem << "\n";      // debug vector-array value
        }
        std::cout << "\n";

        std::cout << "VectorDouble:\n";
        for (const auto& elem : VectorDouble) {
            std::cout << elem << "\n";      // debug vector-array value
        }
        std::cout << "\n";

        std::cout << "VectorFloat:\n";
        for (const auto& elem : VectorFloat) {
            std::cout << elem << "\n";      // debug vector-array value
        }
        std::cout << "\n";

        std::cout << "VectorInt:\n";
        for (const auto& elem : VectorInt) {
            std::cout << elem << "\n";      // debug vector-array value
        }
        std::cout << "\n";

    } catch (const std::bad_any_cast& e) {
        std::cout << "Failed to cast variable: " << e.what() << "\n";
    }

    return 0;
}
