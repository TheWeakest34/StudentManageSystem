#include "idatabase.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QCoreApplication>

IDataBase::IDataBase(QObject *parent) : QObject{parent}
{
    initDataBase();
    studentSelections.reserve(Max_Tabs);   // 预留容量，避免频繁扩容
    studentTableModels.reserve(Max_Tabs);
}

void IDataBase::initDataBase()
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    QString aFile = QCoreApplication::applicationDirPath() + "/FinalLab.db";
    database.setDatabaseName(aFile);
    database.open();
}

QString IDataBase::userLogin(QString userName, QString password)
{
    if(userName.isEmpty())
        return "UserName Is Null";
    QSqlQuery query;
    query.prepare("select UserName,Password from User where username = :USER");
    query.bindValue(":USER",userName);
    query.exec();
    if(query.first() && query.value("UserName").isValid()){
        QString Password = query.value("Password").toString();
        if(Password == password)
            return "true";
        else
            return "Password Error！";
    }
    else
        return "No Such User！";
}

QString IDataBase::userSignUp(QString userName, QString password, QString confirmPassword)
{
    if(userName.isEmpty())
        return "UserName Is Null";
    QSqlQuery query;
    query.prepare("select UserName from User where UserName = :USER");
    query.bindValue(":USER",userName);
    query.exec();
    if(query.first() && query.value("UserName").isValid())  //用户已存在
        return "User Already Exists！";
    else if(password.isEmpty())
        return "Password Is Null";
    else if(password != confirmPassword)
        return "Passwords Not Match！";
    else
    {
        query.prepare("insert into User (UserName, Password) values (:USER, :PWD)");
        query.bindValue(":USER", userName);
        query.bindValue(":PWD", password);
        query.exec();
        return "true";
    }
}

QVector<IDataBase::TrendData> IDataBase::getTrendData(QString studentId)
{
    QVector<TrendData> result;
    QStringList fields = {"MathScore", "CScore", "JavaScore", "TotalScore"};

    for(int i = 0; i < Max_Tabs; i++) {
        QString tableName = QString("exam%1").arg(i+1);

        // 检查表是否存在且有数据
        QSqlQuery checkQuery;
        if(!checkQuery.exec(QString("select count(*) from %1").arg(tableName)))
            continue;
        if(!checkQuery.next() || checkQuery.value(0).toInt() == 0)
            continue;

        QSqlQuery query;
        bool execSuccess = false;

        // --- 1. 查询个人 ---
        if(!studentId.isEmpty()) {
            // 拼接 SQL: select MathScore, CScore, JavaScore, TotalScore from exam1 where ID = '...'
            QString sql = QString("select %1 from %2 where ID = :ID").arg(fields.join(", "), tableName);
            query.prepare(sql);
            query.bindValue(":ID", studentId);
            execSuccess = query.exec();
        }
        // --- 2. 查询全年级 (修复重点) ---
        else {
            // 手动拼接 SQL: select avg(MathScore), avg(CScore), avg(JavaScore), avg(TotalScore) from exam1
            QString avgFields = "avg(" + fields.join("), avg(") + ")";
            QString sql = QString("select %1 from %2").arg(avgFields, tableName);

            query.prepare(sql);
            execSuccess = query.exec();
        }

        // 获取数据
        if(execSuccess && query.next()) {
            TrendData data;
            data.examName = QString("第%1次考试").arg(i+1);
            for(int j = 0; j < 4; j++) {
                data.scores.append(query.value(j).toDouble());
            }
            result.append(data);
        }
    }
    return result;
}

int IDataBase::TableCount()
{
    for(int i=0 ; i<Max_Tabs ; i++){
        QString tableName = QString("exam%1").arg(i+1);
        QSqlQuery query;
        QString sql = QString("select count(*) from %1").arg(tableName);
        query.exec(sql);

        if(query.next() && query.value(0).toInt() > 0){
            continue;
        }
        return i;
    }
    return Max_Tabs;
}

