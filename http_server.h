#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <QHttpServer>
#include <QHttpServerResponse>

#if QT_CONFIG(ssl)
#  include <QSslCertificate>
#  include <QSslKey>
#endif

#include <QObject>
#include <QFile>
#include <QDir>
#include <QString>
#include <QHostInfo>
#include "database.h"
#include "xml_parser.h"
#include <QUrl>


class Http_server : public QObject
{
    Q_OBJECT
public:
    explicit Http_server(QObject *parent = nullptr);

    ~Http_server();

    void listen();
    void listen_ssl();

    QHostAddress get_ip() const;
    void set_ip(const QHostAddress &newIp);


private:
    void route_soap();
    void route_rest();
    QHttpServerResponse write_response(const QString& error = "");
    QHttpServerResponse write_response(const QMap<QString, QString>& data);

    //REST endpoints
    void route_get_endpoints();
    void route_post_endpoints();
    void route_delete_endpoints();
    void route_put_endpoints();

    //SOAP services
    void route_user_serivces(const QString& endpoint, const QString& name);
    void route_pizza_serivces(const QString& endpoint, const QString& name);
    void route_order_serivces(const QString& endpoint, const QString& name);


    static inline QString host(const QHttpServerRequest &request);
    QHttpServer http_server;
    Database db;
    Xml_parser xml_parser;
    QHostAddress ip;
    QString http_domain_name = "https://pizzacrash.servebeer.com";
    QString domain_name = "pizzacrash.servebeer.com";
    const quint16 port = 80;
    const quint16 ssl_port = 443;

signals:

};
#endif // HTTP_SERVER_H
