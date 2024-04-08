#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
class Database_entity;
class User;
class Pizza;
class Order;

class Database : public QObject
{
    Q_OBJECT
public:
    enum Entity_type {
        Order_type = 0,
        Pizza_type = 1,
        User_type = 2
    };
    explicit Database(QObject *parent = nullptr);
    void test_query();
    void create_entity(const Database_entity& entity);
    void update_entity(const Database_entity& entity);
    User get_user(const QString& id);
    void delete_user(const QString& id);
    Pizza get_pizza(const QString& id);
    void delete_pizza(const QString& id);
    Order get_order(const QString& id);
    void delete_order(const QString& id);
    ~Database();
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
private:

signals:

};

#endif // DATABASE_H
