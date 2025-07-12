#include <boost/asio.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <vector>

namespace ba = boost::asio;
using namespace std;

// Глобальные переменные для адреса и порта
string g_host = "127.0.0.1";
int g_port = 9999;
int g_seq_1 = 0;
int g_seq_2 = 9;

TEST(VersionTest, SendStdinToServer) {
    locale::global(locale(""));
    
    cout << "arguments: " << g_host << ' ' << g_port << ' ' << g_seq_1 << ' ' << g_seq_2 << endl;
    try {
        ba::io_context io_context;
        ba::ip::tcp::endpoint ep(
            ba::ip::make_address(g_host),
            g_port
        );
        ba::ip::tcp::socket sock(io_context);
        sock.connect(ep);

        std::ostringstream oss;
        for (int i = g_seq_1; i <= g_seq_2; ++i) {
            oss << (rand() % (g_seq_2 - g_seq_1 + 1) + g_seq_1) << '\n';
        }
        std::string data = oss.str();
        if (!data.empty()) {
            ba::write(sock, ba::buffer(data));
        }
    }
    catch(const boost::system::system_error& ex) {
        cout << "boost exception! " << ex.what() << endl;
        FAIL() << "boost exception: " << ex.what();
    }
    catch(const exception& ex) {
        cout << "std::exception! " << ex.what() << endl;
        FAIL() << "std::exception: " << ex.what();
    }
}

int main(int argc, char **argv) {
    // Парсим адрес и порт из аргументов командной строки
    // Пример: test_client seq 0 9 to 127.0.0.1 9999
    ::testing::InitGoogleTest(&argc, argv);
    if (argc == 7) {
        try {
            if (strcmp(argv[1], "seq") == 0) {
                g_seq_1 = stoi(argv[2]);
                g_seq_2 = stoi(argv[3]);
            }
            if (strcmp(argv[4], "to") == 0) {
                g_host = argv[5];
                g_port = stoi(argv[6]);
            }
        } catch (const std::exception& e) {
            cerr << "Error parsing arguments: " << e.what() << endl;
            return 1;
        }
    }
    else
    {
        cout << "wrong number of arguments" << endl;
        return 1;
    }
    return RUN_ALL_TESTS();
}