#include <QCoreApplication>
#include <QProcess>
#include "http_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QDir::setCurrent("D:/Documents/Server");
    Http_server server;
    server.listen_ssl();
    return app.exec();
}
