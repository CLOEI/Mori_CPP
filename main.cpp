#include <iostream>
#include "lib/manager/manager.hpp"

using namespace lib::{Manager, eLoginMethod};

int main()
{
  Manager manager{};
  manager.add("peroperod", "", eLoginMethod::LEGACY_SIGNIN);
  return 0;
}