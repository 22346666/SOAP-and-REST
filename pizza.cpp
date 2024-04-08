#include "pizza.h"
#include <QSqlQuery>
#include <QJsonDocument>
#include <QSqlError>

Pizza::Pizza() : id("-1") {}

Pizza::Pizza(const QMap<QString, QString> &data) : id((data["ID"].isEmpty()) ? "-1" : data["ID"]), name(data["Name"]), cost(data["Cost"]), image(data["Image"])
{
}

Pizza::Pizza(const QJsonDocument &data) : id((QString::number(data["ID"].toInt()).isEmpty()) ? "-1" : QString::number(data["ID"].toInt())), name(data["Name"].toString()), cost(QString::number(data["Cost"].toInt())), image(data["Image"].toString())
{
}

Pizza Pizza::get_from_db(QSqlQuery &query, const QString &id)
{
    if(id.toInt()) {
        query.exec(QString("SELECT * FROM pizza WHERE PizzaId = '%1'").arg(id));
        if(query.lastError().isValid()) {
            qWarning() << query.lastError().text();
        }
        if(query.next()){
            Pizza pizza(QMap<QString, QString>{{"ID", query.value(0).toString()},
                                             {"Name", query.value(1).toString()},
                                             {"Cost", query.value(2).toString()},
                                             {"Image", query.value(3).toString()}});
            return std::move(pizza);
        } else {
            qInfo() << "Not found";
        }
    }
    Pizza pizza;
    return std::move(pizza);
}

void Pizza::delete_from_db(QSqlQuery &query, const QString &id)
{
    if(!id.toInt()) {
        throw std::runtime_error("Invalid ID");
    }
    if(!check_exist_id(query, id)) {
        throw std::runtime_error("Entity with this ID doesn't exist");
    }
    query.exec(QString("DELETE FROM pizza WHERE PizzaId = '%1'").arg(id));
    if(query.lastError().isValid()) {
        throw std::runtime_error("SQL error has been thrown");
    }
}

void Pizza::create(QSqlQuery &query) const
{
    if(!check_valid()) {
        throw std::runtime_error("Invalid pizza data");
    }
    if(check_exist_name(query)) {
        throw std::runtime_error("Entity with this ID already exists");
    }
    query.exec(QString("INSERT INTO pizza (PizzaName, PizzaCost, PizzaImage) values ('%1', '%2', '%3')").arg(name, cost, image));
    if(query.lastError().isValid()) {
        if(query.lastError().type() == QSqlError::ErrorType::StatementError) {
            throw std::runtime_error("Pizza with this name already exists. Name should be unique");
        } else {
            throw std::runtime_error("SQL error has been thrown");
        }
    }
}

void Pizza::update(QSqlQuery &query) const
{
    if(!check_valid()) {
        throw std::runtime_error("Invalid pizza data");
    }
    if(id == "-1" || !id.toInt()) {
        throw std::runtime_error("Invalid ID");
    }
    if(!check_exist_id(query)) {
        throw std::runtime_error("Entity with this ID doesn't exist");
    }
    query.exec(QString("UPDATE pizza SET PizzaName = '%1', PizzaCost = '%2', PizzaImage = '%3' WHERE PizzaId = %4").arg(name, cost, image, id));
    if(query.lastError().isValid()) {
        if(query.lastError().type() == QSqlError::ErrorType::StatementError) {
            throw std::runtime_error("Pizza with this name already exists. Name should be unique");
        } else {
            throw std::runtime_error("SQL error has been thrown");
        }
    }
}

QMap<QString, QString> Pizza::to_map() const
{
    QMap<QString, QString> pizza_data;
    pizza_data["ID"] = id;
    pizza_data["Name"] = name;
    pizza_data["Cost"] = cost;
    pizza_data["Image"] = image;
    return std::move(pizza_data);
}

bool Pizza::check_exist_name(QSqlQuery &query) const
{
    query.exec(QString("SELECT PizzaId FROM pizza WHERE PizzaName = '%1'").arg(name));
    if(query.lastError().isValid()) {
        qWarning() << query.lastError().text();
        return false;
    }
    if(query.next()) {
        qInfo() << name << "exists in db";
        return true;
    }
    qInfo() << name << "doesn't exist in db";
    return false;
}

bool Pizza::check_exist_id(QSqlQuery &query) const
{
    query.exec(QString("SELECT PizzaId FROM pizza WHERE PizzaId = %1").arg(id));
    if(query.lastError().isValid()) {
        qWarning() << query.lastError().text();
        return false;
    }
    if(query.next()) {
        qInfo() << name << "exists in db";
        return true;
    }
    qInfo() << name << "doesn't exist in db";
    return false;
}

bool Pizza::check_exist_id(QSqlQuery &query, const QString &id)
{
    query.exec(QString("SELECT PizzaId FROM pizza WHERE PizzaId = %1").arg(id));
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

bool Pizza::check_valid() const
{
    return (name.isEmpty() || (cost.isEmpty() || !cost.toInt()) || image.isEmpty()) ? false : true;
}

QString Pizza::get_id() const
{
    return id;
}
