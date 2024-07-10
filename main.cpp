#include <iostream>
#include "manager/manager.hpp"

using lib::Manager;
using lib::types::eLoginMethod;

int main() {
    Manager manager{};
    manager.add("peroperod", "", eLoginMethod::LEGACY_SIGNIN);
    return 0;
}