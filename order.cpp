#include "order.h"
#include "user.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>

Order::Order() : id("-1") {}


Order::Order(const QMap<QString, QString> &data) : id((data["ID"].isEmpty()) ? "-1" : data["ID"]), date(data["Date"]), address(data["Address"]), summ(data["Summ"]), user_id(data["UserID"])
{

}

Order::Order(const QJsonDocument &data) : id((QString::number(data["ID"].toInt()).isEmpty()) ? "-1" : QString::number(data["ID"].toInt())), date(data["Date"].toString()), address(data["Address"].toString()), summ(QString::number(data["Summ"].toInt())), user_id(QString::number(data["UserID"].toInt()))
{

}

Order Order::get_from_db(QSqlQuery &query, const QString &id)
{
    if(id.toInt()) {
        query.exec(QString("SELECT * FROM orders WHERE OrderID = '%1'").arg(id));
        if(query.lastError().isValid()) {
            qWarning() << query.lastError().text();
        }
        if(query.next()){
            Order order(QMap<QString, QString>{{"ID", query.value(0).toString()},
                                               {"Date", query.value(1).toString()},
                                               {"Address", query.value(2).toString()},
                                               {"Summ", query.value(3).toString()},
                                               {"UserID", query.value(4).toString()}});
            return std::move(order);
        } else {
            qInfo() << "Not found";
        }
    }
    Order order;
    return std::move(order);
}

void Order::delete_from_db(QSqlQuery &query, const QString &id)
{
    if(!id.toInt()) {
        throw std::runtime_error("Invalid ID");
    }
    if(!check_exist_id(query, id)) {
        throw std::runtime_error("Entity with this ID doesn't exist");
    }
    query.exec(QString("DELETE FROM orders WHERE OrderId = '%1'").arg(id));
    if(query.lastError().isValid()) {
        throw std::runtime_error("SQL error has been thrown");
    }
}

void Order::create(QSqlQuery &query) const
{
    if(!check_valid() || !User::check_exist_id(query, user_id)) {
        throw std::runtime_error("Invalid order data");
    }
    if(check_exist_id(query)) {
        throw std::runtime_error("Entity with this ID already exists");
    }
    query.exec(QString("INSERT INTO orders (OrderDate, OrderAddress, OrderSumm, UserID) values ('%1', '%2', '%3', %4)").arg(date, address, summ, user_id));
    if(query.lastError().isValid()) {
        throw std::runtime_error("SQL error has been thrown");
    }
}

void Order::update(QSqlQuery &query) const
{
    if(!check_valid() || !User::check_exist_id(query, user_id)) {
        throw std::runtime_error("Invalid order data");
    }
    if(id == "-1" || !id.toInt()) {
        throw std::runtime_error("Invalid ID");
    }
    if(!check_exist_id(query)) {
        throw std::runtime_error("Order ID doesn't exist or incorrect user ID");
    }
    query.exec(QString("UPDATE orders SET OrderDate = '%1', OrderAddress = '%2', OrderSumm = '%3', UserID = %4 WHERE OrderID = %5").arg(date, address, summ, user_id, id));
    if(query.lastError().isValid()) {
        throw std::runtime_error("SQL error has been thrown");
    }
}

QMap<QString, QString> Order::to_map() const
{
    QMap<QString, QString> order_data;
    order_data["ID"] = id;
    order_data["Date"] = date;
    order_data["Address"] = address;
    order_data["Summ"] = summ;
    order_data["UserID"] = user_id;
    return std::move(order_data);
}

bool Order::check_exist_id(QSqlQuery &query) const
{
    query.exec(QString("SELECT OrderID FROM orders WHERE OrderID = %1").arg(id));
    if(query.lastError().isValid()) {
        qWarning() << query.lastError().text();
        return false;
    }
    if(query.next()) {
        qInfo() << id << "exists in db";
        return true;
    }
    qInfo() << id << "doesn't exist in db";
    return false;
}

bool Order::check_exist_id(QSqlQuery &query, const QString &id)
{
    query.exec(QString("SELECT OrderID FROM orders WHERE OrderID = %1").arg(id));
    if(query.lastError().isValid()) {
        qWarning() << query.lastError().text();
        return false;
    }
    if(query.next()) {
        qInfo() << id << "exists in db";
        return true;
    }
    qInfo() << id << "doesn't exist in db";
    return false;
}

bool Order::check_valid() const
{
    return (date.isEmpty() || address.isEmpty() || (summ.isEmpty() || !summ.toInt()) || (user_id.isEmpty() || !user_id.toInt())) ? false : true;
}

QString Order::get_id() const
{
    return id;
}
