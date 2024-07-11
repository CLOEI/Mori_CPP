#include "manager/manager.hpp"
#include "connect/connect.hpp"

using lib::Connect;
using lib::Manager;
using lib::types::eLoginMethod;

int main()
{
    spdlog::info("Getting oauth links");
    if (Connect::get_oauth_link() != 0)
    {
        spdlog::error("Failed to get oauth links");
        return 1;
    }
    Manager manager{};
    manager.add("Peroperod", "", eLoginMethod::LEGACY_SIGNIN);
    return 0;
}
