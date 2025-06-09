#include <gtest/gtest.h>
#include "Request.hpp" // Requestクラスのヘッダ

class RequestTest : public ::testing::Test {
protected:
    Request req;

    void SetUp() override {
        // 何かセットアップが必要ならここに書く
    }

    void TearDown() override {
        // 何かクリーンアップが必要ならここに書く
    }
};

// --- 文字列を小文字にする ---
TEST_F(RequestTest, ConvertLowerTest) {
    std::string key = "ConTENT-TyPE";
    convertLower(key);
    EXPECT_EQ(key, "content-type");
}

// --- メソッド解析のテスト ---
TEST_F(RequestTest, ParseMethodValid) {
std::string request = "GET / HTTP/1.1";
std::string::const_iterator it = request.begin();  // ← ★ const_iteratorで宣言
EXPECT_TRUE(req.ParseMethod(request, it));

}


// --- URI解析テスト ---
TEST_F(RequestTest, ParseUriValid) {
    std::string request = "/index.html";
    EXPECT_TRUE(req.ValidUri(request));
    EXPECT_EQ(req.getDirectory(), "/");
    EXPECT_EQ(req.getFile(), "index.html");
    EXPECT_EQ(req.getExtension(), ".html");
}

// --- HTTPバージョン解析テスト ---
TEST_F(RequestTest, ParseVersionValid) {
    const std::string request = "HTTP/1.1";
    auto it = request.begin();
    EXPECT_TRUE(req.ParseVersion(request, it));
}

// --- Hostヘッダー解析テスト ---
TEST_F(RequestTest, ParseHostHeader) {
    std::string request = "Host: example.com\r\n";
    EXPECT_TRUE(req.parseHeader(request));
    EXPECT_TRUE(req.getHostFlag());
    EXPECT_EQ(req.getHeader().at("host"), "example.com");
}

// --- Content-Lengthヘッダーの解析 ---
TEST_F(RequestTest, ParseContentLengthHeader) {
    std::string request = "Content-Length: 10\r\n";
    EXPECT_TRUE(req.parseHeader(request));
    EXPECT_EQ(req.getBodySize(), 10);
}

// --- Transfer-Encoding chunkedの解析 ---
TEST_F(RequestTest, ParseTransferEncodingChunked) {
    std::string request = "Transfer-Encoding: chunked\r\n";
    EXPECT_TRUE(req.parseHeader(request));
    EXPECT_TRUE(req.getChunkFlag());
}

// --- connection: close 解析 ---
TEST_F(RequestTest, ParseConnectionClose) {
    std::string request = "Connection: close\r\n";
    EXPECT_TRUE(req.parseHeader(request));
    EXPECT_TRUE(req.getConnectionFlag());
}

// --- Chunkサイズ解析（成功ケース） ---
TEST_F(RequestTest, ParseChunkSizeSuccess) {
    std::string chunkHeader = "5\r\n";
    EXPECT_TRUE(req.parseChunkSize(chunkHeader));
}

// --- Chunkボディ解析（成功ケース） ---
TEST_F(RequestTest, ParseChunkValueSuccess) {
    req.setChunkSize(5); // セッターでセット
    std::string chunkBody = "hello\r\n";
    EXPECT_TRUE(req.parseChunkValue(chunkBody));
}

// --- POSTボディパース ---
TEST_F(RequestTest, ParsePostBody) {
    req.setBodySize(5);
    EXPECT_TRUE(req.parsePostBody("hello"));
}

// --- リクエストラインパース ---
TEST_F(RequestTest, ParseRequestLineSuccess) {
    std::string requestLine = "GET /index.html HTTP/1.1";
    EXPECT_TRUE(req.ParseRequestLine(requestLine));
}

// --- Bodyパース(普通のPOST) ---
TEST_F(RequestTest, ParseBodySuccess) {
    std::string request = "body\r\n";
    EXPECT_TRUE(req.ParseBody(request));
}

// --- Chunk全体パース ---
TEST_F(RequestTest, ExecuteChunkSizeThenValue) {
    std::string size = "5\r\n";
    EXPECT_TRUE(req.executeChunk(size));

    req.setChunkSize(5); // 必要ならセット
    std::string value = "hello\r\n";
    EXPECT_TRUE(req.executeChunk(value));
}

// --- Multipart boundaryパース（成功ケース） ---
TEST_F(RequestTest, ParseMultipartSuccess) {
    req.setBoundary("--boundary");
    req.setBodySize(36); // boundaryだけの長さ2回分

    std::string start_boundary = "--boundary\r\n";
    std::string end_boundary = "--boundary--\r\n";

    EXPECT_NO_THROW(req.parseMultipart(start_boundary));
    EXPECT_NO_THROW(req.parseMultipart(end_boundary));
}

// --- リクエストオールパース ---
TEST_F(RequestTest, ParseWholeRequest) {
    std::string requestLine = "GET /index.html HTTP/1.1";
    EXPECT_TRUE(req.ParseRequest(requestLine, false));
}
