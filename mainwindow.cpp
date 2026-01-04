#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "idatabase.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    IDataBase &db = IDataBase::getInstance();
    if(db.initStudentModel()){
        ui->tableView->setModel(db.studentTableModel);
        ui->tableView->setSelectionModel(db.studentSelection);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btEdit_clicked()
{
    QModelIndex curIndex = IDataBase::getInstance().studentSelection->currentIndex();
    studentEditView = new StudentEditView(this,curIndex.row());
    studentEditView->show();
}


void MainWindow::on_btSearch_clicked()
{
    QString SearchWay = ui->SearchBy->currentText();
    QString SearchText = ui->SearchText->text();
    QString filter;

    if(SearchWay == "姓名")
        filter = QString("Name like '%%1%'").arg(SearchText);
    else if(SearchWay == "学号")
        filter = QString("ID like '%%1%'").arg(SearchText);
    else if(SearchWay == "班级")
        filter = QString("Class like '%%1%'").arg(SearchText);
    else{
        QMessageBox::information(this,"错误","请选择一种查询方式",QMessageBox::Ok);
        return;
    }

    IDataBase::getInstance().searchStudent(filter);
}


void MainWindow::on_btAdd_clicked()
{
    int curRow = IDataBase::getInstance().addNewStudent();
    studentEditView = new StudentEditView(this,curRow);
    studentEditView->show();
}


void MainWindow::on_btDelete_clicked()
{
    IDataBase::getInstance().deleteStudent();
}


void MainWindow::on_btOrder_clicked()
{
    IDataBase &db = IDataBase::getInstance();
    QString sortBy = ui->OrderBy->currentText();
    Qt::SortOrder order;

    QString fieldName;
    if(sortBy == "学号")
        fieldName = "ID";
    else if(sortBy == "姓名")
        fieldName = "Name";
    else if(sortBy == "性别")
        fieldName = "Sex";
    else if(sortBy == "班级")
        fieldName = "Class";
    else if(sortBy == "数学成绩")
        fieldName = "MathScore";
    else if(sortBy == "C语言成绩")
        fieldName = "CScore";
    else if(sortBy == "Java成绩")
        fieldName = "JavaScore";
    else if(sortBy == "总分")
        fieldName = "TotalScore";
    else{
        QMessageBox::information(this,"错误","请选择一种排序方式",QMessageBox::Ok);
        return;
    }

    // 确定排序方向（升序/降序）
    if(ui->AscOrDesc->isChecked())
        order = Qt::AscendingOrder;
    else
        order = Qt::DescendingOrder;

    // 执行排序
    int columnIndex = db.studentTableModel->fieldIndex(fieldName);
    db.studentTableModel->setSort(columnIndex, order);
    db.studentTableModel->select();
}

