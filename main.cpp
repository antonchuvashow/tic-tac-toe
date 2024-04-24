#include <iostream>
#include "TTTGame.h"

int main() {
  std::cout << "1) Start a new game\n2) Connect to an existing game\n";
  int choice;
  std::cin >> choice;

  switch(choice) {
    case 1: {
      std::cout << "Waiting for another player to connect...\n";
      TicTacToe game;
      game.Start();
      break;
    }
    case 2: {
      std::string host;
      std::cout << "Enter the host: ";
      std::cin >> host;
      TicTacToeClient game(host);
      game.Start();
      break;
    }
    default: {
      std::cout << "Invalid choice. Exiting...\n";
      break;
    }
  }

  return 0;
}
