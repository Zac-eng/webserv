// ClientSocketTest.cpp
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include "ClientSocket.hpp" // 君のクラスヘッダ
#include "Location.hpp"
#include "nginx.hpp"
#include "Request.hpp"
#include "Response.hpp"

class ClientSocketTest : public ::testing::Test {
protected:
    ClientSocket* clientSocket;

    void SetUp() override {
        clientSocket = new ClientSocket();
        // 必要ならRequest, Responseも初期化する
    }

    void TearDown() override {
        delete clientSocket;
    }
};


