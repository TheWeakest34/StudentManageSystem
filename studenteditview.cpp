#include "studenteditview.h"
#include "ui_studenteditview.h"
#include "idatabase.h"
#include <QMessageBox>

StudentEditView::StudentEditView(QWidget *parent,int index)
    : QDialog(parent)
    , ui(new Ui::StudentEditView)
{
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    IDataBase &db = IDataBase::getInstance();
    tableModel = db.studentTableModel;

    dataMapper->setModel(tableModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->ID,tableModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->Name,tableModel->fieldIndex("Name"));
    dataMapper->addMapping(ui->Sex,tableModel->fieldIndex("Sex"));
    dataMapper->addMapping(ui->Class,tableModel->fieldIndex("Class"));
    dataMapper->addMapping(ui->MathScore,tableModel->fieldIndex("MathScore"));
    dataMapper->addMapping(ui->CScore,tableModel->fieldIndex("CScore"));
    dataMapper->addMapping(ui->JavaScore,tableModel->fieldIndex("JavaScore"));
    dataMapper->addMapping(ui->TotalScore,tableModel->fieldIndex("TotalScore"));

    dataMapper->setCurrentIndex(index);
}

StudentEditView::~StudentEditView()
{
    delete ui;
}

void StudentEditView::on_btSave_clicked()
{
    IDataBase &db = IDataBase::getInstance();
    if(ui->ID->text().isEmpty()){
        QMessageBox::information(this,"提示","学号不能为空！",QMessageBox::Ok);
        db.revertStudentEdit();
        return;
    }
    else if(ui->Name->text().isEmpty()){
        QMessageBox::information(this,"提示","姓名不能为空！",QMessageBox::Ok);
        db.revertStudentEdit();
        return;
    }
    else if(ui->Class->text().isEmpty()){
        QMessageBox::information(this,"提示","班级不能为空！",QMessageBox::Ok);
        db.revertStudentEdit();
        return;
    }

    db.submitStudentEdit();
    this->close();
}

void StudentEditView::on_btCancel_clicked()
{
    IDataBase::getInstance().revertStudentEdit();
    this->close();
}
