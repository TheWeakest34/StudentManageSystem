#include "idatabase.h"
#include <QSqlQuery>
#include <QSqlRecord>

IDataBase::IDataBase(QObject *parent) : QObject{parent}
{
    initDataBase();
}

void IDataBase::initDataBase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    QString aFile = "C://Users//32788//Desktop//作业//Qt//FinalLab.db";
    database.setDatabaseName(aFile);
    database.open();
}

QString IDataBase::userLogin(QString userName, QString password)
{
    if(userName.isEmpty())
        return "\t      UserName Is Null";
    QSqlQuery query;
    query.prepare("select UserName,Password from User where username = :USER");
    query.bindValue(":USER",userName);
    query.exec();
    if(query.first() && query.value("UserName").isValid()){
        QString Password = query.value("Password").toString();
        if(Password == password)
            return "true";
        else
            return "\t       Password Error！";
    }
    else
        return "\t      No Such User！";
}

QString IDataBase::userSignUp(QString userName, QString password, QString confirmPassword)
{
    if(userName.isEmpty())
        return "\t          UserName Is Null";
    QSqlQuery query;
    query.prepare("select UserName from User where UserName = :USER");
    query.bindValue(":USER",userName);
    query.exec();
    if(query.first() && query.value("UserName").isValid())  //用户已存在
        return "\t          User Already Exists！";
    else if(password.isEmpty())
        return "\t          Password Is Null";
    else if(password != confirmPassword)
        return "\t          Passwords Not Match！";
    else
    {
        query.prepare("insert into User (UserName, Password) values (:USER, :PWD)");
        query.bindValue(":USER", userName);
        query.bindValue(":PWD", password);
        query.exec();
        return "true";
    }
}

bool IDataBase::initStudentModel()
{

}

bool IDataBase::searchStudent(QString filter)
{

}

void IDataBase::deleteStudent()
{

}

bool IDataBase::submitStudentEdit()
{

}

void IDataBase::revertStudentEdit()
{

}

int IDataBase::addNewStudent()
{

}

