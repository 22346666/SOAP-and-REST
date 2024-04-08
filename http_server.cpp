#include "http_server.h"
#include "order.h"
#include "pizza.h"
#include "user.h"
#include "QJsonObject"
#include "QJsonDocument"

Http_server::Http_server(QObject *parent)
    : QObject{parent}
{
    ip.setAddress("192.168.1.105");
    http_server.afterRequest([](QHttpServerResponse &&responce) {
        responce.setHeader("Server", "Pizza cite server");
        return std::move(responce);
    });
    db.test_query();
    route_soap();
    route_rest();
}

void Http_server::route_soap()
{
    QString soap_endpoint = "/soap/";
    route_user_serivces(soap_endpoint, "User");
    route_order_serivces(soap_endpoint, "Order");
    route_pizza_serivces(soap_endpoint, "Pizza");
}

void Http_server::route_rest()
{
    route_get_endpoints();
    route_post_endpoints();
    route_delete_endpoints();
    route_put_endpoints();
}

QHttpServerResponse Http_server::write_response(const QString &error)
{
    if (error.isEmpty()) {
        return QHttpServerResponse("Successful", QHttpServerResponse::StatusCode::Ok);
    }
    return QHttpServerResponse(error, QHttpServerResponse::StatusCode::BadRequest);
}

QHttpServerResponse Http_server::write_response(const QMap<QString, QString> &data)
{
    QJsonObject json;
    QMapIterator<QString, QString> i(data);
    while (i.hasNext()) {
        i.next();
        json.insert(i.key(), i.value());
    }
    return QHttpServerResponse(json, QHttpServerResponse::StatusCode::Ok);
}

void Http_server::route_get_endpoints()
{
    http_server.route("/get/user/<arg>", [this](QString id, const QHttpServerRequest &request) {
        return write_response(db.get_user(id).to_map());
    });

    http_server.route("/get/order/<arg>", [this](QString id, const QHttpServerRequest &request) {
        return write_response(db.get_order(id).to_map());
    });

    http_server.route("/get/pizza/<arg>", [this](QString id, const QHttpServerRequest &request) {
        return write_response(db.get_pizza(id).to_map());
    });
}

void Http_server::route_post_endpoints()
{
    http_server.route("/post/user", [this](const QHttpServerRequest &request) {
        QJsonDocument json = QJsonDocument::fromJson(request.body());
        try {
            db.create_entity(User(json));
            return write_response();
        } catch (const std::exception& e) {
            return write_response(e.what());
        }
    });

    http_server.route("/post/order", [this](const QHttpServerRequest &request) {
        QJsonDocument json = QJsonDocument::fromJson(request.body());
        try {
            db.create_entity(Order(json));
            return write_response();
        } catch (const std::exception& e) {
            return write_response(e.what());
        }
    });

    http_server.route("/post/pizza", [this](const QHttpServerRequest &request) {
        QJsonDocument json = QJsonDocument::fromJson(request.body());
        try {
            db.create_entity(Pizza(json));
            return write_response();
        } catch (const std::exception& e) {
            return write_response(e.what());
        }
    });
}

void Http_server::route_delete_endpoints()
{
    http_server.route("/delete/user/<arg>", [this](QString id, const QHttpServerRequest &request) {
        try {
            db.delete_user(id);
            return write_response();
        } catch (const std::exception& e) {
            return write_response(e.what());
        }
    });

    http_server.route("/delete/order/<arg>", [this](QString id, const QHttpServerRequest &request) {
        try {
            db.delete_order(id);
            return write_response();
        } catch (const std::exception& e) {
            return write_response(e.what());
        }
    });

    http_server.route("/delete/pizza/<arg>", [this](QString id, const QHttpServerRequest &request) {
        try {
            db.delete_pizza(id);
            return write_response();
        } catch (const std::exception& e) {
            return write_response(e.what());
        }
    });
}

void Http_server::route_put_endpoints()
{
    http_server.route("/put/user", [this](const QHttpServerRequest &request) {
        QJsonDocument json = QJsonDocument::fromJson(request.body());
        try {
            db.update_entity(User(json));
            return write_response();
        } catch (const std::exception& e) {
            return write_response(e.what());
        }
    });

    http_server.route("/put/order", [this](const QHttpServerRequest &request) {
        QJsonDocument json = QJsonDocument::fromJson(request.body());
        try {
            db.update_entity(Order(json));
            return write_response();
        } catch (const std::exception& e) {
            return write_response(e.what());
        }
    });

    http_server.route("/put/pizza", [this](const QHttpServerRequest &request) {
        QJsonDocument json = QJsonDocument::fromJson(request.body());
        try {
            db.update_entity(Pizza(json));
            return write_response();
        } catch (const std::exception& e) {
            return write_response(e.what());
        }
    });
}

void Http_server::route_user_serivces(const QString &endpoint, const QString &name)
{
    http_server.route(endpoint + name.toLower() + "/update" + name, [this](const QHttpServerRequest &request) {
        User user(xml_parser.read_user_xml(request.body(), "updateUser"));
        try {
            db.update_entity(user);
            return xml_parser.write_response();
        } catch (const std::exception& e) {
            return xml_parser.write_response(e.what());
        }
    });

    http_server.route(endpoint + name.toLower() + "/create"+name, [this](const QHttpServerRequest &request) {
        User user(xml_parser.read_user_xml(request.body(), "createUser"));
        try {
            db.create_entity(user);
            return xml_parser.write_response();
        } catch (const std::exception& e) {
            return xml_parser.write_response(e.what());
        }
    });

    http_server.route(endpoint + name.toLower() + "/get"+name, [this](const QHttpServerRequest &request) {
        User user(xml_parser.read_user_xml(request.body(), "getUser"));
        return xml_parser.write_response(db.get_user(user.get_id()).to_map());
    });

    http_server.route(endpoint + name.toLower() + "/delete"+name, [this](const QHttpServerRequest &request) {
        User user(xml_parser.read_user_xml(request.body(), "updateUser"));
        try {
            db.delete_user(user.get_id());
            return xml_parser.write_response();
        } catch (const std::exception& e) {
            return xml_parser.write_response(e.what());
        }
    });
}

