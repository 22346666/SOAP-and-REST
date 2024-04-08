#include "database.h"
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QSqlRecord>
#include "order.h"
#include "user.h"
#include "pizza.h"
#include "database_entity.h"

Database::Database(QObject *parent)
    : QObject{parent}
{
    qInfo() << "Opening database";
    db.setHostName("AN/MSSQLSERVER01");
    db.setDatabaseName("SQL server");
    db.setUserName("AN\21");
    db.setPassword("11");
    if(!db.open()) {
        qInfo() << db.lastError().text();
        return;
    }
    qInfo() << "Database is working";
}

void Database::test_query()
{
}

void Database::create_entity(const Database_entity &entity)
{
    QSqlQuery query(db);
    return entity.create(query);
}

void Database::update_entity(const Database_entity &entity)
{
    QSqlQuery query(db);
    return entity.update(query);
}

Database::~Database()
{
    db.close();
    qInfo() << "Closing connection with DB";
}

User Database::get_user(const QString &id)
{
    QSqlQuery query(db);
    return std::move(User::get_from_db(query, id));
}

void Database::delete_user(const QString &id)
{
    QSqlQuery query(db);
    User::delete_from_db(query, id);
}

Pizza Database::get_pizza(const QString &id)
{
    QSqlQuery query(db);
    return std::move(Pizza::get_from_db(query, id));
}

void Database::delete_pizza(const QString &id)
{
    QSqlQuery query(db);
    Pizza::delete_from_db(query, id);
}

Order Database::get_order(const QString &id)
{
    QSqlQuery query(db);
    return std::move(Order::get_from_db(query, id));
}

void Database::delete_order(const QString &id)
{
    QSqlQuery query(db);
    Order::delete_from_db(query, id);
}

