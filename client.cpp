#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include <string>

void Clear() {
#if defined _WIN32
  system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
  system("clear");
#elif defined (__APPLE__)
  system("clear");
#endif
}


int main() {
  auto game = TicTacToeClient();
  game.Start();
  return 0;
}
