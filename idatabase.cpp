#include "idatabase.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

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
    studentTableModel = new QSqlTableModel(this,database);
    studentTableModel->setTable("Student");
    studentTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    studentTableModel->setSort(studentTableModel->fieldIndex("ID"),Qt::AscendingOrder);

    if(!studentTableModel->select())
        return false;

    studentSelection = new QItemSelectionModel(studentTableModel);
    return true;
}

bool IDataBase::searchStudent(QString filter)
{
    studentTableModel->setFilter(filter);
    return studentTableModel->select();
}

void IDataBase::deleteStudent()
{
    QModelIndex currentIndex = studentSelection->currentIndex();

    //获取选中记录的学生姓名
    int row = currentIndex.row();
    int column = studentTableModel->fieldIndex("Name");
    QModelIndex index = studentTableModel->index(row,column);
    QString Name = studentTableModel->data(index).toString();

    QMessageBox::StandardButton reply = QMessageBox::question(nullptr,"提示","确认要删除" + Name + "同学的信息吗？",
                                                              QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        studentTableModel->removeRow(currentIndex.row());
        studentTableModel->submitAll();
        studentTableModel->select();
    }
    else
        return;
}

bool IDataBase::submitStudentEdit()
{
    return studentTableModel->submitAll();
}

void IDataBase::revertStudentEdit()
{
    studentTableModel->revertAll();
}

int IDataBase::addNewStudent()
{
    studentTableModel->insertRow(studentTableModel->rowCount(),QModelIndex());
    QModelIndex curIndex = studentTableModel->index(studentTableModel->rowCount() - 1,1);

    int curRecNo = curIndex.row();
    QSqlRecord curRec = studentTableModel->record(curRecNo);
    studentTableModel->setRecord(curRecNo,curRec);

    return curRecNo;
}