bool IDataBase::initNewModel(int index)
{
    QSqlTableModel *model = new QSqlTableModel(this,database);
    model->setTable("exam" + QString::number(index+1));
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setSort(model->fieldIndex("ID"),Qt::AscendingOrder);

    if(!model->select())
        return false;
    studentTableModels.push_back(model);

    QItemSelectionModel *selection = new QItemSelectionModel(model);
    studentSelections.push_back(selection);

    return true;
}

bool IDataBase::searchStudent(int index,QString filter)
{
    studentTableModels[index]->setFilter(filter);
    return studentTableModels[index]->select();
}

void IDataBase::deleteStudent(int index)
{
    QModelIndex currentIndex = studentSelections[index]->currentIndex();

    if(currentIndex.row() < 0){
        QMessageBox::warning(nullptr,"提示","请先选择一行!",QMessageBox::Ok);
        return;
    }

    //获取选中记录的学生姓名
    int row = currentIndex.row();
    int column = studentTableModels[index]->fieldIndex("Name");
    QModelIndex nameIndex = studentTableModels[index]->index(row,column);
    QString Name = studentTableModels[index]->data(nameIndex).toString();

    QMessageBox::StandardButton reply = QMessageBox::question(nullptr,"提示","确认要删除「" + Name + "」同学的信息吗？",
                                                              QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        studentTableModels[index]->removeRow(currentIndex.row());
        studentTableModels[index]->submitAll();
        studentTableModels[index]->select();
    }
    else
        return;
}

bool IDataBase::submitStudentEdit(int index)
{
    return studentTableModels[index]->submitAll();
}

void IDataBase::revertStudentEdit(int index)
{
    studentTableModels[index]->revertAll();
}

int IDataBase::addNewStudent(int index)
{
    studentTableModels[index]->insertRow(studentTableModels[index]->rowCount(),QModelIndex());
    QModelIndex curIndex = studentTableModels[index]->index(studentTableModels[index]->rowCount() - 1,1);

    int curRecNo = curIndex.row();
    QSqlRecord curRec = studentTableModels[index]->record(curRecNo);
    studentTableModels[index]->setRecord(curRecNo,curRec);

    return curRecNo;
}

IDataBase::ScoreStat IDataBase::getStatistics(int tabIndex, QString subject)
{
    ScoreStat stat;
    QString tableName = "exam" + QString::number(tabIndex + 1);
    QSqlQuery query;
    QString sql;

    QString fieldName;
    if(subject == "数学")
        fieldName = "MathScore";
    else if(subject == "C语言")
        fieldName = "CScore";
    else if(subject == "Java")
        fieldName = "JavaScore";
    else if(subject == "全科")
        fieldName = "TotalScore";

    if(fieldName == "TotalScore")
        sql = QString(
                          "SELECT "
                          "COUNT(*) as total, "
                          "AVG(%1) as avg, "
                          "MAX(%1) as max, "
                          "MIN(%1) as min, "
                          "SUM(CASE WHEN %1 >= 180 THEN 1 ELSE 0 END) as pass, "
                          "SUM(CASE WHEN %1 >= 255 THEN 1 ELSE 0 END) as excellent "
                          "FROM %2"
                          ).arg(fieldName, tableName);
    else
        sql = QString(
                  "SELECT "
                  "COUNT(*) as total, "
                  "AVG(%1) as avg, "
                  "MAX(%1) as max, "
                  "MIN(%1) as min, "
                  "SUM(CASE WHEN %1 >= 60 THEN 1 ELSE 0 END) as pass, "
                  "SUM(CASE WHEN %1 >= 85 THEN 1 ELSE 0 END) as excellent "
                  "FROM %2"
                  ).arg(fieldName, tableName);

    //qDebug()<<sql;
    if(query.exec(sql)) {
        if(query.next()) {
            stat.totalCount = query.value("total").toInt();
            stat.avg = query.value("avg").toDouble();
            stat.max = query.value("max").toDouble();
            stat.min = query.value("min").toDouble();

            //计算及格率和优秀率
            stat.passRate = (double) query.value("pass").toInt() / stat.totalCount * 100.0;
            stat.excellentRate = (double) query.value("excellent").toInt() / stat.totalCount * 100.0;
        }
    }
    return stat;
}

