#ifndef DATABASE_ENTITY_H
#define DATABASE_ENTITY_H
#include <QMap>
class QSqlQuery;
class QJsonDocument;

class Database_entity
{
public:
    virtual ~Database_entity() = default;
    virtual void create(QSqlQuery& query) const = 0;
    virtual void update(QSqlQuery& query) const = 0;
    virtual QMap<QString, QString> to_map() const = 0;
};

#endif // DATABASE_ENTITY_H
