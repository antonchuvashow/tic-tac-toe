//
// Created by Anton Chuvashov on 24.04.2024.
//

#include "utils.h"
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <iostream>
#include "TTTGame.h"

bool TTTAbstract::ValidMove(std::pair<int, int> move) {
  if (move.first >= 3 || move.second >= 3 || board[move.first][move.second] != ' ') {
    std::cout << "That's unfair! Choose correct coordinates!\n";
    return false;
  }

  return true;
}

void TTTAbstract::UpdateBoard() {
  Clear();
  std::cout << "-------------\n";
  for (int i = 0; i < 3; i++) {
    std::cout << "| ";
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == ' ')
        std::cout << (j + 1) + 3*i << " | ";
      else
        std::cout << board[i][j] << " | ";
    }
    std::cout << "\n-------------\n";
  }
}
void TTTAbstract::SendMove(std::pair<int, int> move, int client_socket_fd) {
  std::string move_str = std::to_string(move.first) + ' ' + std::to_string(move.second);
  send(client_socket_fd, move_str.c_str(), move_str.length(), 0);
}
std::pair<int, int> TTTAbstract::GetMove(int client_socket_fd) {
  size_t n = read(client_socket_fd, buffer, 255);
  buffer[n] = '\0';
  std::string move_str(buffer);
  size_t comma_pos = move_str.find(' ');
  int row = std::stoi(move_str.substr(0, comma_pos));
  int col = std::stoi(move_str.substr(comma_pos + 1));
  return std::make_pair(row, col);
}
bool TTTAbstract::CheckWin(char player) {
  for (int i = 0; i < 3; i++) {
    if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
      return true;
    }
  }
  for (int i = 0; i < 3; i++) {
    if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
      return true;
    }
  }
  if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
    return true;
  }
  if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
    return true;
  }
  return false;
}
bool TTTAbstract::Filled() {
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (board[i][j] == ' ') {
        return false;
      }
    }
  }
  return true;
}

TicTacToe::TicTacToe() {
  board = std::vector<std::vector<char> >(3, std::vector<char>(3, ' '));

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    std::cerr << "Error: Opening socket\n";
    exit(1);
  }

  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);
  if (bind(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Error: Binding\n";
    exit(1);
  }

  listen(socket_fd, 2);

  client_socket_fd = accept(socket_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_len);
  if (client_socket_fd < 0) {
    std::cerr << "Error: Accepting connection\n";
    exit(1);
  }
}

TicTacToe::~TicTacToe() {
  close(socket_fd);
  close(client_socket_fd);
}

void TicTacToe::Start() {
  while (true) {
    std::pair<int, int> move;

    UpdateBoard();
    do {
      std::cout << "Enter cell number for your move(e.g., 1): ";
      int value;
      std::cin >> value;
      move.first = (value - 1) / 3;
      move.second = value - move.first * 3 - 1;
    } while (!ValidMove(move));

    board[move.first][move.second] = 'X';
    SendMove(move, client_socket_fd);
    UpdateBoard();

    if (CheckWin('X')) {
      std::cout << "X won in this game! Congratulations!";
      break;
    } else if (CheckWin('O')) {
      std::cout << "O won in this game... I am sure you did your best, I am sorry :(";
      break;
    } else if (Filled()) {
      std::cout << "Oh it seems to be Draw. I am sorry ;(";
      break;
    }

    move = GetMove(client_socket_fd);
    board[move.first][move.second] = 'O';
    UpdateBoard();

    if (CheckWin('X')) {
      std::cout << "X won in this game! Congratulations!";
      break;
    } else if (CheckWin('O')) {
      std::cout << "O won in this game... I am sure you did your best, I am sorry :(";
      break;
    } else if (Filled()) {
      std::cout << "Oh it seems to be Draw. I am sorry ;(";
      break;
    }
  }
}

TicTacToeClient::TicTacToeClient(const std::string &host) {
  board = std::vector<std::vector<char>>(3, std::vector<char>(3, ' '));

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    std::cerr << "Error: Opening socket\n";
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(host.c_str());
  server_addr.sin_port = htons(port);

  if (connect(socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Error: Connection failed\n";
    exit(1);
  }
}

TicTacToeClient::~TicTacToeClient() {
  close(socket_fd);
}

void TicTacToeClient::Start() {
  while (true) {
    std::pair<int, int> move;

    move = GetMove(socket_fd);
    board[move.first][move.second] = 'X';
    UpdateBoard();

    if (CheckWin('O')) {
      std::cout << "O won in this game! Congratulations!";
      break;
    } else if (CheckWin('X')) {
      std::cout << "X won in this game... I am sure you did your best, I am sorry :(";
      break;
    } else if (Filled()) {
      std::cout << "Oh it seems to be Draw. I am sorry ;(";
      break;
    }

    do {
      std::cout << "Enter cell number for your move(e.g., 1): ";
      int value;
      std::cin >> value;
      move.first = (value - 1) / 3;
      move.second = value - move.first * 3 - 1;
    } while (!ValidMove(move));

    board[move.first][move.second] = 'O';
    SendMove(move, socket_fd);
    UpdateBoard();

    if (CheckWin('O')) {
      std::cout << "O won in this game! Congratulations!";
      break;
    } else if (CheckWin('X')) {
      std::cout << "X won in this game... I am sure you did your best, I am sorry :(";
      break;
    } else if (Filled()) {
      std::cout << "Oh it seems to be Draw. I am sorry ;(";
      break;
    }
  }
}