#include "Server.hpp"
#include "ASocket.hpp"
#include "ListenSocket.hpp"
#include "CgiSocket.hpp"
#include "ClientSocket.hpp"
#include "Request.hpp"
#include "Response.hpp"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return 1;
  }
  return 0;
  try {
    // 設定ファイルの情報をもらう
    ParseConfig();
    ASocket listen_socket;
    CreateListenServer();
    ExecuteServer();
  }
  catch (std::exception& e)
  {
    std::cerr << "エラーが発生しました。"<< e.what() << std::endl;
  }
}
