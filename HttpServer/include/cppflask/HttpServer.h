#pragma once

#include <thread>
#include <memory>
#include <future>

namespace httplib { class Server; }

namespace cppflask {
    class IRouter;

    class HttpServer {
    public:
        explicit HttpServer(IRouter& router);
        ~HttpServer();

        void start(int port = 8080);
        void stop();

        [[nodiscard]] bool isRunning() const;
        [[nodiscard]] std::future<void> getStopSignal();

    private:
        std::unique_ptr<httplib::Server> _server;
        std::thread _serverThread{};
        bool _isStarted{false};
        int _port{80};
        std::promise<void> _stopSignal{};

        void listen();
    };
}