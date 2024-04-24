//
// Created by Anton Chuvashov on 24.04.2024.
//

#ifndef GAME_MAIN_CPP_TTTGAME_H_
#define GAME_MAIN_CPP_TTTGAME_H_

#include <vector>
#include <netinet/in.h>
class TTTAbstract {
 protected:
  std::vector<std::vector<char>> board;
  int socket_fd;
  struct sockaddr_in server_addr{};
  const int port = 22092004;
  char buffer[4]{};
  void UpdateBoard();

  std::pair<int, int> GetMove(int client_socket_fd);
  static void SendMove(std::pair<int, int>, int client_socket_fd) ;
  bool CheckWin(char player);
  bool Filled();
 public:
  virtual void Start() = 0;
  bool ValidMove(std::pair<int, int> move);
};

class TicTacToe : TTTAbstract {
 private:
  struct sockaddr_in client_addr{};
  int client_socket_fd;
  int client_len = sizeof(client_addr);

 public:
  void Start() override;
  TicTacToe();
  ~TicTacToe();
};
class TicTacToeClient : TTTAbstract{
 public:
  void Start() override;
  explicit TicTacToeClient(const std::string& host = "127.0.0.1");
  ~TicTacToeClient();
};
#endif //GAME_MAIN_CPP_TTTGAME_H_