void Http_server::route_pizza_serivces(const QString& endpoint, const QString& name)
{
    http_server.route(endpoint + name.toLower() + "/update" + name, [this](const QHttpServerRequest &request) {
        Pizza pizza(xml_parser.read_pizza_xml(request.body(), "updatePizza"));
        try {
            db.update_entity(pizza);
            return xml_parser.write_response();
        } catch (const std::exception& e) {
            return xml_parser.write_response(e.what());
        }
    });

    http_server.route(endpoint + name.toLower() + "/create"+name, [this](const QHttpServerRequest &request) {
        Pizza pizza(xml_parser.read_pizza_xml(request.body(), "createPizza"));
        try {
            db.create_entity(pizza);
            return xml_parser.write_response();
        } catch (const std::exception& e) {
            return xml_parser.write_response(e.what());
        }
    });

    http_server.route(endpoint + name.toLower() + "/get"+name, [this](const QHttpServerRequest &request) {
        Pizza pizza(xml_parser.read_pizza_xml(request.body(), "getPizza"));
        try {
            return xml_parser.write_response(db.get_pizza(pizza.get_id()).to_map());
        } catch (const std::exception& e) {
            return xml_parser.write_response(e.what());
        }
    });

    http_server.route(endpoint + name.toLower() + "/delete"+name, [this](const QHttpServerRequest &request) {
        Pizza pizza(xml_parser.read_pizza_xml(request.body(), "deletePizza"));
        try {
            db.delete_pizza(pizza.get_id());
            return xml_parser.write_response();
        } catch (const std::exception& e) {
            return xml_parser.write_response(e.what());
        }
    });
}

void Http_server::route_order_serivces(const QString& endpoint, const QString& name)
{
    http_server.route(endpoint + name.toLower() + "/update" + name, [this](const QHttpServerRequest &request) {
        Order order(xml_parser.read_order_xml(request.body(), "updateOrder"));
        try {
            db.update_entity(order);
            return xml_parser.write_response();
        } catch (const std::exception& e) {
            return xml_parser.write_response(e.what());
        }
    });

    http_server.route(endpoint + name.toLower() + "/create"+name, [this](const QHttpServerRequest &request) {
        Order order(xml_parser.read_order_xml(request.body(), "createOrder"));
        try {
            db.create_entity(order);
            return xml_parser.write_response();
        } catch (const std::exception& e) {
            return xml_parser.write_response(e.what());
        }
    });

    http_server.route(endpoint + name.toLower() + "/get"+name, [this](const QHttpServerRequest &request) {
        Order order(xml_parser.read_order_xml(request.body(), "getOrder"));
        try {
            return xml_parser.write_response(db.get_order(order.get_id()).to_map());
        } catch (const std::exception& e) {
            return xml_parser.write_response(e.what());
        }
    });

    http_server.route(endpoint + name.toLower() + "/delete"+name, [this](const QHttpServerRequest &request) {
        Order order(xml_parser.read_order_xml(request.body(), "deleteOrder"));
        try {
            db.delete_order(order.get_id());
            return xml_parser.write_response();
        } catch (const std::exception& e) {
            return xml_parser.write_response(e.what());
        }
    });
}

Http_server::~Http_server()
{
    http_server.deleteLater();
}

void Http_server::listen()
{
    if(http_server.listen(ip, port) == 0) {
        qWarning() << "Couldn't managed to listen without SSL to this ip adress:" << ip << "and port:" << port;
        return;
    }
    qInfo().noquote().nospace() << "Running on http://" << ip.toString() << ":" << port << "/";
}

void Http_server::listen_ssl()
{
#if QT_CONFIG(ssl)
    const auto sslCertificateChain = QSslCertificate::fromPath(QStringLiteral(":/assets/fullchain.pem"));
    if (sslCertificateChain.empty()) {
        qWarning() << "Couldn't retrieve SSL certificate from file.";
        return;
    }
    QFile private_key_file(QStringLiteral(":/assets/privkey.pem"));
    if (!private_key_file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file for reading:" << private_key_file.errorString();
        return;
    }
    http_server.sslSetup(sslCertificateChain.front(), QSslKey(&private_key_file, QSsl::KeyAlgorithm::Ec));
    private_key_file.close();

    http_server.listen(ip, ssl_port);
    if (!ssl_port) {
        qWarning() << "Server failed to listen on a port";
        return;
    }
    qInfo().noquote().nospace() << "Running with SSL on https://" << ip.toString() << ":" << ssl_port << "/";
#else
    qWarning() << "Couldn't managed to run SSL correctly";
#endif
}

QString Http_server::host(const QHttpServerRequest &request)
{
    return QString::fromLatin1(request.value("Host"));
}

QHostAddress Http_server::get_ip() const
{
    return ip;
}

void Http_server::set_ip(const QHostAddress &new_ip)
{
    ip = new_ip;
}


