#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "idatabase.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initTableView(ui->tableView);
    initTableView(ui->tableView2);
    initTableView(ui->tableView3);

    IDataBase &db = IDataBase::getInstance();
    if(db.initStudentModel()){
        ui->tableView->setModel(db.studentTableModels[0]);
        ui->tableView->setSelectionModel(db.studentSelections[0]);

        ui->tableView2->setModel(db.studentTableModels[1]);
        ui->tableView2->setSelectionModel(db.studentSelections[1]);

        ui->tableView3->setModel(db.studentTableModels[2]);
        ui->tableView3->setSelectionModel(db.studentSelections[2]);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTableView(QTableView *tableview)
{
    tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableview->setSelectionMode(QAbstractItemView::SingleSelection);
    tableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableview->setAlternatingRowColors(true);
}

void MainWindow::on_btEdit_clicked()
{
    int tabIndex = ui->tabWidget->currentIndex();   //通过索引来找到对应模型
    QModelIndex curIndex = IDataBase::getInstance().studentSelections[tabIndex]->currentIndex();

    if(curIndex.row() < 0){
        QMessageBox::warning(this,"提示","请先选择一行!",QMessageBox::Ok);
        return;
    }

    studentEditView = new StudentEditView(this,curIndex.row(),tabIndex);
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
        QMessageBox::warning(this,"错误","请选择一种查询方式",QMessageBox::Ok);
        return;
    }

    int tabIndex = ui->tabWidget->currentIndex();
    IDataBase::getInstance().searchStudent(tabIndex,filter);
}


void MainWindow::on_btAdd_clicked()
{
    int tabIndex = ui->tabWidget->currentIndex();
    int curRow = IDataBase::getInstance().addNewStudent(tabIndex);
    studentEditView = new StudentEditView(this,curRow,tabIndex);
    studentEditView->show();
}


void MainWindow::on_btDelete_clicked()
{
    int tabIndex = ui->tabWidget->currentIndex();
    IDataBase::getInstance().deleteStudent(tabIndex);
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
        QMessageBox::warning(this,"错误","请选择一种排序方式",QMessageBox::Ok);
        return;
    }

    // 确定排序方向（升序/降序）
    if(ui->AscOrDesc->isChecked())
        order = Qt::AscendingOrder;
    else
        order = Qt::DescendingOrder;

    // 执行排序
    int tabIndex = ui->tabWidget->currentIndex();
    int columnIndex = db.studentTableModels[tabIndex]->fieldIndex(fieldName);
    db.studentTableModels[tabIndex]->setSort(columnIndex, order);
    db.studentTableModels[tabIndex]->select();
}
