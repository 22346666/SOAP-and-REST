#ifndef ORDER_H
#define ORDER_H
#include "database_entity.h"

class Order : public Database_entity
{
public:
    Order(const QMap<QString, QString>& data);
    Order(const QJsonDocument& data);
    static Order get_from_db(QSqlQuery &query, const QString& id);
    static void delete_from_db(QSqlQuery &query, const QString& id);
    virtual void create(QSqlQuery &query) const override;
    virtual void update(QSqlQuery &query) const override;
    virtual QMap<QString, QString> to_map() const override;
    bool check_exist_id(QSqlQuery &query) const;
    static bool check_exist_id(QSqlQuery &query, const QString &id);
    bool check_valid() const;
    QString get_id() const;
private:
    Order();
    QString id;
    QString date;
    QString address;
    QString summ;
    QString user_id;
};

#endif // ORDER_H
