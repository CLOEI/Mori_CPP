#include "manager/manager.hpp"
#include "connect/connect.hpp"

using lib::Connect;
using lib::Manager;
using lib::types::eLoginMethod;

int main()
{
    spdlog::info("Getting oauth links");
    Connect::get_oauth_link();
    Manager manager{};
    manager.add("peroperod", "", eLoginMethod::LEGACY_SIGNIN);
    return 0;
}
