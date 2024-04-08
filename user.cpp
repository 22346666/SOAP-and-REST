#include <QMap>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include "user.h"

User::User() : id("-1")
{

}

User::User(const QMap<QString, QString> &data) : id((data["ID"].isEmpty()) ? "-1" : data["ID"]), email(data["Email"]), name(data["Name"]), surname(data["Surname"]), password(data["Password"]) {
}

User::User(const QJsonDocument &data) : id((QString::number(data["ID"].toInt()).isEmpty()) ? "-1" : QString::number(data["ID"].toInt())), email(data["Email"].toString()), name(data["Name"].toString()), surname(data["Surname"].toString()), password(data["Password"].toString())
{

}

User User::get_from_db(QSqlQuery &query, const QString &id)
{
    if(id.toInt()) {
        query.exec(QString("SELECT * FROM users WHERE UserId = '%1'").arg(id));
        if(query.lastError().isValid()) {
            qWarning() << query.lastError().text();
        }
        if(query.next()){
            User user(QMap<QString, QString>{{"ID", query.value(0).toString()},
                                             {"Name", query.value(1).toString()},
                                             {"Surname", query.value(2).toString()},
                                             {"Email", query.value(3).toString()},
                                             {"Password", query.value(4).toString()}});
            return std::move(user);
        } else {
            qInfo() << "Not found";
        }
    }
    User user;
    return std::move(user);
}

void User::delete_from_db(QSqlQuery &query, const QString &id)
{
    if(!id.toInt()) {
        throw std::runtime_error("Invalid ID");
    }
    if(!check_exist_id(query, id)) {
        throw std::runtime_error("Entity with this ID doesn't exist");
    }
    query.exec(QString("DELETE FROM users WHERE UserId = '%1'").arg(id));
    if(query.lastError().isValid()) {
        throw std::runtime_error("SQL error has been thrown");
    }
}

void User::create(QSqlQuery &query) const
{
    if(!check_valid()) {
        throw std::runtime_error("Invalid user data");
    }
    if(check_exist_email(query)) {
        throw std::runtime_error("Entity with this ID already exists");
    }
    query.exec(QString("INSERT INTO users (UserName, UserSurname, UserEmail, UserPassword) values ('%1', '%2', '%3', '%4')").arg(name, surname, email, password));
    if(query.lastError().isValid()) {
        if(query.lastError().type() == QSqlError::ErrorType::StatementError) {
            throw std::runtime_error("User with this email already exists. Email should be unique");
        } else {
            throw std::runtime_error("SQL error has been thrown");
        }
    }
}

void User::update(QSqlQuery &query) const
{
    if(!check_valid()) {
        throw std::runtime_error("Invalid user data");
    }
    if(id == "-1" || !id.toInt()) {
        throw std::runtime_error("Invalid ID");
    }
    if(!check_exist_id(query)) {
        throw std::runtime_error("Entity with this ID doesn't exist");
    }
    query.exec(QString("UPDATE users SET UserName = '%1', UserSurname = '%2', UserEmail = '%3', UserPassword = '%4' WHERE UserID = %5").arg(name, surname, email, password, id));
    if(query.lastError().isValid()) {
        if(query.lastError().type() == QSqlError::ErrorType::StatementError) {
            throw std::runtime_error("User with this email already exists. Email should be unique");
        } else {
            throw std::runtime_error("SQL error has been thrown");
        }
    }
}

QMap<QString, QString> User::to_map() const
{
    QMap<QString, QString> user_data;
    user_data["ID"] = id;
    user_data["Email"] = email;
    user_data["Name"] = name;
    user_data["Surname"] = surname;
    user_data["Password"] = password;
    return std::move(user_data);
}

bool User::check_exist_email(QSqlQuery &query) const
{
    query.exec(QString("SELECT UserId FROM users WHERE UserEmail = '%1'").arg(email));
    if(query.lastError().isValid()) {
        qWarning() << query.lastError().text();
        return false;
    }
    if(query.next()) {
        qInfo() << email << "exists in db";
        return true;
    }
    qInfo() << email << "doesn't exist in db";
    return false;
}

bool User::check_exist_id(QSqlQuery &query) const
{
    query.exec(QString("SELECT UserId FROM users WHERE UserId = %1").arg(id));
    if(query.lastError().isValid()) {
        qWarning() << query.lastError().text();
        return false;
    }
    if(query.next()) {
        qInfo() << email << "exists in db";
        return true;
    }
    qInfo() << email << "doesn't exist in db";
    return false;
}

bool User::check_exist_id(QSqlQuery &query, const QString &id)
{
    query.exec(QString("SELECT UserId FROM users WHERE UserId = %1").arg(id));
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

bool User::check_valid() const
{
    return (surname.isEmpty() || password.isEmpty() || name.isEmpty() || email.isEmpty()) ? false : true;
}

QString User::get_id() const
{
    return id;
}

