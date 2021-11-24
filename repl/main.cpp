#include <iostream>
#include <string>
#include "../scheme.h"

int main() {
    Interpreter i;
    std::string s;
    while (true) {
        try {
            std::cout << ">> ";
            std::getline(std::cin, s);
            std::cout << i.Run(s) << "\n\n";
        } catch(const std::runtime_error& e) {
            std::cout << e.what() << "\n\n";
        } 
    }
    return 0;
}
