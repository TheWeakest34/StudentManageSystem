#include "studenteditview.h"
#include "ui_studenteditview.h"
#include "idatabase.h"
#include <QMessageBox>
#include <QDebug>

StudentEditView::StudentEditView(QWidget *parent,int index,int tabIndex)
    : QDialog(parent)
    , ui(new Ui::StudentEditView)
{
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    curTabIndex = tabIndex;
    IDataBase &db = IDataBase::getInstance();
    tableModel = db.studentTableModels[curTabIndex];

    dataMapper->setModel(tableModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->ID,tableModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->Name,tableModel->fieldIndex("Name"));
    dataMapper->addMapping(ui->Class,tableModel->fieldIndex("Class"));
    dataMapper->addMapping(ui->MathScore,tableModel->fieldIndex("MathScore"));
    dataMapper->addMapping(ui->CScore,tableModel->fieldIndex("CScore"));
    dataMapper->addMapping(ui->JavaScore,tableModel->fieldIndex("JavaScore"));
    dataMapper->addMapping(ui->TotalScore,tableModel->fieldIndex("TotalScore"));

    dataMapper->setCurrentIndex(index);

    int sexColumn = tableModel->fieldIndex("Sex");
    QModelIndex sexIndex = tableModel->index(dataMapper->currentIndex(), sexColumn);
    tableModel->setData(sexIndex, ui->Sex->currentText());
}

StudentEditView::~StudentEditView()
{
    delete ui;
}

void StudentEditView::on_btSave_clicked()
{
    IDataBase &db = IDataBase::getInstance();
    if(ui->ID->text().isEmpty()){
        QMessageBox::warning(this,"提示","学号不能为空！",QMessageBox::Ok);
        db.revertStudentEdit(curTabIndex);
        return;
    }
    else if(ui->Name->text().isEmpty()){
        QMessageBox::warning(this,"提示","姓名不能为空！",QMessageBox::Ok);
        db.revertStudentEdit(curTabIndex);
        return;
    }
    else if(ui->Class->text().isEmpty()){
        QMessageBox::warning(this,"提示","班级不能为空！",QMessageBox::Ok);
        db.revertStudentEdit(curTabIndex);
        return;
    }

    db.submitStudentEdit(curTabIndex);
    this->close();
}

void StudentEditView::on_btCancel_clicked()
{
    IDataBase::getInstance().revertStudentEdit(curTabIndex);
    this->close();
}
