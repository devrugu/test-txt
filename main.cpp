#include "TxtConfigParser.h"

int
main()
{
    try {
        TxtConfigParser configParser;
        std::unordered_map<std::string, std::any> CastedVariables = configParser.ParseConfigTxt("/home/operator/config-files/config-txts/config-txt.txt");

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
