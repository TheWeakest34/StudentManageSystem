#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QItemSelectionModel>
#include <QVector>

class IDataBase : public QObject
{
    Q_OBJECT
public:
    static const int Max_Tabs = 10;   //最大支持的页面数

    QVector<QSqlTableModel *> studentTableModels;
    QVector<QItemSelectionModel *>studentSelections;

    static IDataBase &getInstance(){
        static IDataBase instance;
        return instance;
    }

    QString userLogin(QString userName,QString password);
    QString userSignUp(QString userName,QString password,QString confirmPassword);

    bool initStudentModel();
    bool searchStudent(int index,QString filter);
    void deleteStudent(int index);
    bool submitStudentEdit(int index);
    void revertStudentEdit(int index);
    int addNewStudent(int index);

private:
    explicit IDataBase(QObject *parent = nullptr);
    IDataBase(IDataBase const &) = delete;
    void operator=(IDataBase const &) = delete;

    QSqlDatabase database;

    void initDataBase();
};

#endif // IDATABASE_H
