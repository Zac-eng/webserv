cmake_minimum_required(VERSION 3.10)
project(WEBSERV_TEST CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Testのインクルードパスを設定
include_directories(/usr/local/include)
# ソースコードのインクルードパスを追加
include_directories(${CMAKE_SOURCE_DIR}/includes)

# Google Testのライブラリパスを設定
link_directories(/usr/local/lib)

# ソースコードのファイルを指定
file(GLOB SOURCES "src/*.cpp")
file(GLOB TEST_SOURCE "test/*.cpp")

# main.cppをテストから除外
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# テスト用実行ファイルを追加
add_executable(WEBSERV_TEST ${SOURCES} ${TEST_SOURCE})

# Google Testライブラリをリンク
target_link_libraries(WEBSERV_TEST gtest gtest_main)
