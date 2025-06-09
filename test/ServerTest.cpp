#include <gtest/gtest.h>
#include "Server.hpp"
#include "nginx.hpp"

// テスト用にコンフィグを作る
ServerConfig makeTestServerConfig() {
    ServerConfig config;
    config.setPort(8080);
    config.setHost("127.0.0.1");
    config.setServerName("localhost");
    config.setClientMaxBodySize(10000);
    return config;
}

// Serverクラスのテスト
class ServerTest : public ::testing::Test {
protected:
    Server* server;
    std::vector<ServerConfig> configs;

    void SetUp() override {
        configs.push_back(makeTestServerConfig());
        server = new Server(configs);
    }

    void TearDown() override {
        delete server;
    }
};

// ---- setConf / getConf テスト ----
TEST_F(ServerTest, ConfSetAndGet) {
    std::vector<ServerConfig> newConfigs;
    ServerConfig newConfig;
    newConfig.setPort(8000);
    newConfigs.push_back(newConfig);

    server->setConf(newConfigs);
    auto conf = server->getConf();

    ASSERT_EQ(conf.size(), 1);
    EXPECT_EQ(conf[0].getPort(), 8000);
}

// ---- set_nonblocking 正常ケース ----
TEST_F(ServerTest, SetNonBlockingFd) {
    int fds[2];
    ASSERT_EQ(pipe(fds), 0);

    EXPECT_NO_THROW({
        set_nonblocking(fds[0]);
    });

    close(fds[0]);
    close(fds[1]);
}

// ---- epollCreate 正常ケース ----
TEST_F(ServerTest, EpollCreateTest) {
    // listen socket作成は別途
    server->createListenServer();
    EXPECT_GE(server->getEpollFd(), 0); // 0以上なら epoll_create 成功
}

// ---- createListenServer でListenSocketが作られるか ----
TEST_F(ServerTest, CreateListenServerCreatesSocket) {
    server->createListenServer();
    // ソケットが最低1個は作成されているか確認
    EXPECT_FALSE(server->getSocketMap().empty());
}

// ---- Destructorでソケットがクローズされるか（リークチェック） ----
// Destructorの動作はgtestでは検証難しいので、valgrindなど外部ツールで

