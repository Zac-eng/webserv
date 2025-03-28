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
    std::vector<ServerConfig> conf;
    Server server;
    // 設定ファイルの情報をもらう
    conf = ParseConfig();
    server.CreateListenServer(conf);
    server.ExecuteServer();
  }
  catch (std::exception& e)
  {
    std::cerr << "エラーが発生しました。"<< e.what() << std::endl;
  }
}
