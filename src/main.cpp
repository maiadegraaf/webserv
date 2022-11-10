#include "Server.hpp"

int main()
{
    location loc("/");
    config conf(80, "127.0.0.1", "www", 100000, loc, "test");
    Server server(&conf);

    server.setup();
    server.run();
    server.closeFds();
}