#ifndef XML_PARSER_H
#define XML_PARSER_H
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QHttpServerResponse>

class Xml_parser
{
public:
    Xml_parser();
    QMap<QString, QString> read_user_xml(const QByteArray& data, const QString& operation);
    QMap<QString, QString> read_pizza_xml(const QByteArray& data, const QString& operation);
    QMap<QString, QString> read_order_xml(const QByteArray& data, const QString& operation);
    QHttpServerResponse write_response(const QString& error = "");
    QHttpServerResponse write_response(const QMap<QString, QString>& data);
private:
    QMap<QString, QString> extract_data(QXmlStreamReader& xml, const QStringList& properties);
    QByteArray xml_from_file(const QString& path);
    QString get_status_string(const QString& error);
    QHttpServerResponse::StatusCode get_status_code(const QString& error);
    const QStringList user_properties {"ID", "Name", "Surname", "Email", "Password"};
    const QStringList pizza_properties {"ID", "Name", "Cost", "Image"};
    const QStringList order_properties {"ID", "Date", "Address", "Summ", "UserID"};
};

#endif // XML_PARSER_H
