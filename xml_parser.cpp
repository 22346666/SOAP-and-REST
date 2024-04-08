#include "xml_parser.h"
#include <QDebug>
#include <QFile>

Xml_parser::Xml_parser() {}

QMap<QString, QString> Xml_parser::extract_data(QXmlStreamReader &xml, const QStringList &properties)
{
    QMap<QString, QString> output;
    for(auto& i : properties) {
        output[i] = "";
    }
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement()) {
            for(auto& i : properties) {
                if(i.toLower() == xml.name().toString().toLower()) {
                    output[i] = xml.readElementText();
                }
            }
        } else if (xml.isEndElement()) {
            break;
        }
    }
    return output;
}

QByteArray Xml_parser::xml_from_file(const QString &path)
{
    QFile file(path);
    QByteArray html;
    if(file.open(QIODevice::ReadOnly)) {
        html+=file.readAll();
    }else {
        qWarning() << file.errorString();
    }
    return html;
}

QString Xml_parser::get_status_string(const QString &error)
{
    return (error.isEmpty()) ? "Operation have completed succesfully" : "Operation failed: " + error;
}

QHttpServerResponse::StatusCode Xml_parser::get_status_code(const QString &error)
{
    return (error.isEmpty()) ? QHttpServerResponse::StatusCode::Ok : QHttpServerResponse::StatusCode::BadRequest;
}

QMap<QString, QString> Xml_parser::read_user_xml(const QByteArray &data, const QString &operation)
{
    QXmlStreamReader xml(data);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == operation) {
                return extract_data(xml, user_properties);
            }
        }
    }
    return QMap<QString, QString>();
}

QMap<QString, QString> Xml_parser::read_pizza_xml(const QByteArray &data, const QString &operation)
{
    QXmlStreamReader xml(data);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == operation) {
                return extract_data(xml, pizza_properties);
            }
        }
    }
    return QMap<QString, QString>();
}

QMap<QString, QString> Xml_parser::read_order_xml(const QByteArray &data, const QString &operation)
{
    QXmlStreamReader xml(data);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == operation) {
                return extract_data(xml, order_properties);
            }
        }
    }
    return QMap<QString, QString>();
}

QHttpServerResponse Xml_parser::write_response(const QString &error)
{
    QByteArray envelope;
    QXmlStreamWriter xml(&envelope);
    xml.writeStartDocument();
    xml.writeNamespace("http://www.w3.org/2003/05/soap-envelope", "soap");
    xml.writeStartElement("soap:Envelope");
    xml.writeStartElement("soap:Header");
    xml.writeEndElement();
    xml.writeStartElement("soap:Body");
    xml.writeTextElement("result", get_status_string(error));
    xml.writeEndElement();
    xml.writeEndDocument();
    return QHttpServerResponse(envelope, get_status_code(error));
}

QHttpServerResponse Xml_parser::write_response(const QMap<QString, QString> &data)
{
    QByteArray envelope;
    QXmlStreamWriter xml(&envelope);
    xml.writeStartDocument();
    xml.writeNamespace("http://www.w3.org/2003/05/soap-envelope", "soap");
    xml.writeStartElement("soap:Envelope");
    xml.writeStartElement("soap:Header");
    xml.writeEndElement();
    xml.writeStartElement("soap:Body");
    xml.writeStartElement("result");
    for(auto i = data.keyBegin(); i != data.keyEnd(); ++i) {
        xml.writeTextElement(*i, data[*i]);
    }
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndElement();
    xml.writeEndDocument();
    return QHttpServerResponse(envelope, QHttpServerResponse::StatusCode::Ok);
}


