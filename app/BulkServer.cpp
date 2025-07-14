#include "BulkServer.h"
using namespace std;

void session::do_read()
{
    auto self(shared_from_this()); // 3. создаем шеред пойнтер чтобы сессия не была уничтожена до завершения асинхронной операции
    // 4. асинхронно читаем данные из буфера
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        // 5. когда данные прочитались, вызывается лямбда, отправив в лямбду эррор код и размер данных
        [this, self](boost::system::error_code ec, size_t length)
        {
            if (!ec)
            {
                // cout << "receive " << length << "=" << string{data_, length} << endl;
                libasync::receive(self->ctx_, data_, length);
                // 6. вызываем do_read снова, чтобы продолжать чтение данных
                do_read();
            }
        });
}

void server::do_accept()
{
    acceptor_.async_accept(
        // 1. акцептует приходящие соединения на этот порт и вызывает лямбду. Передает в лямбду
        //  эррор код если акцептор его выдал и tcp сокет от акцептора
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                make_shared<session>(ctx_, move(socket))->start(bulkSize_); // 2. стартуем сессию для текущего потока, отдавая ему на владение сокет
            }

            do_accept(); // снова ждем акцептим следующее соединение
        });
}
