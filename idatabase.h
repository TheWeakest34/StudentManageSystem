#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QItemSelectionModel>

class IDataBase : public QObject
{
    Q_OBJECT
public:
    QSqlTableModel *studentTableModel;
    QItemSelectionModel *studentSelection;

    static IDataBase &getInstance(){
        static IDataBase instance;
        return instance;
    }

    QString userLogin(QString userName,QString password);
    QString userSignUp(QString userName,QString password,QString confirmPassword);

    bool initStudentModel();
    bool searchStudent(QString filter);
    void deleteStudent();
    bool submitStudentEdit();
    void revertStudentEdit();
    int addNewStudent();

private:
    explicit IDataBase(QObject *parent = nullptr);
    IDataBase(IDataBase const &) = delete;
    void operator=(IDataBase const &) = delete;

    QSqlDatabase database;

    void initDataBase();
};

#endif // IDATABASE_H
