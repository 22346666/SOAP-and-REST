#ifndef USER_H
#define USER_H
#include <QString>
#include "database_entity.h"

class User : public Database_entity
{
public:
    User(const QMap<QString, QString>& data);
    User(const QJsonDocument& data);
    static User get_from_db(QSqlQuery &query, const QString& id);
    static void delete_from_db(QSqlQuery &query, const QString& id);
    virtual void create(QSqlQuery &query) const override;
    virtual void update(QSqlQuery &query) const override;
    virtual QMap<QString, QString> to_map() const override;
    bool check_exist_email(QSqlQuery &query) const;
    bool check_exist_id(QSqlQuery &query) const;
    static bool check_exist_id(QSqlQuery &query, const QString &id);
    bool check_valid() const;
    QString get_id() const;
private:
    User();
    QString id;
    QString email;
    QString name;
    QString surname;
    QString password;
};

#endif // USER_H
