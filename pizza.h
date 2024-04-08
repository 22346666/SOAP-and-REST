#ifndef PIZZA_H
#define PIZZA_H
#include "database_entity.h"

class Pizza : public Database_entity
{
public:
    Pizza(const QMap<QString, QString>& data);
    Pizza(const QJsonDocument& data);
    static Pizza get_from_db(QSqlQuery &query, const QString& id);
    static void delete_from_db(QSqlQuery &query, const QString& id);
    virtual void create(QSqlQuery &query) const override;
    virtual void update(QSqlQuery &query) const override;
    virtual QMap<QString, QString> to_map() const override;
    bool check_exist_name(QSqlQuery &query) const;
    bool check_exist_id(QSqlQuery &query) const;
    static bool check_exist_id(QSqlQuery &query, const QString &id);
    bool check_valid() const;
    QString get_id() const;
private:
    Pizza();
    QString id;
    QString name;
    QString cost;
    QString image;


};

#endif // PIZZA_H
