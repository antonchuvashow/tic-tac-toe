//
// Created by Anton Chuvashov on 24.04.2024.
//

#include <cstdlib>
#include "utils.h"
void Clear() {
#if defined _WIN32
  system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
  system("clear");
#elif defined (__APPLE__)
  system("clear");
#endif
}